#include "map_builder.h"

MapBuilder::MapBuilder(){

}

MapBuilder::~MapBuilder(){

}

void MapBuilder::run(message* msg){

  while(true){

    if(msg->new_frame_flag.read()==true){

      node_id++;
      Node* node=new Node();
      node->id.write(node_id);
      node->init(msg);

      graph_nodes[node_id]=node;

      if(node->id.read()>0){

        align_source_node(msg,node);

        int id2=node_id-1;

        BA.optimize(graph_nodes,node_id,node_id);

      }else{
        n_blocks=msg->n_blocks.read();
        Matches.init(n_blocks);
      }

      node->create_MapPoints(msg);

      show_image(msg,node,Matches);
      msg->new_frame_flag.write(false);

      Matches.reset();

    }else{

    }

    // detele old nodes
    if(graph_nodes.size()>param.NODE_BUFFER_SIZE){

        //std::lock_guard<std::mutex> lock(msg->mtx_nodes_map);
        std::map<int, Node*>::iterator it;
        it=graph_nodes.begin();
        delete it->second;
        graph_nodes.erase(it);
        //std::cout<<"Clear Old Node !!! "<<graph_nodes.size() <<std::endl;
    }

  }
}

void MapBuilder::align_source_node(message* msg, Node* source_node){

  int ref_node_id=source_node->id.read()-param.motion_model_window_size-1;
  if(ref_node_id<0)ref_node_id=0;

  int filter=1; //MA
  motion_model m;
  m.get_predicted_pose(graph_nodes, ref_node_id,  source_node, filter);

  int ntID1=source_node->id.read()-param.co_visible_window_size;
  if(ntID1<0)ntID1=0;
  int ntID2=source_node->id.read()-1;



  search_obzs_over_window(source_node->id.read(),ntID1,ntID2);

  computeSE3(msg,ExMz, node_id,ntID1,graph_nodes,Matches,param);

}

void MapBuilder::search_obzs_over_window(int nsID,int ntID1, int ntID2){



  for(int i=ntID1; i<=ntID2; i++){

    std::cout<<"Nref "<<ntID1 <<" Nt "<<i <<" Ns "<<nsID<<std::endl;
    search_obzs_frame(nsID,i,ntID1);

  }
}

void MapBuilder::search_obzs_frame(int nsID,int ntID,int refID){

  Node* nr=graph_nodes[refID];
  Node* nt=graph_nodes[ntID];
  Node* ns=graph_nodes[nsID];

  int n_threads=1;

  cv::Mat feature_grid_map1=ns->feature_grid_map1;
  Eigen::Matrix4d ref_pose=nr->pose.read();

  int thread_index=0;
  std::thread threads[n_threads];
  int n=0;
  for(int i=0; i<nt->kpts_MPs.size();i++){

    if(thread_index<n_threads){

      keypoint kpt=nt->kpts1.read_element(nt->kpts_MPs.read_element(i));

      threads[thread_index] = std::thread(&MapBuilder::search_obz,this,ns,nt,
                                          std::ref(kpt),kpt.descriptor,feature_grid_map1,std::ref(ref_pose));
      thread_index++;
    }

    if(thread_index>=n_threads || i>=nt->kpts_MPs.size()-1){
        //std::cout<<"Running 3D "<<thread_index<<std::endl;
        for(int j=0;j<thread_index;j++){
          threads[j].join();
        }
        thread_index=0;
    }
    n++;
  }

  std::cout<<"Matches/kpts_MPs "<<Matches.size()<<"/"<<nt->kpts_MPs.size()<<std::endl;
}

void MapBuilder::search_obz(Node* ns, Node* nt, keypoint& kpt, cv::Mat descriptor,
                            cv::Mat feature_grid_map1, Eigen::Matrix4d& ref_pose){

  int xp,yp;
  int min_dist=param.ORB_HAMMING_DISTANCE_TH+1;
  int matched_kptID=-1;
  int matched_dist=100;
  bool valid=false;
  bool far=false;
  int BlockID=-1;

  Eigen::Matrix4d Rpose_nt=ref_pose.inverse()*nt->pose.read();
  //Eigen::Matrix4d Rpose_ns=ref_pose.inverse()*ns->predicted_pose.read();

  Eigen::Matrix4d rpose_ns=nt->pose.read().inverse()*ns->predicted_pose.read();

  Eigen::Vector4d point4D=kpt.point4D;

  Eigen::Vector4d Rpoint4D=Rpose_nt*kpt.point4D;
  nt->kpts1.write_Rpoint4D(kpt.id,Rpoint4D);

  if(project_MapPoint(xp,yp,point4D,rpose_ns.inverse(), param)){

      int xMin=xp-param.search_radius;
      if(xMin<0)xMin=0;
      int xMax=xp+param.search_radius;
      if(xMax>param.width-1)xMax=param.width-1;

      int yMin=yp-param.search_radius;
      if(yMin<0)yMin=0;
      int yMax=yp+param.search_radius;
      if(yMax>param.height-1)yMax=param.height-1;

      for(int i=yMin; i<yMax;i++){

        for(int j=xMin; j<xMax; j++){

          if(feature_grid_map1.at<int>(i,j)!=-1){

            int kpsID=feature_grid_map1.at<int>(i,j);
            int dist;

            bool v;
            bool f;
            dist=ns->kpts1.descriptor_dist(kpsID, descriptor, v,f,BlockID);

            if(dist<=param.ORB_HAMMING_DISTANCE_TH){

              if(dist<min_dist){

                min_dist=dist;
                matched_dist=dist;
                matched_kptID=kpsID;
                valid=v;
                far=f;

                ns->kpts1.write_assigned(kpsID,true);

              }
            }
          }
        }
      }
      //std::cout<<"Mp "<<Mp.id<<" "<< matched_kptID<< " Dist "<< matched_dist<<std::endl;
    }

    // Match found or not
    if(matched_kptID!=-1){
      match m;
      m.SnodeID=ns->id.read();;
      m.SkptID=matched_kptID;
      m.TnodeID=nt->id.read();
      m.TkptID=kpt.id;
      m.Rpose_nt=Rpose_nt; //Eigen::Matrix4d::Identity();
      //m.Rpose_nt=Eigen::Matrix4d::Identity();

      // kp.far=Zw>param.max_depth;
      // kp.valid=Zw > 1.0;

      if(!far && !kpt.far ){

        if(valid)
          Matches.write_element_close(BlockID,m);

      }else{

        if(valid)
          Matches.write_element_far(BlockID,m);
      }

    }

}

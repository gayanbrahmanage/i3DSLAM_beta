#include "computeSE3.h"

void computeSE3(message* msg,
                em* ExMz,
                int node_id,
                int ref_node_id,
                std::map<int, Node*>& graph_nodes,
                SLAM_Match_Vector& m,
                parameters& param){

  SLAM_variable<Eigen::Matrix4d> tf;
  SLAM_Vector<match> inlier_matches;

  int n_threads=param.n_threads;

  int thread_index=0;
  std::thread threads[n_threads];
  bool tf_found=false;


  m.remove_unused_BlockIDs();

  for(int ransac_itr=0; ransac_itr< param.ransac_iterations; ransac_itr++ ){

      if(thread_index<n_threads){

        threads[thread_index] = std::thread(ransac,
                                            node_id,
                                            std::ref(graph_nodes),
                                            std::ref(m),
                                            std::ref(inlier_matches),
                                            std::ref(tf),
                                            msg,
                                            std::ref(param));

        thread_index++;
      }

      if(thread_index>=n_threads || ransac_itr>=param.ransac_iterations-1){
          //std::cout<<" Roi RANSAC "<<thread_index<<std::endl;
          for(int j=0;j<thread_index;j++){
            // thread join
            threads[j].join();
          }
          thread_index=0;
      }

  }

  SLAM_Vector<match> matches_far;


  Eigen::Matrix4d Rtf4d=tf.read();
  Eigen::Matrix4d testRtf4d=Rtf4d;

  //ExMz->estimate(node_id,graph_nodes,inlier_matches,Rtf4d,param);

  select_matches_far(graph_nodes,
                     m,
                     Rtf4d,
                     matches_far,
                     msg,
                     param);

  inlier_matches.merge(matches_far.read());

  //ExMz->estimate(node_id,graph_nodes,inlier_matches,Rtf4d,param);

  Eigen::Matrix4d Rpose_nt=graph_nodes[ref_node_id]->pose.read().inverse() * graph_nodes[node_id-1]->pose.read();
  Eigen::Matrix4d tf4d=Rpose_nt.inverse()*Rtf4d;

  graph_nodes[node_id]->tf.write(tf4d);
  graph_nodes[node_id]->pose.write(graph_nodes[node_id-1]->pose.read()*tf4d);
  graph_nodes[node_id]->inlier_matches.write(inlier_matches.read());
  //graph_nodes[node_id]->inlier_matches.merge(matches_far);

  msg->n_matches.write(inlier_matches.size());

  std::cout<<"Matches Selected ["<<inlier_matches.size()<<"+"<<matches_far.size()<<"]/"<<m.n_matches<<std::endl;

}

void ransac(int node_id,
            std::map<int, Node*>& graph_nodes,
            SLAM_Match_Vector& m,
            SLAM_Vector<match>& inlier_matches,
            SLAM_variable<Eigen::Matrix4d>& tf,
            message* msg,
            parameters& param){

  SE3 SE3TF;

  std::vector<match> matches;

  std::vector<match> sample=init_sample(m,param);

  Eigen::Matrix4d tf_ransac=computeTF(graph_nodes,sample,param);

  select_matches_close(graph_nodes,
                       m,
                       tf_ransac,
                       matches,
                       msg,
                       param);

  // std::cout<<"RANSAC match "<<matches.size()<<" "<<std::endl;
  // std::cout<<"tf "<<std::endl<<tf_ransac <<std::endl;
  //std::cout<<sample[0].SnodeID<<" "<<sample[0].SkptID<<std::endl;

   if(inlier_matches.size()<matches.size()){

     inlier_matches.write(matches);
     tf.write(tf_ransac);
   }

}

std::vector<match> init_sample(SLAM_Match_Vector& m,parameters& param){

  std::vector<match> sample;
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

  std::shuffle(m.vec_close_BlockIDs.begin(), m.vec_close_BlockIDs.end(), std::default_random_engine(seed));

  std::vector<int> BlockIDs(m.vec_close_BlockIDs.begin(),m.vec_close_BlockIDs.begin()+param.sample_size);

  for(int i=0; i<BlockIDs.size(); i++){

    int size=m.vec_close[BlockIDs[i]].size();
    int index=rand()%size;
    sample.push_back(m.vec_close[BlockIDs[i]][index]);

  }

  return sample;
}

Eigen::Matrix4d computeTF(std::map<int, Node*>& graph_nodes,
                          std::vector<match>& sample,
                          parameters& param){


  //
  SE3 SE3TF;
  int size=sample.size();
  for(int i=0;i<size;i++){

    match m=sample[i];

    Eigen::Vector4d point4D=graph_nodes[m.SnodeID]->kpts1.read_point4D(m.SkptID);
    Eigen::Vector4d corresponding_point4D=graph_nodes[m.TnodeID]->kpts1.read_Rpoint4D(m.TkptID);

    Eigen::Vector3d point=point4D.head(3);

    Eigen::Vector3d corresponding_point=corresponding_point4D.head(3);

    SE3TF.compute_statistics(point,corresponding_point);
  }

  return SE3TF.compute();

}

void select_matches_close(std::map<int, Node*>& graph_nodes,
                          SLAM_Match_Vector& m,
                          Eigen::Matrix4d &Rtf,
                          std::vector<match>& matches,
                          message* msg,
                          parameters& param){

  int n=0;
  double squaredMaxInlierDistInM=param.ransac_max_dist_pix*param.ransac_max_dist_pix;

  //Eigen::Matrix4d tfinv=tf.inverse();

  for(std::vector<std::vector<match>>::iterator it= m.vec_close.begin(); it!=m.vec_close.end(); ++it){

    for(std::vector<match>::iterator it2=(*it).begin();it2!=(*it).end();++it2){

      match m=*it2;

      keypoint kps=graph_nodes[m.SnodeID]->kpts1.read_element(m.SkptID);
      keypoint kpt=graph_nodes[m.TnodeID]->kpts1.read_element(m.TkptID);
      Eigen::Matrix4d tf= m.Rpose_nt.inverse()*Rtf;

      //std::cout<<m.Rpose_nt<<std::endl<<std::endl<<std::endl;
      Eigen::Matrix4d tfinv=tf.inverse();

      double error1=error_pix(kps,kpt,tf,msg,param);
      double error2=error_pix(kpt,kps,tfinv,msg,param);

      if(error1 <squaredMaxInlierDistInM && error2 <squaredMaxInlierDistInM){
        matches.push_back(m);
        n++;
      }
    }
  }
}

void select_matches_far(std::map<int, Node*>& graph_nodes,
                        SLAM_Match_Vector& m,
                        Eigen::Matrix4d &Rtf,
                        SLAM_Vector<match>& matches,
                        message *msg,
                        parameters& param){

  int n=0;
  double squaredMaxInlierDistInM=param.ransac_max_dist_pix*param.ransac_max_dist_pix;

  //Eigen::Matrix4d tfinv=tf.inverse();

  for(std::vector<std::vector<match>>::iterator it= m.vec_far.begin(); it!=m.vec_far.end(); ++it){

    for(std::vector<match>::iterator it2=(*it).begin();it2!=(*it).end();++it2){

      match m=*it2;

      keypoint kps=graph_nodes[m.SnodeID]->kpts1.read_element(m.SkptID);
      keypoint kpt=graph_nodes[m.TnodeID]->kpts1.read_element(m.TkptID);
      Eigen::Matrix4d tf= m.Rpose_nt.inverse()*Rtf;
      Eigen::Matrix4d tfinv=tf.inverse();

      double error1=error_pix(kps,kpt,tf,msg,param);
      double error2=error_pix(kpt,kps,tfinv,msg,param);

      if(error1 <squaredMaxInlierDistInM && error2 <squaredMaxInlierDistInM){
        matches.push_back(m);
        n++;
      }
    }
  }
}

double error_pix(keypoint& kpt_source,
                 keypoint& kpt_target,
                 Eigen::Matrix4d& tf,
                 message *msg,
                 parameters& param){


  Eigen::Vector4d pt_3D_target_hat=tf*kpt_source.point4D;

  double invZ=1.0/pt_3D_target_hat(2);
  double xe = msg->cx + msg->fx * pt_3D_target_hat(0) * invZ;
  double xe2 = msg->cx + msg->fx * (pt_3D_target_hat(0)-msg->b )* invZ;
  double ye = msg->cy + msg->fy * pt_3D_target_hat(1) * invZ;


  double distX = kpt_target.LKeyPoint.pt.x-xe;
  double distX2 = kpt_target.RKeyPoint.pt.x-xe2;
  double distY = kpt_target.LKeyPoint.pt.y-ye;

  //double error2 = distX*distX+distY*distY;
  double error2 = distX*distX+distY*distY+distX2*distX2;
  //std::cout<<error2<<std::endl;
  return error2;

}

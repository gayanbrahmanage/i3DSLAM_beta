#include <node.h>
#include "message.h"

Node::Node(){

}

Node::~Node(){

}

void Node::init(message* msg){


  frame_number.write(msg->frame_number.read());
  time_stamp.write(msg->time_stamp.read());

  int width=msg->image_w.read();
  int height=msg->image_h.read();

  image_w.write(width);
  image_h.write(height);


  row_kpts1=std::vector<std::vector<int>>(height,std::vector<int>(1,-1));
  feature_grid_map1=cv::Mat(cv::Size(width,  height), CV_32SC1, -1);

  row_kpts2=std::vector<std::vector<int>>(height,std::vector<int>(1,-1));
  feature_grid_map2=cv::Mat(cv::Size(width,  height), CV_32SC1, -1);


  std::thread t(detect_orb,std::ref(msg->image2),std::ref(feature_grid_map2),std::ref(row_kpts2),std::ref(kpts2),std::ref(param));
  detect_orb(msg->image1,feature_grid_map1,row_kpts1,kpts1,param);
  t.join();


  //get3D(kpts1,kpts2,row_kpts1,row_kpts2,feature_grid_map2,param);

  getSP3D(kpts1,kpts2,row_kpts1,row_kpts2,feature_grid_map2,msg->image3,param);

  // initialization
  pose.write(Eigen::Matrix4d::Identity());
  predicted_pose.write(Eigen::Matrix4d::Identity());
  tf.write(Eigen::Matrix4d::Identity());
  predicted_tf.write(Eigen::Matrix4d::Identity());

}

void Node::create_MapPoints(message* msg){

  if(msg->frame_number.read()==0){

    for(int i=0; i<kpts1.size(); i++){

      if(!kpts1.read_valid(i))
        continue;

      msg->MapPointID++;
      kpts1.write_MapPointID(i,msg->MapPointID);
      kpts_MPs.push_back(i);

    }

  }else{
    for(int i=0; i<kpts1.size(); i++){

      if(!kpts1.read_valid(i))
        continue;

      if(kpts1.read_assigned(i))
        continue;

      msg->MapPointID++;
      kpts1.write_MapPointID(i,msg->MapPointID);
      kpts_MPs.push_back(i);

    }
  }
}

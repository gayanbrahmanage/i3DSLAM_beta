
#include "viz.h"
#include <Eigen/Dense>

int main(int argc, char* argv[]){

  viz GUI;
  //GUI.init("cam" /*Pose type */, 0.01f /*pose scale*/);

  while(true){

    // std::vector<Eigen::Matrix4d,Eigen::aligned_allocator<Eigen::Matrix4d>> pose;
    // Eigen::Matrix4d P1=Eigen::Matrix4d::Identity();
    // Eigen::Matrix4d P=Eigen::Matrix4d::Identity();
    // P1(2,3)=-0.1;
    // for(int i=0; i<10; i++){
    //   P=P*P1;
    //   pose.push_back(P);
    // }
    // GUI.draw(pose);
  }

  GUI.~viz();
  return 0;
}

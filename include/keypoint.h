#ifndef KEYPOINT_H
#define KEYPOINT_H

#include "parameters.h"

struct keypoint{
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  int id=-1;
  int MapPointID=-1;
  int FrameID=-1;
  int depth_type=-1;
  int BlockID=-1;
  cv::KeyPoint LKeyPoint;
  cv::KeyPoint RKeyPoint;
  int xp=-1;
  int yp=-1;
  Eigen::Vector4d point4D=Eigen::Vector4d(0,0,0,1);
  Eigen::Vector4d Rpoint4D=Eigen::Vector4d(0,0,0,1);

  double disparity=-1;
  bool far=false;
  bool valid=false;
  bool assigned=false;
  cv::Mat descriptor;
  int covilibility_index=0;
  Eigen::Matrix3d kptinfoMatrix=Eigen::Matrix3d::Identity();
};

struct match{
  int SnodeID;
  int SkptID;
  int TnodeID;
  int TkptID;
  Eigen::Matrix4d Rpose_nt;

};

struct Matches{
  std::map<int,std::vector<match>> Matches_Close;
  std::vector<int> Matches_Close_BlockIDs;
  std::map<int,std::vector<match>> Matches_Far;
  std::vector<int> Matches_Far_BlockIDs;
};


#endif

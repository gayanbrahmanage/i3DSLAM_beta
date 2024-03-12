#ifndef NODE_H
#define NODE_H

#include "features2D.h"
#include "stereo3D.h"
#include "stereoP3D.h"

class Node{

  private:

  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW //http://eigen.tuxfamily.org/dox-devel/group__TopicStructHavingEigenMembers.html

    parameters param;
    SLAM_variable<int> id;
    SLAM_variable<int> vertex_id;
    int MapPointID=-1;

    SLAM_variable<int> frame_number;
    SLAM_variable<bool> KF;
    SLAM_variable<float> time_stamp; // seconds
    SLAM_variable<int> image_w;
    SLAM_variable<int> image_h;

    std::vector<std::vector<int>> row_kpts1;
    std::vector<std::vector<int>> row_kpts2;

    cv::Mat feature_grid_map1;
    cv::Mat feature_grid_map2;

    SLAM_KPT_Vector kpts1;
    SLAM_KPT_Vector kpts2;

    SLAM_variable<Eigen::Matrix4d> pose;
    SLAM_variable<Eigen::Matrix4d> predicted_pose;
    SLAM_variable<Eigen::Matrix4d> tf; // realative tf to the target
    SLAM_variable<Eigen::Matrix4d> predicted_tf; // realative tf to the target

    SLAM_Vector<int> kpts_MPs;
    SLAM_Vector<match> inlier_matches;
    //SLAM_Match_Vector Matches;


    Node();
    ~Node();

    void init(message* msg);
    void create_MapPoints(message* msg);

};

 #endif

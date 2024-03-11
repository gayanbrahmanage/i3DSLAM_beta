#ifndef message_H
#define message_H

#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs.hpp"

#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <map>
#include <chrono>
#include <condition_variable> // std::condition_variable
#include "variable.h"
#include "keypoint.h"

#define EigenMat4dMap std::map<int, Eigen::Matrix4d, std::less<int>, Eigen::aligned_allocator<std::pair<const int, Eigen::Matrix4d>>>

class message{

  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    int MapPointID=-1;
    bool PD_DEPTH_EN=true;

    SLAM_variable<bool> new_frame_flag;
    SLAM_variable<bool> end_flag;
    SLAM_variable<int> frame_number;
    SLAM_variable<cv::Mat> image1;
    SLAM_variable<cv::Mat> image2;
    cv::Mat image3;
    SLAM_variable<float> time_stamp;
    SLAM_variable<int> image_w;
    SLAM_variable<int> image_h;
    SLAM_variable<bool> draw_kpts;
    SLAM_variable<bool> draw_matches;
    SLAM_variable<double> fps;
    SLAM_variable<double> n_blocks;
    SLAM_variable<int> n_matches;

    EigenMat4dMap poseGT;
    EigenMat4dMap pose;

    message();
    ~message();

    void init();

};

#endif

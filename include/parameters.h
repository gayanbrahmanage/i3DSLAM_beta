#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <Eigen/Core>
#include <vector>
#include <mutex>
#include <map>
#include <chrono>
#include <condition_variable> // std::condition_variable
#include "message.h"

class parameters{
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    int n_threads=16;
    //int gui_width=1460;
    //int gui_height=920;
    int i3D_width=1280;
    int i3D_height=720;
    int video_width=1280;
    int video_height=370;

    cv::Scalar gui_b_color=cv::Scalar(16,16,16);
    cv::Scalar gui_panel_color=cv::Scalar(28,28,28);

    int width=1226;
    int height=370;
    float max_depth=40.0f;

    int NODE_BUFFER_SIZE=20;
    int grid_x=100;
    int grid_y=100;
    int marging=8;

    // param_setting
    int nfeatures_per_grid_block=100;
    float scaleFactor = 1.2f;
    int nlevels = 8;
    int edgeThreshold = 19;
    int firstLevel = 0;
    int WTA_K = 2;
    int patchSize = 31;
    int fastThreshold = 20;
    int motion_model_window_size=3;
    int ORB_HAMMING_DISTANCE_TH=64;
    int search_radius=50;
    int sample_size=3;
    double ransac_max_dist_pix=8;
    int ransac_iterations=96;
    int co_visible_window_size=5;
    int g2o_iterations_local_BA=20;

    parameters();
    ~parameters();


};

#endif

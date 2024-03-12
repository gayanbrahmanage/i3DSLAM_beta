#ifndef stereoP3D_H
#define stereoP3D_H

#include "features2D.h"
#include "parameters.h"
#include <thread>
#include "message.h"
#include "P3D.h"


int DescriptorDistanceST(const cv::Mat &a, const cv::Mat &b);

void get3Dpoint(int index,
                SLAM_KPT_Vector& kpts1,
                SLAM_KPT_Vector& kpts2,
                std::vector<std::vector<int>>& row_kpts,
                std::vector<std::vector<int>>& row_kpts2,
                const cv::Mat& feature_grid_map2,
                const cv::Mat& depth_image,
                message* msg,
                parameters& param);

void getSP3D( SLAM_KPT_Vector& kpts1,
            SLAM_KPT_Vector& kpts2,
            std::vector<std::vector<int>>& row_kpts,
            std::vector<std::vector<int>>& row_kpts2,
            const cv::Mat& feature_grid_map2,
            const cv::Mat& depth_image,
            message* msg,
            parameters& param);

#endif

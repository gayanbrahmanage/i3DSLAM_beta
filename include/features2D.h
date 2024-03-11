#ifndef FEATURES2D_H
#define FEATURES2D_H

#include "message.h"
#include "parameters.h"
#include <thread>

int DescriptorDistance(const cv::Mat &a, const cv::Mat &b);

void detect_orb(SLAM_variable<cv::Mat>& input,
                cv::Mat& feature_grid_map,
                std::vector<std::vector<int>>& row_kpts,
                SLAM_KPT_Vector& kpts,
                parameters &param);


void block_DescriptorExtractor(int,
                               cv::Mat& roi,
                               SLAM_KPT_Vector& kpts,
                               cv::Mat& feature_grid_map,
                               std::vector<std::vector<int>>& row_kpts,
                               cv::Rect& r,
                               parameters &param);

#endif

#ifndef P3D_H
#define P3D_H

#include "parameters.h"
#include "message.h"

float get_P3D(const cv::Mat&, int x, int y);
float decode_depth_png(const cv::Mat& src, int x, int y);

#endif

#ifndef UTILS_H
#define UTILS_H

#include "message.h"
#include "node.h"
#include <filesystem>

class time_record{
  public:
    unsigned long time;
    time_record();
    ~time_record();
    void start();
    unsigned int stop();

};

void show_image(message* msg, Node* node,SLAM_Match_Vector& Matches);
void draw_kpts(cv::Mat& image, Node* node);
void draw_key_points(cv::Mat& image, std::vector<keypoint> &kpts, cv::Scalar color, int marker_size, int line_thicknes);

void draw_all_matches(cv::Mat& image, Node* node, SLAM_Match_Vector& Matches);

void draw_matches(cv::Mat& image, Node* node);

void draw_init_matches(message* msg, Node* node);
void draw_key_points(cv::Mat& image, SLAM_KPT_Vector& kpts, cv::Scalar color, int marker_size, int line_thicknes);
void draw_ProjectedMapPoints(message* msg, Node* node);

void save_vector_to_txt(std::vector<float>, std::string name);

#endif

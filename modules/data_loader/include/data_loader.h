#ifndef DATALOADER_H
#define DATALOADER_H

#include "message.h"
#include "parameters.h"
#include "utils.h"

class kitti{
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    parameters param;
    time_record tr;
    std::string data_folder="/home/bgs/data/";
    int sq_no=0;
    int start_frame_number=0;
    int frame_number=0;
    std::vector<float> times;
    std::map<int, Eigen::Matrix4d, std::less<int>, Eigen::aligned_allocator<std::pair<const int, Eigen::Matrix4d>>> gt;

    kitti(int,message*);
    ~kitti();

    void read_frame(message* msg);
    void read_time_stamp();
    void read_gt(message* msg);
    void run(message* msg);
    void read_calib(message* msg);
};

#endif

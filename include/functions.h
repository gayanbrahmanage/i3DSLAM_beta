#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "message.h"

bool project_MapPoint(int& x, int&y, Eigen::Vector4d& point4D, Eigen::Matrix4d tf,message* msg,parameters& param);

#endif

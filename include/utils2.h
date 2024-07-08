#ifndef UTILS2_H
#define UTILS2_H
#include <Eigen/Dense>
// Function to rotate a matrix around the X, Y, and Z axes
Eigen::Matrix4d rotateMatrix(const Eigen::Matrix4d& matrix, double angleX, double angleY, double angleZ);


#endif

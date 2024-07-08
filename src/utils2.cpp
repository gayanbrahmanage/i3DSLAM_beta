#include <utils2.h>
#include <cmath>

// Convert degrees to radians
constexpr double degreesToRadians(double degrees) {
    return degrees * M_PI / 180.0;
}

// Function to create rotation matrix around the X-axis
Eigen::Matrix4d rotationMatrixX(double angleDegrees) {
    double angleRadians = degreesToRadians(angleDegrees);
    Eigen::Matrix4d rotationMatrix = Eigen::Matrix4d::Identity();
    rotationMatrix(1, 1) = std::cos(angleRadians);
    rotationMatrix(1, 2) = -std::sin(angleRadians);
    rotationMatrix(2, 1) = std::sin(angleRadians);
    rotationMatrix(2, 2) = std::cos(angleRadians);
    return rotationMatrix;
}

// Function to create rotation matrix around the Y-axis
Eigen::Matrix4d rotationMatrixY(double angleDegrees) {
    double angleRadians = degreesToRadians(angleDegrees);
    Eigen::Matrix4d rotationMatrix = Eigen::Matrix4d::Identity();
    rotationMatrix(0, 0) = std::cos(angleRadians);
    rotationMatrix(0, 2) = std::sin(angleRadians);
    rotationMatrix(2, 0) = -std::sin(angleRadians);
    rotationMatrix(2, 2) = std::cos(angleRadians);
    return rotationMatrix;
}

// Function to create rotation matrix around the Z-axis
Eigen::Matrix4d rotationMatrixZ(double angleDegrees) {
    double angleRadians = degreesToRadians(angleDegrees);
    Eigen::Matrix4d rotationMatrix = Eigen::Matrix4d::Identity();
    rotationMatrix(0, 0) = std::cos(angleRadians);
    rotationMatrix(0, 1) = -std::sin(angleRadians);
    rotationMatrix(1, 0) = std::sin(angleRadians);
    rotationMatrix(1, 1) = std::cos(angleRadians);
    return rotationMatrix;
}

// Function to rotate a matrix around the X, Y, and Z axes
Eigen::Matrix4d rotateMatrix(const Eigen::Matrix4d& matrix, double angleX, double angleY, double angleZ) {
    Eigen::Matrix4d rotationX = rotationMatrixX(angleX);
    Eigen::Matrix4d rotationY = rotationMatrixY(angleY);
    Eigen::Matrix4d rotationZ = rotationMatrixZ(angleZ);

    // Combine the rotations
    Eigen::Matrix4d rotation = rotationZ * rotationY * rotationX;

    // Apply the combined rotation to the input matrix
    return rotation * matrix;
}

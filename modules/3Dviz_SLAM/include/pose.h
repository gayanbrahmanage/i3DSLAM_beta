#ifndef pose_H
#define pose_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "viz_parameters.h"
#include <Eigen/Dense>
#include <vector>
#include <map>

#define EigenMat4dMap std::map<int, Eigen::Matrix4d, std::less<int>, Eigen::aligned_allocator<std::pair<const int, Eigen::Matrix4d>>>

class pose{
public:
  float scale=0.01f;
  viz_parameters* viz_param= new viz_parameters;
  unsigned int VBO, VAO, EBO;
  int vertices_size;
  int indices_size;
  std::string draw_type;
  std::vector<float> color={1.0f,1.0f,1.0f};

  pose();
  ~pose();
  void init_pose(float size, std::string,std::vector<float> marker_color);
  void draw_pose(EigenMat4dMap& pose);

};
#endif

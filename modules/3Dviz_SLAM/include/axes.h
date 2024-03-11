#ifndef axes_H
#define axes_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "viz_parameters.h"

class axes{
public:
  viz_parameters* viz_param= new viz_parameters;
  unsigned int VBO, VAO, EBO;
  unsigned int gVBO, gVAO;
  float grid_size;
  float grid_space;
  std::string grid_plane;

  axes();
  ~axes();
  void init_axes(float size);
  void draw_axes();

  void init_grid(float size, float space, std::string plane);
  void draw_grid();

};
#endif

#ifndef viz_H
#define viz_H

#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <vector>


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <shader_m.h>
#include <camera.h>
#include "viz_parameters.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <axes.h>
#include <pose.h>
#include <message.h>

class viz{
public:

  glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

  axes ax;
  bool grid_on=false;
  pose _pose;
  pose _pose2;
  pose _pose_gt;
  //shader
  unsigned int shaderProgram;
  unsigned int fragmentShader;
  unsigned int vertexShader;

  Shader ourShader;
  //Camera cam;
  // FLAGS
  bool MOUSE_BUTTON_MIDDLE_PRESSED=false;


  viz_parameters* viz_param= new viz_parameters;
  GLFWwindow* window;

  viz();
  ~viz();
  void init(std::string pose_type, float pose_scale, bool, std::string);
  void draw_axes(float, float);
  void draw(message* msg);
  void update_view(double radius, double anglex, double angley);
  void save_window();


};

#endif

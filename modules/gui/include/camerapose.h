#ifndef camerapose_H
#define camerapose_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm.h>
#include <epoxy/gl.h>
#include <gtkmm/application.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "parameters.h"
#include <utils2.h>
#include <Eigen/Core>
#include <Eigen/Dense>

static float size_cam=0.1;

static const GLfloat vertex_data_cam[] = {
  size_cam, -size_cam, -size_cam, 1.0, -1.0, 0.0,
  size_cam, -size_cam, size_cam , 1.0, -1.0, 0.0,
  -size_cam, -size_cam, size_cam, 1.0, -1.0, 0.0,
  size_cam, -size_cam, -size_cam, 1.0, -1.0, 0.0,
  -size_cam, -size_cam, size_cam, 1.0, -1.0, 0.0,
  -size_cam, -size_cam, -size_cam, 1.0, -1.0, 0.0,


  -size_cam, size_cam, size_cam, 0.0, 1.0, 0.0,
  size_cam, size_cam, size_cam,  0.0, 1.0, 0.0,
  size_cam, size_cam, -size_cam, 0.0, 1.0, 0.0,
  -size_cam, size_cam, size_cam, 0.0, 1.0, 0.0,
  size_cam, size_cam, -size_cam, 0.0, 1.0, 0.0,
  -size_cam, size_cam,-size_cam, 0.0, 1.0, 0.0,


  -size_cam, -size_cam,-size_cam, -1.0, 0.0, 0.0,
  -size_cam, -size_cam, size_cam, -1.0, 0.0, 0.0,
  -size_cam, size_cam, -size_cam, -1.0, 0.0, 0.0,
  -size_cam, -size_cam, size_cam, -1.0, 0.0, 0.0,
  -size_cam, size_cam, size_cam, -1.0, 0.0, 0.0,
  -size_cam, size_cam, -size_cam, -1.0, 0.0, 0.0,

  -size_cam, -size_cam,size_cam, 0.0, 1.0, 1.0,
  size_cam, -size_cam, size_cam, 0.0, 1.0, 1.0,
  -size_cam, size_cam, size_cam, 0.0, 1.0, 1.0,
  size_cam, -size_cam, size_cam, 0.0, 1.0, 1.0,
  size_cam, size_cam, size_cam, 0.0, 1.0, 1.0,
  -size_cam, size_cam, size_cam, 0.0, 1.0, 1.0,

  size_cam, size_cam, -size_cam, 1.0, 0.0, -1.0,
  size_cam,-size_cam, -size_cam, 1.0, 0.0, -1.0,
  -size_cam, -size_cam, -size_cam, 1.0, 0.0, -1.0,
  size_cam, size_cam, -size_cam, 1.0, 0.0, -1.0,
  -size_cam, -size_cam, -size_cam, 1.0, 0.0, -1.0,
  -size_cam, size_cam, -size_cam, 1.0, 0.0, -1.0,

  size_cam, size_cam, size_cam,1.0, 0.0, 0.0,
  size_cam, -size_cam, size_cam,1.0, 0.0, 0.0,
  size_cam, -size_cam,-size_cam,1.0, 0.0, 0.0,
  size_cam, size_cam, size_cam,1.0, 0.0, 0.0,
  size_cam, -size_cam, -size_cam,1.0, 0.0, 0.0,
  size_cam, size_cam, -size_cam,1.0, 0.0, 0.0

};


class camerapose{

public:

  parameters* param=new parameters();

  GLuint VAO {0};
  GLuint VBO {0};
  GLuint EBO {0};

  glm::mat4 model = glm::mat4(1.0);
  Eigen::Matrix4d pose=Eigen::Matrix4d::Identity();

  float size=1.0;

  camerapose();
  ~camerapose();

  void init();
  void set_pose(glm::mat4 tf);
  void draw(GLuint Program3D, glm::mat4 tf);
  void reset();
  glm::mat4 Eigen4x4ToGLMmat4(Eigen::Matrix4d);

};


#endif

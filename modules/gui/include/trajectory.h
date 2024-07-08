#ifndef trajectory_H
#define trajectory_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm.h>
#include <epoxy/gl.h>
#include <gtkmm/application.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "parameters.h"
#include "message.h"
#include <Eigen/Core>
#include <Eigen/Dense>
#include <utils2.h>

class trajectory{

public:

  parameters* param=new parameters();

  GLuint VAO {0};
  GLuint VBO {0};
  GLuint EBO {0};

  glm::mat4 model = glm::mat4(1.0);
  std::vector<GLfloat> vertices;
  float linewidth=3.0;

  trajectory();
  ~trajectory();

  void init();
  void set_pose(glm::mat4 tf);
  void update_trajectory(message* msg);
  void draw(GLuint Program3D, glm::mat4 tf,  message* msg);
  void reset();
  glm::mat4 Eigen4x4ToGLMmat4(Eigen::Matrix4d);

};


#endif

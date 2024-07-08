#ifndef axis_H
#define axis_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm.h>
#include <epoxy/gl.h>
#include <gtkmm/application.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "parameters.h"

class axis{

public:

  parameters* param=new parameters();

  GLuint VAO {0};
  GLuint VBO {0};
  GLuint EBO {0};

  float size=1.0;
  glm::mat4 model = glm::mat4(1.0);
  float linewidth=3.0;

  axis();
  ~axis();

  void init();
  void draw(GLuint Program3D, glm::mat4 tf);
  void set_pose(glm::mat4 tf);
  void reset();

};


#endif

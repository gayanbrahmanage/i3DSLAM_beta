#ifndef grid_H
#define grid_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm.h>
#include <epoxy/gl.h>
#include <gtkmm/application.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "parameters.h"

class grid{

public:

  parameters* param=new parameters();

  int n=50;
  GLuint VAO {0};
  GLuint VBO {0};
  GLuint EBO {0};

  float grid_size=100;
  float grid_units=10;
  glm::mat4 model = glm::mat4(1.0);
  float linewidth=1.0;

  grid();
  ~grid();

  void init();
  void draw(GLuint Program3D, glm::mat4 tf);
  void set_pose(glm::mat4 tf);
  void reset();

};


#endif

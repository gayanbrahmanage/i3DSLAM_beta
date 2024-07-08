#ifndef logo_H
#define logo_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm.h>
#include <epoxy/gl.h>
#include <gtkmm/application.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "parameters.h"
#include <opencv2/opencv.hpp>

static const GLfloat vertex_data[] = {
  1.0, -1.0, -1.0, 0.0, -1.0, 0.0,
  1.0, -1.0, 1.0, 0.0, -1.0, 0.0,
  -1.0, -1.0, 1.0, 0.0, -1.0, 0.0,
  1.0, -1.0, -1.0, 0.0, -1.0, 0.0,
  -1.0, -1.0, 1.0, 0.0, -1.0, 0.0,
  -1.0, -1.0,-1.0, 0.0, -1.0, 0.0,

  -1.0, 1.0, 1.0, 0.0, 1.0, 0.0,
  1.0, 1.0, 1.0, 0.0, 1.0, 0.0,
  1.0, 1.0, -1.0, 0.0, 1.0, 0.0,
  -1.0, 1.0, 1.0, 0.0, 1.0, 0.0,
  1.0, 1.0, -1.0, 0.0, 1.0, 0.0,
  -1.0, 1.0,-1.0, 0.0, 1.0, 0.0,

  -1.0, -1.0,-1.0, -1.0, 0.0, 0.0,
  -1.0, -1.0, 1.0, -1.0, 0.0, 0.0,
  -1.0, 1.0, -1.0, -1.0, 0.0, 0.0,
  -1.0, -1.0, 1.0, -1.0, 0.0, 0.0,
  -1.0, 1.0, 1.0, -1.0, 0.0, 0.0,
  -1.0, 1.0, -1.0, -1.0, 0.0, 0.0,

  -1.0, -1.0,1.0, 0.0, 0.0, 1.0,
  1.0, -1.0, 1.0, 0.0, 0.0, 1.0,
  -1.0, 1.0, 1.0, 0.0, 0.0, 1.0,
  1.0, -1.0, 1.0, 0.0, 0.0, 1.0,
  1.0, 1.0, 1.0, 0.0, 0.0, 1.0,
  -1.0, 1.0, 1.0, 0.0, 0.0, 1.0,

  1.0, 1.0, -1.0, 0.0, 0.0, -1.0,
  1.0,-1.0, -1.0, 0.0, 0.0, -1.0,
  -1.0, -1.0, -1.0, 0.0, 0.0, -1.0,
  1.0, 1.0, -1.0, 0.0, 0.0, -1.0,
  -1.0, -1.0, -1.0, 0.0, 0.0, -1.0,
  -1.0, 1.0, -1.0, 0.0, 0.0, -1.0,

  1.0, 1.0, 1.0,1.0, 0.0, 0.0,
  1.0, -1.0, 1.0,1.0, 0.0, 0.0,
  1.0, -1.0,-1.0,1.0, 0.0, 0.0,
  1.0, 1.0, 1.0,1.0, 0.0, 0.0,
  1.0, -1.0, -1.0,1.0, 0.0, 0.0,
  1.0, 1.0, -1.0,1.0, 0.0, 0.0

};

class logo{

public:

  parameters* param=new parameters();
  cv::Mat image;
  unsigned int texture;

  GLuint Vao {0};
  GLuint Buffer {0};

  long current_frame = 0.0;
  long delta_time = 0.0;
  GDateTime *last_frame;
  int dt = 0;
  glm::mat4 model = glm::mat4(1.0);

  logo();
  ~logo();

  void init();
  void read_image();
  void draw(GLuint );
  void reset();

};


#endif

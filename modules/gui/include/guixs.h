#ifndef guixs_H
#define guixs_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm.h>
#include <giomm/resource.h>
#include <epoxy/gl.h>
#include <gtkmm/application.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "parameters.h"

struct eye{
  glm::mat4 model = glm::mat4(1.0);
  glm::vec3 position = glm::vec3(0,0,5);
  glm::vec3 front = glm::vec3(0,0,-1);
  glm::vec3 up = glm::vec3(0,1,0);
  long delta_time=0;
  long current_frame = 0.0;
  GDateTime *last_frame=g_date_time_new_now_local();
  int dt = 0;
};

static const char  *VERTEX_SOURCE =
"#version 330\n"
"in vec3 position;\n"
"in vec3 normal;\n"
"out vec3 transformedNormal;\n"
"out vec3 originalNormal;\n"
"uniform mat4 projection;\n"
"uniform mat4 view;\n"
"uniform mat4 model;\n"
"void main(){\n"
"    gl_Position =  projection * view * model * vec4(position, 1.0);\n"
"    mat3 normalMatrix = transpose(inverse(mat3(view * model)));\n"
"    transformedNormal = normalMatrix * normal;\n"
"    originalNormal = abs(normal);\n"
"}\n";

static const GLchar *FRAGMENT_SOURCE =
"#version 330\n"
"in vec3 transformedNormal;\n"
"in vec3 originalNormal;\n"
"out vec4 outputColor;\n"
"void main() {\n"
"vec3 color = originalNormal;\n"
"float lighting = abs(dot(transformedNormal, vec3(0,0,-1)));\n"
"outputColor = vec4(color * lighting, 1.0f);\n" //constant white
"}";


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


static GLuint create_shader(int type, const char *src) {

  auto shader = glCreateShader(type);
  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);

  int status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

  if(status == GL_FALSE){
    int log_len;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);

    std::string log_space(log_len+1, ' ');
    glGetShaderInfoLog(shader, log_len, nullptr, (GLchar*)log_space.c_str());

    std::cerr << "Compile failure in " <<
      (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<
      " shader: " << log_space << std::endl;

    glDeleteShader(shader);

    return 0;
  }else{
    std::cout<<" Shader Created "<<std::endl;
  }

  return shader;
}


#endif

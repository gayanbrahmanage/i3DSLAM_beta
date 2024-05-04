
#include "gui.h"

void gui::GLArea3D_realize(){

  GLArea3D.make_current();

  try{
    GLArea3D.throw_if_error();
    GLArea3D_init_buffers();
    GLArea3D_init_shaders();
  }

  catch(const Gdk::GLError& gle){
    std::cout << "An error occured making the context current during realize:" << std::endl;
    std::cout << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
  }

}

void gui::GLArea3D_unrealize(){

  GLArea3D.make_current();
  try{

    GLArea3D.throw_if_error();

    //Delete buffers and program
    glDeleteBuffers(1, &Buffer3D);
    glDeleteProgram(Program3D);
  }
  catch(const Gdk::GLError& gle){

    std::cout << "An error occured making the context current during unrealize" << std::endl;
    std::cout << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
  }
}


void gui::GLArea3D_init_shaders(){

  auto vertex = create_shader(GL_VERTEX_SHADER,VERTEX_SOURCE);
  if(vertex == 0){
     Program3D = 0;
     return;
   }

   auto fragment = create_shader(GL_FRAGMENT_SHADER,FRAGMENT_SOURCE);
   if(fragment == 0){
      Program3D = 0;
      return;
    }

    Program3D = glCreateProgram();
    glAttachShader(Program3D, vertex);
    glAttachShader(Program3D, fragment);

    glLinkProgram(Program3D);

    int status;
    glGetProgramiv(Program3D, GL_LINK_STATUS, &status);
    if(status == GL_FALSE)
    {
      int log_len;
      glGetProgramiv(Program3D, GL_INFO_LOG_LENGTH, &log_len);

      std::string log_space(log_len+1, ' ');
      glGetProgramInfoLog(Program3D, log_len, nullptr, (GLchar*)log_space.c_str());

      std::cerr << "Linking failure: " << log_space << std::endl;

      glDeleteProgram(Program3D);
      Program3D = 0;
    }
    else
    {

      Mvp3D = glGetUniformLocation(Program3D, "Mvp3D");

      glDetachShader(Program3D, vertex);
      glDetachShader(Program3D, fragment);
    }

  glDeleteShader(vertex);
  glDeleteShader(fragment);

}

void gui::GLArea3D_init_buffers(){

  glGenVertexArrays(1, &Vao3D);
  glBindVertexArray(Vao3D);
  glGenBuffers(1, &Buffer3D);
  glBindBuffer(GL_ARRAY_BUFFER, Buffer3D);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

}


bool gui::GLArea3D_render(const Glib::RefPtr<Gdk::GLContext>& context){

  try{

    GLArea3D.throw_if_error();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //draw_triangle();
    draw_3DSLAM_logo(Program3D, Vao3D, eyem, param);

    glFlush();

    return true;
  }

  catch(const Gdk::GLError& gle){
    std::cerr << "An error occurred in the render callback of the GLArea" << std::endl;
    std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
    return false;
  }

}

void gui::draw_3DSLAM_logo(){

  GDateTime *date_time;
  date_time = g_date_time_new_now_local();
  m.current_frame = g_date_time_get_microsecond(date_time);
  m.delta_time = g_date_time_difference(date_time, m.last_frame) / 1000;
  m.last_frame = date_time;

  glUseProgram (program);

  m.model = rotate(m.model, (float)m.delta_time/1000, glm::vec3(1,1,0));
  glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, &m.model[0][0]);
  m.position = glm::vec3(0,0,5);
  m.front = glm::vec3(0,0,-1);
  m.up = glm::vec3(0,1,0);
  glm::mat4 view = glm::lookAt(m.position, m.position + m.front, m.up);
  glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, &view[0][0]);
  glm::mat4 projection = glm::perspective(45.0, double(param->i3D_width)/double(param->i3D_height), 0.1, 100.0);
  glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, &projection[0][0]);

  glBindVertexArray(Vao3D);
  /* Use the vertices in our buffer */

  /* Draw the three vertices as a triangle */
  glDrawArrays (GL_TRIANGLES, 0, 36);

  /* We finished using the buffers and program */
  glBindVertexArray(0);
  glDisableVertexAttribArray (0);
  glBindBuffer (GL_ARRAY_BUFFER, 0);
  glUseProgram (0);
}

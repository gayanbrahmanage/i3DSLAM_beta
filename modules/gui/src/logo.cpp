#include "logo.h"

logo::logo(){

}

logo::~logo(){

}

void logo::read_image(){

  cv::Mat image = cv::imread("../modules/gui/extra/camera.jpg");

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  // set the texture wrapping/filtering options (on the currently bound texture object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load and generate the texture
  int width=image.cols;
  int height=image.rows;
  int nrChannels=image.channels();

  unsigned char *data = image.data;

  if (data){
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
  }
  else{

      std::cout << "Failed to load texture" << std::endl;
  }

}


void logo::init(){

  read_image();

  /* We only use one VAO, so we always keep it bound */
  glGenVertexArrays (1, &Vao);
  glBindVertexArray (Vao);

  /* This is the buffer that holds the vertices */
  glGenBuffers (1, &Buffer);
  glBindBuffer (GL_ARRAY_BUFFER, Buffer);
  glBufferData(GL_ARRAY_BUFFER,sizeof(vertex_data),vertex_data,GL_STATIC_DRAW);
  glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray (0);
  glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray (1);
  glBindBuffer (GL_ARRAY_BUFFER, 0);

  last_frame=g_date_time_new_now_local();
}

void logo::draw(GLuint Program3D){

  GDateTime *date_time;
  date_time = g_date_time_new_now_local();
  current_frame = g_date_time_get_microsecond(date_time);
  delta_time = g_date_time_difference(date_time, last_frame) / 1000;
  last_frame = date_time;

  glUseProgram (Program3D);

  model = glm::rotate(model, (float)delta_time/1000, glm::vec3(1,1,0));
  glUniformMatrix4fv(glGetUniformLocation(Program3D, "model"), 1, GL_FALSE, &model[0][0]);
  glm::vec3 position = glm::vec3(0,0,5);
  glm::vec3 front = glm::vec3(0,0,-1);
  glm::vec3 up = glm::vec3(0,1,0);
  glm::mat4 view = glm::lookAt(position, position + front, up);
  glUniformMatrix4fv(glGetUniformLocation(Program3D, "view"), 1, GL_FALSE, &view[0][0]);
  glm::mat4 projection = glm::perspective(45.0, double(param->i3D_width)/double(param->i3D_height), 0.1, 100.0);
  glUniformMatrix4fv(glGetUniformLocation(Program3D, "projection"), 1, GL_FALSE, &projection[0][0]);

  glBindVertexArray(Vao);
  glDrawArrays (GL_TRIANGLES, 0, 36);

  glBindVertexArray(0);
  glDisableVertexAttribArray (0);
  glBindBuffer (GL_ARRAY_BUFFER, 0);
  glUseProgram (0);

}

void logo::reset(){
  glDeleteBuffers (1, &Buffer);
}

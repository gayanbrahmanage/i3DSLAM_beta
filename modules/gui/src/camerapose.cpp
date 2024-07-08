#include "camerapose.h"

camerapose::camerapose(){

}

camerapose::~camerapose(){

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

}

void camerapose::init(){

  /* We only use one VAO, so we always keep it bound */
  glGenVertexArrays (1, &VAO);
  glBindVertexArray (VAO);

  glGenBuffers (1, &VBO);
  glBindBuffer (GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER,sizeof(vertex_data_cam),vertex_data_cam,GL_STATIC_DRAW);

  glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray (0);

  glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray (1);

  glBindBuffer (GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

}

glm::mat4 camerapose::Eigen4x4ToGLMmat4(Eigen::Matrix4d eigenMatrix){

  return glm::make_mat4(eigenMatrix.data());

}

void camerapose::set_pose(glm::mat4 tf){

  model=tf*Eigen4x4ToGLMmat4(rotateMatrix(pose, 0, 180, 180));

}


void camerapose::draw(GLuint Program3D, glm::mat4 tf){

  set_pose(tf);

  glUniformMatrix4fv(glGetUniformLocation(Program3D, "model"), 1, GL_FALSE, &model[0][0]);

  glBindVertexArray(VAO);
  glDrawArrays (GL_TRIANGLES, 0, 36);

  glBindVertexArray(0);
  // glDisableVertexAttribArray (0);
  // glBindBuffer (GL_ARRAY_BUFFER, 0);
  // glUseProgram (0);


}

void camerapose::reset(){

}

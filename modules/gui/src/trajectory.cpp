#include "trajectory.h"

trajectory::trajectory(){

}

trajectory::~trajectory(){

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

}

void trajectory::init(){


  /* We only use one VAO, so we always keep it bound */
  glGenVertexArrays (1, &VAO);
  glBindVertexArray (VAO);

  glGenBuffers (1, &VBO);
  glBindBuffer (GL_ARRAY_BUFFER, VBO);

  //glBufferData(GL_ARRAY_BUFFER,sizeof(vertex_data_point),vertex_data_point,GL_STATIC_DRAW);

  glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray (0);

  glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray (1);

  glBindBuffer (GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);



}

glm::mat4 trajectory::Eigen4x4ToGLMmat4(Eigen::Matrix4d eigenMatrix){

  return glm::make_mat4(eigenMatrix.data());

}

void trajectory::set_pose(glm::mat4 tf){

  model = glm::mat4(1.0);
  model=tf*model;

}

void trajectory::update_trajectory(message* msg){

  vertices.clear();

  for(int i=0; i<msg->traj.size(); i++){

      Eigen::Matrix4d pose=msg->traj.read_element(i);
      pose = rotateMatrix(pose, 0, 180, 180);

      vertices.push_back(pose(0,3));
      vertices.push_back(pose(1,3));
      vertices.push_back(pose(2,3));

      vertices.push_back(0.0f);
      vertices.push_back(1.0f);
      vertices.push_back(0.0f);

    }

}


void trajectory::draw(GLuint Program3D, glm::mat4 tf, message* msg){

  update_trajectory(msg);
  set_pose(tf);

  glUniformMatrix4fv(glGetUniformLocation(Program3D, "model"), 1, GL_FALSE, &model[0][0]);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_DYNAMIC_DRAW);

  glLineWidth(linewidth); // Set line width to 5 pixels
  glDrawArrays(GL_LINE_STRIP, 0, vertices.size() / 6);

  glBindVertexArray(0);
  // glDisableVertexAttribArray (0);
  // glBindBuffer (GL_ARRAY_BUFFER, 0);
  // glUseProgram (0);


}

void trajectory::reset(){

}

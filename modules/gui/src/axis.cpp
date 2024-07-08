#include "axis.h"

axis::axis(){

}

axis::~axis(){

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

}




void axis::init(){

  float vertices[24] = {
      // positions         // colors
       0.0f,  0.0f, 0.0f,  1.0f, 1.0f, 1.0f,  // 0,0,0
       size,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  // x
       0.0f,  size, 0.0f,  1.0f, 1.0f, 0.0f,  // y
       0.0f,  0.0f, size,  0.0f, 0.0f, 1.0f   // z
  };

  unsigned int indices[6] = {  // note that we start from 0!
      0, 1,
      0, 2,
      0, 3
  };


  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // 3. copy our index array in a element buffer for OpenGL to use
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  glBindVertexArray(0);

}

void axis::set_pose(glm::mat4 tf){

  model = glm::mat4(1.0);
  model = glm::rotate(model, glm::radians(30.f), glm::vec3(1,0,0));
  model=tf*model;

}

void axis::draw(GLuint Program3D, glm::mat4 tf){

  set_pose(tf);
  glUniformMatrix4fv(glGetUniformLocation(Program3D, "model"), 1, GL_FALSE, &model[0][0]);

  glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
  glLineWidth(linewidth); // Set line width to 5 pixels
  glDrawElements(GL_LINE_STRIP, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

}

void axis::reset(){

}

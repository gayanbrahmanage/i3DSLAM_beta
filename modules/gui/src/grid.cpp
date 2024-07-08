#include <grid.h>

grid::grid(){

}

grid::~grid(){

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}


void grid::init(){

  float vertices[12*n*2]={0.0f};
  int grid_space= grid_size/grid_units;

  unsigned int indices[3*n*2]={0};

  for (int i=0; i<n; i++){

    vertices[12*i]=i*grid_space-grid_space*n/2;
    vertices[12*i+1]=grid_space*n/2;
    vertices[12*i+2]=0;

    vertices[12*i+3]=0.5;
    vertices[12*i+4]=0.5;
    vertices[12*i+5]=0.5;

    vertices[12*i+6]=i*grid_space-grid_space*n/2;
    vertices[12*i+7]=-grid_space*n/2;
    vertices[12*i+8]=0;

    vertices[12*i+9]=0.5;
    vertices[12*i+10]=0.5;
    vertices[12*i+11]=0.5;

    indices[3*i]  =2*i;
    indices[3*i+1]=2*i+1;
    indices[3*i+2]=0xFFFF;

  }

  for (int i=n; i<2*n; i++){

    vertices[12*i]=grid_space*n/2;
    vertices[12*i+1]=i*grid_space-grid_space*n*3/2;
    vertices[12*i+2]=0;

    vertices[12*i+3]=0.5;
    vertices[12*i+4]=0.5;
    vertices[12*i+5]=0.5;

    vertices[12*i+6]=-grid_space*n/2;
    vertices[12*i+7]=i*grid_space-grid_space*n*3/2;
    vertices[12*i+8]=0;

    vertices[12*i+9]=0.5;
    vertices[12*i+10]=0.5;
    vertices[12*i+11]=0.5;

    indices[3*i]  =2*i;
    indices[3*i+1]=2*i+1;
    indices[3*i+2]=0xFFFF;

  }

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

void grid::set_pose(glm::mat4 tf){

  model = glm::mat4(1.0);
  model = glm::rotate(model, glm::radians(90.f), glm::vec3(1,0,0));
  model=tf*model;

}


void grid::draw(GLuint Program3D, glm::mat4 tf){

  set_pose(tf);
  glUniformMatrix4fv(glGetUniformLocation(Program3D, "model"), 1, GL_FALSE, &model[0][0]);

  glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(0xFFFF);
  glLineWidth(linewidth); // Set line width to  pixels
  glDrawElements(GL_LINE_STRIP, 3*n*2, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

}

void grid::reset(){

}

#include<axes.h>
axes::axes(){

}

axes::~axes(){
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  glDeleteVertexArrays(1, &gVAO);
  glDeleteBuffers(1, &gVBO);

}

void axes::init_axes(float size){
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


void axes::draw_axes(){
  glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
  glDrawElements(GL_LINE_STRIP, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void axes::init_grid(float size, float space, std::string plane){

  grid_size=size;
  grid_space=space;
  grid_plane=plane;

  float vertices[12]={0};

  glGenVertexArrays(1, &gVAO);
  glGenBuffers(1, &gVBO);

  glBindVertexArray(gVAO);

  glBindBuffer(GL_ARRAY_BUFFER, gVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)/* n points */, vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6/* number of atrributes per point*/ * sizeof(float), (void*)0);
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


void axes::draw_grid(){

  float vertices[12]={0.0f};
  int steps= grid_size/grid_space;
  grid_size=grid_space*steps;
  for (float i=-grid_size/2; i<=grid_size/2; i += grid_space){

    if(grid_plane=="grid_xy"){
      vertices[0]=i;
      vertices[1]=grid_size/2;
      vertices[2]=0;

      vertices[6]=i;
      vertices[7]=-grid_size/2;
      vertices[8]=0;

    }else if(grid_plane=="grid_xz"){
      vertices[0]=i;
      vertices[1]=0;
      vertices[2]=grid_size/2;

      vertices[6]=i;
      vertices[7]=0;
      vertices[8]=-grid_size/2;

    }else if(grid_plane=="grid_yz"){
      vertices[0]=0;
      vertices[1]=i;
      vertices[2]=grid_size/2;

      vertices[6]=0;
      vertices[7]=i;
      vertices[8]=-grid_size/2;
    }

    vertices[3]=viz_param->grid_color_mono;
    vertices[4]=viz_param->grid_color_mono;
    vertices[5]=viz_param->grid_color_mono;

    vertices[9]=viz_param->grid_color_mono;
    vertices[10]=viz_param->grid_color_mono;
    vertices[11]=viz_param->grid_color_mono;

    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) /* n points */, vertices, GL_STATIC_DRAW);

    glBindVertexArray(gVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_LINE_STRIP, 0, 2);

    glBindVertexArray(0);

    if(grid_plane=="grid_xy"){
      vertices[0]=grid_size/2;
      vertices[1]=i;
      vertices[2]=0;

      vertices[6]=-grid_size/2;
      vertices[7]=i;
      vertices[8]=0;

    }else if(grid_plane=="grid_xz"){
      vertices[0]=grid_size/2;
      vertices[1]=0;
      vertices[2]=i;

      vertices[6]=-grid_size/2;
      vertices[7]=0;
      vertices[8]=i;

    }else if(grid_plane=="grid_yz"){
      vertices[0]=0;
      vertices[1]=grid_size/2;
      vertices[2]=i;

      vertices[6]=0;
      vertices[7]=-grid_size/2;
      vertices[8]=i;

    }

    vertices[3]=viz_param->grid_color_mono;
    vertices[4]=viz_param->grid_color_mono;
    vertices[5]=viz_param->grid_color_mono;

    vertices[9]=viz_param->grid_color_mono;
    vertices[10]=viz_param->grid_color_mono;
    vertices[11]=viz_param->grid_color_mono;

    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) /* n points */, vertices, GL_STATIC_DRAW);

    glBindVertexArray(gVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_LINE_STRIP, 0, 2);

    glBindVertexArray(0);

  }

}

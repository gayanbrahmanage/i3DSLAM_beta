#include "viz.h"
#include "shader.h"

///Get euler angles from affine matrix (helper for isBigTrafo)
Eigen::Vector3d tf_to_RPY(const Eigen::Matrix4d& t) {

    double roll = atan2(t(2,1),t(2,2));
    double pitch = atan2(-t(2,0),sqrt(t(2,1)*t(2,1)+t(2,2)*t(2,2)));
    double yaw = atan2(t(1,0),t(0,0));

    Eigen::Vector3d  rpy(roll, pitch, yaw);
    return rpy;

}

//model
float axis_rotation_angle=0.0f;
float X_axis_rotation_angle=0.0f, Y_axis_rotation_angle=0.0f;
glm::vec3 roation_axis =glm::vec3(0.0f, 1.0f, 0.0f);

// camera

glm::vec3 cameraPos   = glm::vec3(0.0f, 3.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, -1.0f, 0.0f);
glm::vec3 cameraUp    = glm::vec3(1.0f, 0.0f, 0.0f);

// glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
// glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
// glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse_left = true, firstMouse_middle = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  640.0f / 2.0;
float lastY =  480.0 / 2.0;
float fov   =  30.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void CursorPosCallback(GLFWwindow* window, double xpos, double ypos){

  if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)){
    if (firstMouse_left){
      lastX = xpos;
      lastY = ypos;
      firstMouse_left = false;
    }

  //if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)){
    //std::cout<<glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)<<std::endl;
    //std::cout<<xpos<<" "<<ypos<<std::endl;
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
  //}

  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  cameraFront = glm::normalize(front);
} else{
  firstMouse_left=true;
}

  if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE)){
    if (firstMouse_middle){
      lastX = xpos;
      lastY = ypos;
      firstMouse_middle = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.05f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    if(abs(xoffset)>abs(yoffset)){
      axis_rotation_angle+=xoffset;
    } else{
      axis_rotation_angle+=yoffset;
    }

    X_axis_rotation_angle+=xoffset;
    Y_axis_rotation_angle+=yoffset;

    float x_direction=abs(Y_axis_rotation_angle)/(abs(X_axis_rotation_angle)+abs(Y_axis_rotation_angle));
    float y_direction=abs(X_axis_rotation_angle)/(abs(X_axis_rotation_angle)+abs(Y_axis_rotation_angle));
    float z_direction=std::min(abs(X_axis_rotation_angle),abs(Y_axis_rotation_angle))/(abs(X_axis_rotation_angle)+abs(Y_axis_rotation_angle));

    //std::cout<<xoffset << " "<< yoffset<<std::endl;
    roation_axis =glm::vec3(x_direction, y_direction, z_direction);

  }else{
    firstMouse_middle=true;
  }

}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
  fov -= (float)yoffset;
  if (fov < 1.0f)
      fov = 1.0f;
  if (fov > 45.0f)
      fov = 45.0f;
}

void processInput(GLFWwindow *window){
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
  float cameraSpeed = 2.5 * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      cameraPos += cameraSpeed*100 * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      cameraPos -= cameraSpeed*100 * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
      cameraPos -= glm::normalize(cameraUp) * cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
      cameraPos += glm::normalize(cameraUp) * cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
    cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

    X_axis_rotation_angle=0.0f;
    Y_axis_rotation_angle=0.0f;
    axis_rotation_angle=0.0f;
    fov=45.0f;

  }
  if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS){
    cameraPos   = glm::vec3(3.0f, 0.0f, 0.0f);
    cameraFront = glm::vec3(-1.0f, 0.0f, 0.0f);
    cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
  }
  if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS){
    cameraPos   = glm::vec3(0.0f, 0.0f, -3.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
    cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
  }
  if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS){
    cameraPos   = glm::vec3(-3.0f, 0.0f, 0.0f);
    cameraFront = glm::vec3(1.0f, 0.0f, 0.0f);
    cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
  }
  if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS){
    cameraPos   = glm::vec3(0.0f, 3.0f, 0.0f);
    cameraFront = glm::vec3(0.0f, -1.0f, 0.0f);
    cameraUp    = glm::vec3(1.0f, 0.0f, 0.0f);
  }
  if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS){
    cameraPos   = glm::vec3(0.0f, -3.0f, 0.0f);
    cameraFront = glm::vec3(0.0f, 1.0f, 0.0f);
    cameraUp    = glm::vec3(1.0f, 0.0f, 0.0f);
  }
}

/*
callback Functions
*/
static void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

// viz class members
viz::viz(){
  std::cout << "OpenGL Application Object is Created" << std::endl;
}

viz::~viz(){
  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  ax.~axes();
  glDeleteProgram(shaderProgram);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
}

void viz::init(std::string pose_type, float pose_scale, bool _grid_on, std::string grid_plane){

  lastX =  viz_param->width / 2.0;
  lastY =  viz_param->height/ 2.0;

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(viz_param->width, viz_param->height, "3DSLAM", NULL, NULL);
  if (window == NULL){
   std::cout << "Failed to create GLFW window" << std::endl;
   glfwTerminate();
  }
  glfwMakeContextCurrent(window);

  //calback functions
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, CursorPosCallback);
  glfwSetScrollCallback(window, scroll_callback);

  /*
  GLAD manages function pointers for OpenGL so we want to initialize
  GLAD before we call any OpenGL function:
  */
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    std::cout << "Failed to initialize GLAD" << std::endl;
  }

  // vertex shader
  // build and compile our shader program
  // ------------------------------------
  ourShader=Shader("../modules/3Dviz_SLAM/shader/shader.vs", "../modules/3Dviz_SLAM/shader/shader.fs"); // you can name your shader files however you like

  // draw axes
  ax.init_axes(viz_param->axis_size);

  if(_grid_on){
    ax.init_grid(viz_param->grid_size, viz_param->grid_space, grid_plane);
    grid_on=true;
  }

   std::vector<float> marker_color_pose={0.0f,1.0f,0.0f};
   std::vector<float> marker_color_gt={1.0f,1.0f,1.0f};
  _pose.init_pose(pose_scale, pose_type,marker_color_pose);
  _pose2.init_pose(pose_scale, "line",marker_color_pose);
  _pose_gt.init_pose(pose_scale, "line",marker_color_gt);

  //draw grid

}


void viz::draw(message* msg){

  if(!glfwWindowShouldClose(window)){

      // per-frame time logic
      // --------------------
      float currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;

      //We then call processInput every iteration of the render loop:
      processInput(window);
      /* At the start of frame we want to clear the screen*/
      glClearColor(viz_param->bg_color_R, viz_param->bg_color_G, viz_param->bg_color_B, 0.5f);
      glEnable(GL_DEPTH_TEST);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



      // render here
      /*
      ourShader.use();
      // create transformations
      glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
      glm::mat4 view          = glm::mat4(1.0f);
      glm::mat4 projection    = glm::mat4(1.0f);
      model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
      //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
      view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));
      projection = glm::perspective(glm::radians(45.0f), (float)viz_param->width / (float)viz_param->height, 0.1f, 100.0f);
      // retrieve the matrix uniform locations
      unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
      unsigned int viewLoc  = glGetUniformLocation(ourShader.ID, "view");
      // pass them to the shaders (3 different ways)
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
      glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
      // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
      ourShader.setMat4("projection", projection);

      */
      // activate shader
      ourShader.use();

      glm::mat4 projection = glm::perspective(glm::radians(fov), (float)viz_param->width / (float)viz_param->height, 0.1f, 100.0f);
      ourShader.setMat4("projection", projection);

      // camera/view transformation
      glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
      float radius = 5.0f;
      float camX   = sin(glfwGetTime()) * radius;
      float camZ   = cos(glfwGetTime()) * radius;
      //view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
      view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
      ourShader.setMat4("view", view);
      glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first


      //model[2][3]=current_pose(0,3);
      // model[1][3]=current_pose(1,3);
      //model[0][3]=current_pose(0,3);

      //Eigen::Vector3d RPY=tf_to_RPY(current_pose);
      //axis_rotation_angle=(float)glfwGetTime() * glm::radians(50.0f);
      // axis_rotation_angle=glm::radians(RPY(1)*180/M_PI);
      // roation_axis=glm::vec3(1.0f, 0.0f, 0.0f);
      model = glm::rotate(model, axis_rotation_angle, roation_axis);
      //
      // axis_rotation_angle=glm::radians(RPY(0)*180/M_PI);
      // roation_axis=glm::vec3(0.0f, 0.0f, 1.0f);
      // model = glm::rotate(model, axis_rotation_angle, roation_axis);
      //
      // axis_rotation_angle=RPY(2);
      // roation_axis=glm::vec3(0.0f, 0.0f, 1.0f);
      // model = glm::rotate(model, axis_rotation_angle, roation_axis);

      //scale
      model = glm::scale(model, glm::vec3(0.1f));

      ourShader.setMat4("model", model);

      // glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
      // glDrawElements(GL_LINE_STRIP, 6, GL_UNSIGNED_INT, 0);
      //glDrawArrays(GL_TRIANGLES, 0, 3);
      // glBindVertexArray(0); // no need to unbind it every time

      ax.draw_axes();

      if(grid_on){
        ax.draw_grid();
      }


      _pose.draw_pose(msg->pose);
      _pose2.draw_pose(msg->pose);
      _pose_gt.draw_pose(msg->poseGT);

      glfwSwapBuffers(window);
      glfwPollEvents();
  }

}


void viz::save_window(){
// need more work here
  int outputdata_array_size=3 * viz_param->width * viz_param->height;
  GLubyte* outputdata=(GLubyte*)malloc(3 * viz_param->width * viz_param->height);
  glReadPixels(0, 0, viz_param->width, viz_param->height, GL_BGR, GL_UNSIGNED_BYTE, outputdata);
  if(outputdata){
     // cv::Mat frame;
     // frame.create(viz_param->height,viz_param->width , cv::CV_8UC3);
  //   std::stringstream png_file_color;
  //   png_file_color <<"../out/color"<<gui_loop_count<<".png";
  //   for(int x=0; x<frame.cols; x++){
  //     for(int y=0; y< frame.rows; y++){
  //
  //       int index= x+(param->height-1-y)*frame.cols;
  //       int r= outputdata[3*index];
  //       int g= outputdata[3*index+1];
  //       int b= outputdata[3*index+2];
  //
  //       frame.at<cv::Vec3b>(y,x)[0]=r;
  //       frame.at<cv::Vec3b>(y,x)[1]=g;
  //       frame.at<cv::Vec3b>(y,x)[2]=b;
  //     }
  //   }
  //   cv::imwrite(png_file_color.str().c_str(),frame);
  }

}

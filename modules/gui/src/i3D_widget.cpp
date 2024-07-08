#include "i3D_widget.h"
#include <cairomm/context.h>
#include <giomm/resource.h>
#include <gdkmm/general.h>
#include <glibmm/fileutils.h>
#include <iostream>

 i3D_widget::i3D_widget(){

 }

 i3D_widget::~i3D_widget(){

 }

 void i3D_widget::init(){

   ax.init();
   grd.init();
   cpose.init();
   tr.init();


 }


void i3D_widget::rotate(float angle, glm::vec3 vec ){

   model = glm::rotate(model, glm::radians(angle), vec);

 }

 void i3D_widget::draw(GLuint Program3D, message* msg){

   glUseProgram (Program3D);
   glUniformMatrix4fv(glGetUniformLocation(Program3D, "model"), 1, GL_FALSE, &model[0][0]);

   if(view_selecton==0){

     Eigen::Matrix4d pose=msg->current_pose.read();
     pose = rotateMatrix(pose, 0, 180, 180);

     glm::vec frontm = glm::vec3(pose(0,3),pose(1,3),pose(2,3));
     Eigen::Vector4d vec=pose*cam_location;
     glm::vec positionm = glm::vec3(vec(0),vec(1),vec(2));
     glm::vec upm = glm::vec3(0,1,0);


     glm::mat4 view = glm::lookAt(positionm, frontm, upm);
     glUniformMatrix4fv(glGetUniformLocation(Program3D, "view"), 1, GL_FALSE, &view[0][0]);
     glm::mat4 projection = glm::perspective(45.0, double(param->i3D_width)/double(param->i3D_height), 0.1, 100.0);
     glUniformMatrix4fv(glGetUniformLocation(Program3D, "projection"), 1, GL_FALSE, &projection[0][0]);

   }else if(view_selecton==1){

     glm::mat4 view = glm::lookAt(position, front, up);
     glUniformMatrix4fv(glGetUniformLocation(Program3D, "view"), 1, GL_FALSE, &view[0][0]);
     glm::mat4 projection = glm::perspective(45.0, double(param->i3D_width)/double(param->i3D_height), 0.1, 100.0*position[2]);
     glUniformMatrix4fv(glGetUniformLocation(Program3D, "projection"), 1, GL_FALSE, &projection[0][0]);

   }



   grd.draw(Program3D, model);
   ax.draw(Program3D, model);
   tr.draw(Program3D, model, msg);
   cpose.draw(Program3D, model);



 }

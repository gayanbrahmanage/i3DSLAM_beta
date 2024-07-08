#ifndef i3D_widget_H
#define i3D_widget_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm.h>
#include <epoxy/gl.h>
#include <gtkmm/application.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "parameters.h"
#include "axis.h"
#include "guixs.h"
#include <grid.h>
#include "camerapose.h"
#include "trajectory.h"

class i3D_widget{

   private:
     parameters* param=new parameters();

   public:
     glm::mat4 model = glm::mat4(1.0);
     Eigen::Vector4d  cam_location=Eigen::Vector4d(0,-10,-25,1);

     axis ax;
     pose pse;
     grid grd;
     camerapose cpose;
     trajectory tr;

     glm::vec3 position = glm::vec3(0,0,300);
     glm::vec3 front = glm::vec3(0,0,-10);
     glm::vec3 up = glm::vec3(0,1,0);

     int view_selecton=0;

     i3D_widget();
     virtual ~i3D_widget();

     void init();
     void draw(GLuint Program3D,message* msg);
     void rotate(float angle, glm::vec3 vec );

   protected:


 };

 #endif

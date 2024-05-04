#ifndef i3D_widget_H
#define i3D_widget_H

#include <gtkmm/drawingarea.h>
#include <gdkmm/pixbuf.h>
#include "message.h"
#include "parameters.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader_m.h>
#include <camera.h>
#include "viz_parameters.h"


 class i3D_widget : public Gtk::DrawingArea {

   private:


   public:

     i3D_widget();
     virtual ~i3D_widget();


   protected:


 };

 #endif

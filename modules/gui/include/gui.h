#ifndef gui_H
#define gui_H


#include <thread>
#include "parameters.h"
#include "message.h"
#include "video_widget.h"
#include "guixs.h"




class gui: public Gtk::Window{
  private:
    // Dispatcher handler.
    parameters* param=new parameters();

  public:

    gui();
    virtual ~gui();

  protected:
    Gtk::Box boxMain;
    Gtk::Frame frame_input_window;
    Gtk::Box boxL;
    Gtk::Frame frame_input_source;
    Gtk::HBox box_input_source;
    Gtk::Grid grid_input_source;
    Gtk::Button browse_button;
    void on_browse_button_clicked();
    Gtk::Button camera_button;
    void on_camera_button_clicked();

    Gtk::Frame frame_display_window;
    Gtk::Box boxM;
    Gtk::Frame frame_input_view;
    Gtk::Box box_input_view;
    video_widget video_image;
    Gtk::Frame frame_3D_view;
    Gtk::Box box_3D_view;

    Gtk::GLArea GLArea3D;
    void GLArea3D_realize();
    void GLArea3D_unrealize();
    bool GLArea3D_render(const Glib::RefPtr<Gdk::GLContext>& context);
    void GLArea3D_init_shaders();
    void GLArea3D_init_buffers();
    GLuint Vao3D {0};
    GLuint Buffer3D {0};
    GLuint Program3D {0};
    GLuint Mvp3D {0};
    eye m;
    void draw_3DSLAM_logo();



    Gtk::Frame frame_display_ctrl_window;
    Gtk::Box boxR;



};

#endif

#ifndef gui_H
#define gui_H


#include <thread>
#include "parameters.h"
#include "message.h"
#include "video_widget.h"
#include "logo.h"
#include "shader.h"
#include "i3D_widget.h"


class gui: public Gtk::Window{
  private:
    logo lg;
    i3D_widget i3d;
    // Dispatcher handler.
    void on_mapbuilder_complete();
    void on_ax_axis_value_change(const Glib::RefPtr<Gtk::Adjustment>& adj);
    void on_ay_axis_value_change(const Glib::RefPtr<Gtk::Adjustment>& adj);
    void on_az_axis_value_change(const Glib::RefPtr<Gtk::Adjustment>& adj);
    void on_zoom_axis_value_change(const Glib::RefPtr<Gtk::Adjustment>& adj);

    parameters* param=new parameters();

    static void realize_cb(GtkWidget *widget, gpointer user_data) {
        gui *renderer = static_cast<gui *>(user_data);
        renderer->realize(widget);
    }

   static gboolean render_cb(GtkGLArea *area, GdkGLContext *context, gui *renderer) {
      return renderer->render(area, context);
   }

   static void unrealize_cb(GtkWidget *widget, gui *renderer) {
      renderer->unrealize(widget);
   }

  public:

    message* msg=new message();
    Glib::Dispatcher Dispatcher_mapbuilder;

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

    GtkWidget *GLArea3D;
    GLuint Program3D {0};
    GLuint Mvp3D {0};

    void realize(GtkWidget *widget);
    bool render(GtkGLArea *area, GdkGLContext *context);
    void unrealize(GtkWidget *widget);
    GLuint create_shader(int type);

    Gtk::Frame frame_display_ctrl_window;
    Gtk::Box boxR;
    Gtk::Grid grid_param;
    Glib::RefPtr<Gtk::Adjustment> adj_ax;
    Gtk::Scale ax_Scale;
    Glib::RefPtr<Gtk::Adjustment> adj_ay;
    Gtk::Scale ay_Scale;
    Glib::RefPtr<Gtk::Adjustment> adj_az;
    Gtk::Scale az_Scale;
    Glib::RefPtr<Gtk::Adjustment> adj_zoom;
    Gtk::Scale zoom_Scale;

    Gtk::Separator separator1;
    Gtk::Button reset_view_button;
    Gtk::ToggleButton followcam_togglebtn;
    void on_followCamera_toggle_clicked();


    void on_reset_view_button_clicked();

};


#endif

#include "gui.h"
//CopyRight! Autonomous Systems and Intelligent Sensing Laboratory    video_image("../extra/camera.png")

gui::gui():
    boxMain(Gtk::ORIENTATION_HORIZONTAL,5),
    frame_input_window(""),
    boxL(Gtk::ORIENTATION_VERTICAL),
    frame_input_source("Select Input"),
    box_input_source(Gtk::ORIENTATION_VERTICAL),
    grid_input_source(),
    browse_button("Browse"),
    camera_button("Camera"),

    frame_display_window(""),
    boxM(Gtk::ORIENTATION_VERTICAL), //middle
    frame_input_view("Input RGB"),
    box_input_view(Gtk::ORIENTATION_VERTICAL),
    video_image("../modules/gui/extra/camera.png"),
    frame_3D_view("3D-Map"),
    box_3D_view(Gtk::ORIENTATION_VERTICAL),

    frame_display_ctrl_window("Display Settings"),
    boxR(Gtk::ORIENTATION_VERTICAL) //right
    {
        set_title("3D-SLAM v1.0");
        //set_default_size(1260, 820);
        boxMain.set_border_width(10);
        add(boxMain);

        //left frame
        boxMain.pack_start(frame_input_window);
        boxL.set_border_width(5);
        frame_input_window.add(boxL);
        boxL.pack_start(frame_input_source);
        box_input_source.set_border_width(5);
        frame_input_source.add(box_input_source);
        box_input_source.add(grid_input_source);
        grid_input_source.attach(browse_button, 0, 0, 1, 1);
        grid_input_source.attach(camera_button, 1, 0, 1, 1);

        //middle frame
        boxMain.pack_start(frame_display_window);
        boxM.set_border_width(5);
        frame_display_window.add(boxM);
        boxM.pack_start(frame_input_view);
        box_input_view.set_border_width(5);
        frame_input_view.add(box_input_view);
        box_input_view.pack_start(video_image, Gtk::PACK_SHRINK);
        boxM.pack_start(frame_3D_view);
        box_3D_view.set_border_width(5);
        frame_3D_view.add(box_3D_view);
        GLArea3D.set_hexpand(true);
        GLArea3D.set_size_request(param->i3D_width, param->i3D_height);
        GLArea3D.set_auto_render(true);
        box_3D_view.pack_start(GLArea3D);

        GLArea3D.signal_realize().connect(sigc::mem_fun(*this, &gui::GLArea3D_realize));
        GLArea3D.signal_unrealize().connect(sigc::mem_fun(*this, &gui::GLArea3D_unrealize), false);
        GLArea3D.signal_render().connect(sigc::mem_fun(*this, &gui::GLArea3D_render), false);

        //box_3D_view.pack_start(i3D, Gtk::PACK_SHRINK);

        //right frame
        boxMain.pack_start(frame_display_ctrl_window);
        boxR.set_border_width(5);
        frame_display_ctrl_window.add(boxR);

        show_all_children();
}

gui::~gui(){

}

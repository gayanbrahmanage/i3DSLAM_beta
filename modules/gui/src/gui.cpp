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
    boxR(Gtk::ORIENTATION_VERTICAL), //right
    grid_param(),
    adj_ax( Gtk::Adjustment::create(0.0, 0.0, 360, 0.1, 1.0, 1.0) ),// Value, lower, upper, step_increment, page_increment, page_size:
    ax_Scale(adj_ax),
    adj_ay( Gtk::Adjustment::create(0.0, 0.0, 360, 0.1, 1.0, 1.0) ),// Value, lower, upper, step_increment, page_increment, page_size:
    ay_Scale(adj_ay),
    adj_az( Gtk::Adjustment::create(0.0, 0.0, 360, 0.1, 1.0, 1.0) ),// Value, lower, upper, step_increment, page_increment, page_size:
    az_Scale(adj_az),
    adj_zoom( Gtk::Adjustment::create(300.0, 0.0, 1000, 0.1, 1.0, 1.0) ),// Value, lower, upper, step_increment, page_increment, page_size:
    zoom_Scale(adj_zoom),
    reset_view_button("Reset")
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

        GLArea3D = gtk_gl_area_new ();
        gtk_widget_set_size_request(GLArea3D, param->i3D_width, param->i3D_height); // Set size here
        box_3D_view.add(*Glib::wrap(GLArea3D));

        g_signal_connect(GLArea3D, "realize", G_CALLBACK(realize_cb), this);
        g_signal_connect(GLArea3D, "render", G_CALLBACK(render_cb), this);
        g_signal_connect(GLArea3D, "unrealize", G_CALLBACK(unrealize_cb), this);


        //right frame
        boxMain.pack_start(frame_display_ctrl_window);
        boxR.set_border_width(5);
        boxR.set_size_request(300, -1);
        grid_param.set_row_spacing(5);
        grid_param.set_column_spacing(5);
        frame_display_ctrl_window.add(boxR);
        boxR.add(grid_param);

        grid_param.attach(*Gtk::make_managed<Gtk::Label>("Roll", 0),0,0,1,1);
        ax_Scale.set_digits(0);
        ax_Scale.set_hexpand(true);
        grid_param.attach(ax_Scale,1,0,1,1);
        adj_ax->signal_value_changed().connect(sigc::bind(sigc::mem_fun(*this, &gui::on_ax_axis_value_change), adj_ax));

        grid_param.attach(*Gtk::make_managed<Gtk::Label>("Pitch", 0),0,1,1,1);
        ay_Scale.set_digits(0);
        ay_Scale.set_hexpand(true);
        grid_param.attach(ay_Scale,1,1,1,1);
        adj_ay->signal_value_changed().connect(sigc::bind(sigc::mem_fun(*this, &gui::on_ay_axis_value_change), adj_ay));

        grid_param.attach(*Gtk::make_managed<Gtk::Label>("Yaw", 0),0,2,1,1);
        az_Scale.set_digits(0);
        az_Scale.set_hexpand(true);
        grid_param.attach(az_Scale,1,2,1,1);
        adj_az->signal_value_changed().connect(sigc::bind(sigc::mem_fun(*this, &gui::on_az_axis_value_change), adj_az));

        grid_param.attach(*Gtk::make_managed<Gtk::Label>("Zoom", 0),0,3,1,1);
        zoom_Scale.set_digits(0);
        zoom_Scale.set_hexpand(true);
        grid_param.attach(zoom_Scale,1,3,1,1);
        adj_zoom->signal_value_changed().connect(sigc::bind(sigc::mem_fun(*this, &gui::on_zoom_axis_value_change), adj_zoom));

        grid_param.attach(separator1, 0,4,4,1);

        grid_param.attach(*Gtk::make_managed<Gtk::Label>("Select View ", 0),1,5,1,1);
        followcam_togglebtn.set_size_request(100, 30);
        followcam_togglebtn.set_label("Cam"); // Initial label
        followcam_togglebtn.signal_clicked().connect(sigc::mem_fun(*this, &gui::on_followCamera_toggle_clicked));
        grid_param.attach(followcam_togglebtn, 0, 5, 1, 1);

        grid_param.attach(reset_view_button, 0, 6, 1, 1);
        reset_view_button.signal_clicked().connect(sigc::mem_fun(*this,&gui::on_reset_view_button_clicked) );

        Dispatcher_mapbuilder.connect(sigc::mem_fun(*this, &gui::on_mapbuilder_complete));

        show_all_children();
}

gui::~gui(){

}

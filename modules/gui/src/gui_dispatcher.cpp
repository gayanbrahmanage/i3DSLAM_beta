#include "gui.h"


void gui::on_mapbuilder_complete(){
  //std::cout<<"on_mapbuilder_complete complete"<<std::endl;
  cv::Mat image=msg->output_image.read();

  if(!image.empty()){
    video_image.update(image);
  }

  //std::cout<<"on_mapbuilder_complete complete "<< msg->pose. <<std::endl;
  i3d.cpose.pose=msg->current_pose.read();

}


void gui::on_ax_axis_value_change(const Glib::RefPtr<Gtk::Adjustment>& adj){

  i3d.model = glm::mat4(1.0);
  i3d.pse.ax=adj->get_value();

  i3d.rotate(i3d.pse.ax, glm::vec3(1,0,0) );
  i3d.rotate(i3d.pse.ay, glm::vec3(0,1,0) );
  i3d.rotate(i3d.pse.az, glm::vec3(0,0,1) );

}

void gui::on_ay_axis_value_change(const Glib::RefPtr<Gtk::Adjustment>& adj){

  i3d.model = glm::mat4(1.0);
  i3d.pse.ay=adj->get_value();

  i3d.rotate(i3d.pse.ax, glm::vec3(1,0,0) );
  i3d.rotate(i3d.pse.ay, glm::vec3(0,1,0) );
  i3d.rotate(i3d.pse.az, glm::vec3(0,0,1) );


}

void gui::on_az_axis_value_change(const Glib::RefPtr<Gtk::Adjustment>& adj){

  i3d.model = glm::mat4(1.0);
  i3d.pse.az=adj->get_value();

  i3d.rotate(i3d.pse.ax, glm::vec3(1,0,0) );
  i3d.rotate(i3d.pse.ay, glm::vec3(0,1,0) );
  i3d.rotate(i3d.pse.az, glm::vec3(0,0,1) );

}

void gui::on_zoom_axis_value_change(const Glib::RefPtr<Gtk::Adjustment>& adj){

  i3d.position = glm::vec3(0,0,adj->get_value());

}

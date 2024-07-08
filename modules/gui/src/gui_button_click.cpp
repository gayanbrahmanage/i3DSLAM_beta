#include "gui.h"

void gui::on_reset_view_button_clicked(){

  i3d.model = glm::mat4(1.0);
  i3d.pse.ax=30;
  i3d.pse.ay=0;
  i3d.pse.az=0;
  adj_ax->set_value(30);
  adj_ay->set_value(0);
  adj_az->set_value(0);
  i3d.position = glm::vec3(0,0,300);
  adj_zoom->set_value(300);

}

void gui::on_followCamera_toggle_clicked(){

  if (followcam_togglebtn.get_active()){

      followcam_togglebtn.set_label("World");
      i3d.view_selecton=1;

  }else{
      on_reset_view_button_clicked();
      i3d.view_selecton=0;
      followcam_togglebtn.set_label("Cam");
  }
}

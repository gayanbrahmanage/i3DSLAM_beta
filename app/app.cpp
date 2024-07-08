#include <thread>
#include "map_builder.h"
#include "data_loader.h"
#include "gui.h"

int main(int argc, char* argv[]){

  int data_set_number=7;


  auto app = Gtk::Application::create(argc, argv, "GayanBrahmanage");
  gui SLAMgui;

  if(argc>1){
    data_set_number=atoi(argv[1]);
  }

  kitti* dataloader=new kitti(data_set_number,SLAMgui.msg);
  std::thread dataloader_thread(&kitti::run,dataloader,SLAMgui.msg);

  MapBuilder* map_builder=new MapBuilder();
  std::thread MapBuilder_thread(&MapBuilder::run,map_builder,SLAMgui.msg, &SLAMgui);

  app->run(SLAMgui);

  MapBuilder_thread.join();
  dataloader_thread.join();

  return 0;

 }

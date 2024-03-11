#include <thread>
#include "map_builder.h"
#include "data_loader.h"
#include "viz.h"

int main(int argc, char* argv[]){

  viz GUI;
  GUI.init("cam" /*Pose type */,
            0.02f /*pose scale*/,
            true/*grid on*/,
            "grid_xz");


  int data_set_number=0;
  message* msg=new message();
  msg->init();

  if(argc>1){
    data_set_number=atoi(argv[1]);
  }

  kitti* dataloader=new kitti(data_set_number,msg);
  std::thread dataloader_thread(&kitti::run,dataloader,msg);

  MapBuilder* map_builder=new MapBuilder();
  std::thread MapBuilder_thread(&MapBuilder::run,map_builder,msg);

  while(true){

    for(std::map<int,Node*>::iterator it=map_builder->graph_nodes.begin(); it!=map_builder->graph_nodes.end(); ++it){
      msg->pose[it->first]=it->second->pose.read();
    }

    GUI.draw(msg);

  }

  MapBuilder_thread.join();
  GUI.~viz();
  return 0;

 }

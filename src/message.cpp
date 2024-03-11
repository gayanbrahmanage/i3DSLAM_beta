#include "message.h"

message::message(){

}

message::~message(){

}

void message::init(){

  frame_number.write(0);
  end_flag.write(false);
  new_frame_flag.write(false);
  draw_kpts.write(true);
  draw_matches.write(true);

}

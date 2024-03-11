#include <data_loader.h>


kitti::kitti(int i,message* msg){
  sq_no=i;
  read_time_stamp();
  read_gt(msg);
}

kitti::~kitti(){

}

void kitti::run(message* msg){
  while(!msg->end_flag.read()) {

    if(!msg->new_frame_flag.read()){

      double fps=1000.0f/tr.stop();
      msg->fps.write(fps);

      std::cout<<"FPS "<< fps << " F/Seconds"<<std::endl;
      tr.start();
      read_frame(msg);

    }
  }
}

void kitti::read_frame(message* msg){

  char target[7];
  sprintf(target, "%06d", frame_number);
  std::stringstream color_frame_left_png,color_frame_right_png, depth_frame;
  //read left image
  color_frame_left_png << data_folder <<"kitti"<< sq_no<<"_data/"<<sq_no<<"/image_0/"<<target<<".png";
  cv::Mat color_frame_left = cv::imread(color_frame_left_png.str());
  //read right image
  color_frame_right_png << data_folder <<"kitti"<< sq_no<<"_data/"<<sq_no<<"/image_1/"<<target<<".png";
  cv::Mat color_frame_right = cv::imread(color_frame_right_png.str());
  // depth

  depth_frame << data_folder <<"kitti"<< sq_no<<"_data/"<<sq_no<<"/pdepth/"<<target<<".png";
  cv::Mat depth = cv::imread(depth_frame.str());

  if(color_frame_left.empty()||color_frame_right.empty()){
    std::cout<< "Image Not Found "<<color_frame_left_png.str()<< " or "<< std::endl;
    std::cout<< "Image Not Found "<<color_frame_right_png.str()<< std::endl;
    msg->end_flag.write(true);

  }else{
    msg->image1.write(color_frame_left.clone());
    msg->image2.write(color_frame_right.clone());
    msg->image3=depth.clone();

    msg->frame_number.write(frame_number);
    msg->time_stamp.write(times[frame_number]);
    msg->new_frame_flag.write(true);
    //msg->poseGT[frame_number].write(gt[frame_number]);
    msg->image_w.write(color_frame_left.cols);
    msg->image_h.write(color_frame_left.rows);

    if(frame_number==0){

      int n_x=(color_frame_left.cols)/param.grid_x+1;
      int n_y=(color_frame_left.rows)/param.grid_y+1;
      int n_blocks=n_x*n_y;
      msg->n_blocks.write(n_blocks);
    }

    std::cout<<"Frame no: "<<frame_number<<std::endl;
    frame_number++;
  }

  //if(frame_number>2)msg->end_flag.write(true);
}

void kitti::read_time_stamp(){
  // read time file
  // store in a vector
  std::stringstream time_txt;
  time_txt << data_folder <<"kitti"<< sq_no<<"_data/"<<sq_no<<"/times.txt";
  std::ifstream time_file (time_txt.str());
  if (time_file.is_open()){
    std::cout<<"\033[21;35mStatus Message:: \033[0m  "<<"Time File Loaded : "<<time_txt.str()<<std::endl;
    std::string line;
    while ( getline (time_file,line)){
      float t;;
      std::istringstream os(line);
      os>>t;
      times.push_back(t);
    }
  }else{
    std::cout<<"\033[21;33mWARNING:: \033[0m  "<<" Time file opening FAILD"<<std::endl;
  }
}

void kitti::read_gt(message* msg){

  Eigen::Matrix4d pose_init=Eigen::Matrix4d::Identity();
  // read the file
  std::stringstream gt_file_name;
  gt_file_name << data_folder <<"kitti"<< sq_no<<"_data/"<<sq_no<<"/gt.txt";
  std::ifstream gt_file (gt_file_name.str());

  int i=-1;
  if (gt_file.is_open()){
    std::string line;
    while ( getline (gt_file,line)){
      i++;
      std::stringstream ss(line);
      std::vector<std::string> str_list;

      Eigen::Matrix4d p=Eigen::Matrix4d::Identity();
      while (ss){
        std::string s;
        if (!getline( ss, s, ' ' )) break;
        str_list.push_back( s );
      }
      p(0,0)=stof(str_list[0]);
      p(0,1)=stof(str_list[1]);
      p(0,2)=stof(str_list[2]);
      p(0,3)=stof(str_list[3]);
      p(1,0)=stof(str_list[4]);
      p(1,1)=stof(str_list[5]);
      p(1,2)=stof(str_list[6]);
      p(1,3)=stof(str_list[7]);
      p(2,0)=stof(str_list[8]);
      p(2,1)=stof(str_list[9]);
      p(2,2)=stof(str_list[10]);
      p(2,3)=stof(str_list[11]);

      if(i==start_frame_number)pose_init=p;

    Eigen::Matrix4d pose=pose_init.inverse()*p;
    msg->poseGT[i]=pose;

    }
  }
  std::cout<<"\033[21;35mStatus Message:: \033[0m  "<<"Ground Truth  File Loaded : "<<gt_file_name.str()<<std::endl;
}

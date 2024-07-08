#include <utils.h>

time_record::time_record(){
  time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
time_record::~time_record(){

}

void time_record::start(){
  time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

unsigned int time_record::stop(){
  unsigned int dt=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()-time;
  return dt;
}

void create_output_image(message* msg, Node* node, SLAM_Match_Vector& Matches){

  cv::Mat image=msg->image1.read().clone();
  msg->output_image.write(image);

  if(msg->draw_kpts.read()){

    draw_kpts(image, node);
  }

  if(msg->draw_matches.read()){
    //draw_all_matches(image,node);
    draw_matches(image, node);
    draw_all_matches(image,node, Matches);
  }
            
}


void show_image(message* msg, Node* node,SLAM_Match_Vector& Matches){

  cv::Mat image=msg->image1.read();

  if(msg->draw_kpts.read()){

    draw_kpts(image, node);
  }

  if(msg->draw_matches.read()){
    //draw_all_matches(image,node);
    draw_matches(image, node);
    draw_all_matches(image,node, Matches);
  }

/*
  if(node->id>0){
    draw_ProjectedMapPoints(msg,node);
  }

  if(msg->draw_init_matches){
    draw_init_matches(msg, node);
  }

  if(msg->save_image){
    std::stringstream name;
    name<<msg->debug_folder<<node->frame_number<<".png";
    cv::imwrite(name.str(),msg->image1);
  }
*/

std::stringstream fps;
fps<<msg->fps.read() <<" "<<msg->n_matches.read();
cv::putText(image, //target image
          fps.str(), //text
          cv::Point(10, 10), //top-left position
          cv::FONT_HERSHEY_DUPLEX,
          0.5,
          CV_RGB(255, 0, 0), //font color
          1.5);

  cv::imshow("3DSLAM_Mx", image);
  //cv::imshow("3DSLAM_Mx", msg->image3);
  cv::waitKey(1);
}

void draw_all_matches(cv::Mat& image, Node* node, SLAM_Match_Vector& m){

  std::vector<keypoint> key_points;


  for(std::vector<std::vector<match>>::iterator it= m.vec_close.begin(); it!=m.vec_close.end(); ++it){

    for(std::vector<match>::iterator it2=(*it).begin();it2!=(*it).end();++it2){

      match m=*it2;
      keypoint kp=node->kpts1.read_element(m.SkptID);
      key_points.push_back(kp);

    }
  }

  draw_key_points(image, key_points, cv::Scalar(0,0,255) , 3, 1);

}


void draw_matches(cv::Mat& image, Node* node){

  std::vector<keypoint> key_points;
  std::vector<match> m=node->inlier_matches.read();

  for(std::vector<match>::iterator it=m.begin(); it!=m.end();++it){

    match m=*it;
    keypoint kp=node->kpts1.read_element(m.SkptID);
    key_points.push_back(kp);
  }

  draw_key_points(image, key_points, cv::Scalar(0,255,255) , 6, 1);

}


/*
void draw_init_matches(message* msg, Node* node){

  std::vector<keypoint> key_points;

  for(std::vector<match>::iterator it=node->init_matches.begin(); it!=node->init_matches.end();++it){

    match m=*it;
    keypoint kp=node->kpts[m.SkptID];
    key_points.push_back(kp);
  }

  //sstd::cout<< " init_matches "<<node->init_matches.size()<<std::endl;

  draw_key_points(msg->image1, key_points, cv::Scalar(255,0,255) , 10, 2);

}




void draw_ProjectedMapPoints(message* msg, Node* node){

  std::vector<keypoint> key_points;

  draw_key_points(msg->image1, key_points, cv::Scalar(255,0,255) , 4, 1);

}
*/
void draw_kpts(cv::Mat& image, Node* node){

  // draw allkpts
  SLAM_KPT_Vector key_points;
  draw_key_points(image, node->kpts1, cv::Scalar(255,0,0) , 1, 1);

  // draw kpts 0<depth<40
  key_points.clear();
  for(int i=0; i<node->kpts1.size(); i++){
    keypoint kp=node->kpts1.read_element(i);

    //std::cout<<" kpt333 "<<kp.LKeyPoint.pt.y<<std::endl;

    if (!kp.valid)
      continue;
    key_points.push_back(kp);
  }


  draw_key_points(image, key_points, cv::Scalar(0,255,0) , 3, 1);

}

void draw_key_points(cv::Mat& image, SLAM_KPT_Vector &kpts, cv::Scalar color, int marker_size, int line_thicknes){



  for(int i=0; i<kpts.size(); i++){

    keypoint kp=kpts.read_element(i);

    //if(kp.point4D(2)<=0 ||kp.point4D(2)>40)continue;

    cv::KeyPoint point=kp.LKeyPoint;
    int tlx=point.pt.x-marker_size;
    int tly=point.pt.y-marker_size;

    int brx=point.pt.x+marker_size;
    int bry=point.pt.y+marker_size;

    cv::Point tl=cv::Point(tlx,tly);
    cv::Point br=cv::Point(brx,bry);

    cv::rectangle(image, cv::Rect(tl,br), color,line_thicknes,8,0);

    cv::drawMarker(image,cv::Point(point.pt.x,point.pt.y),color,cv::MARKER_CROSS,marker_size,1,8 );

  }
}

void draw_key_points(cv::Mat& image, std::vector<keypoint> &kpts, cv::Scalar color, int marker_size, int line_thicknes){



  for(int i=0; i<kpts.size(); i++){

    keypoint kp=kpts[i];

    //if(kp.point4D(2)<=0 ||kp.point4D(2)>40)continue;

    cv::KeyPoint point=kp.LKeyPoint;
    int tlx=point.pt.x-marker_size;
    int tly=point.pt.y-marker_size;

    int brx=point.pt.x+marker_size;
    int bry=point.pt.y+marker_size;

    cv::Point tl=cv::Point(tlx,tly);
    cv::Point br=cv::Point(brx,bry);

    cv::rectangle(image, cv::Rect(tl,br), color,line_thicknes,8,0);

    cv::drawMarker(image,cv::Point(point.pt.x,point.pt.y),color,cv::MARKER_CROSS,marker_size,1,8 );

  }
}

void save_vector_to_txt(std::vector<float> v, std::string name){

  std::ofstream logfile;
  std::string path="../../i3DSLAM_Mx_debug/vectors";

  std::filesystem::create_directories(path);

  //initialize log
  std::stringstream logfile_name;
  logfile_name<<path<<"/"<<name<<".txt";
  logfile=std::ofstream(logfile_name.str());

  std::stringstream line;
  for(int i=0; i<v.size(); i++){
    line<<v[i]<<" ";
  }

  std::string line_str=line.str().c_str();
  logfile<<line_str<<std::endl;
  //close log file
  logfile.flush();
  logfile.close();
}

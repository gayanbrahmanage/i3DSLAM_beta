#include "stereo3D.h"

int DescriptorDistanceST(const cv::Mat &a, const cv::Mat &b){

    const int *pa = a.ptr<int32_t>();
    const int *pb = b.ptr<int32_t>();

    int dist=0;

    for(int i=0; i<8; i++, pa++, pb++)
    {
        unsigned  int v = *pa ^ *pb;
        v = v - ((v >> 1) & 0x55555555);
        v = (v & 0x33333333) + ((v >> 2) & 0x33333333);
        dist += (((v + (v >> 4)) & 0xF0F0F0F) * 0x1010101) >> 24;
    }

    return dist;
}


void get3Dpoint(int index,
                SLAM_KPT_Vector& kpts1,
                SLAM_KPT_Vector& kpts2,
                std::vector<std::vector<int>>& row_kpts,
                std::vector<std::vector<int>>& row_kpts2,
                const cv::Mat& feature_grid_map2,
                message* msg,
                parameters& param){

  keypoint kp=kpts1.read_element(index);
  int kpy=kp.LKeyPoint.pt.y;
  int d=1;
  int ymin=kpy-d;
  if(ymin<0)ymin=0;

  int ymax=kpy+d;
  if(ymax>row_kpts2.size()-1)ymax=row_kpts2.size()-1;

  int dist_ham_min=1e6;
  int matched_id=-1;

  for(int i=ymin; i<=ymax; i++){
    for(int j=1; j<row_kpts2[i].size(); j++){

      int xp=row_kpts2[i][j];
      int yp=i;
      int id=feature_grid_map2.at<int>(yp,xp);
      cv::Mat descrip_right=kpts2.read_element(id).descriptor;

      int dist_ham=DescriptorDistanceST(kp.descriptor,descrip_right);

      if(dist_ham<dist_ham_min){
        dist_ham_min=dist_ham;

        if(dist_ham<=param.ORB_HAMMING_DISTANCE_TH)
          matched_id=id;
      }

    }
  }

  if(matched_id!=-1){

    // compute 3D
    cv::Point2f  pt_left=kp.LKeyPoint.pt;
    cv::Point2f  pt_right=kpts2.read_element(matched_id).LKeyPoint.pt;

    float d=1000;
    float disparity=pt_left.x-pt_right.x;

    if(disparity>0){
      d=(msg->bf)/(disparity*1000.0f);
      //std::cout<<disparity<<std::endl;
    }else {
      d=0;
    }

    float  Zw = d;
    float  Xw =(pt_left.x - msg->cx) * Zw / msg->fx;
    float  Yw =(pt_left.y - msg->cy) * Zw / msg->fy;

    kp.point4D=Eigen::Vector4d(Xw,Yw,Zw,1);
    kp.RKeyPoint=kpts2.read_element(matched_id).LKeyPoint;
    kp.disparity=disparity;
    kp.depth_type=1;
    kp.far=Zw>param.max_depth;
    kp.valid=Zw > 0.1;

  }

  kpts1.write_element(index,kp);
}

void get3D( SLAM_KPT_Vector& kpts1,
            SLAM_KPT_Vector& kpts2,
            std::vector<std::vector<int>>& row_kpts1,
            std::vector<std::vector<int>>& row_kpts2,
            const cv::Mat& feature_grid_map2,
            message* msg,
            parameters& param){
  //
  int thread_index=0;
  std::thread threads[param.n_threads];

  for(int i=0; i<kpts1.size();i++){

    if(thread_index<param.n_threads){


      threads[thread_index] = std::thread(get3Dpoint,
                                          i,
                                          std::ref(kpts1),
                                          std::ref(kpts2),
                                          std::ref(row_kpts1),
                                          std::ref(row_kpts2),
                                          std::ref(feature_grid_map2),
                                          msg,
                                          std::ref(param));


      thread_index++;
    }

    if(thread_index>=param.n_threads || i>=kpts1.size()-1){
        //std::cout<<"Running 3D "<<thread_index<<std::endl;
        for(int j=0;j<thread_index;j++){
          threads[j].join();
        }
        thread_index=0;
      }
  }

}

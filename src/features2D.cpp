#include "features2D.h"

int DescriptorDistance(const cv::Mat &a, const cv::Mat &b){
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

void detect_orb(SLAM_variable<cv::Mat>& input,
                cv::Mat& feature_grid_map,
                std::vector<std::vector<int>>& row_kpts,
                SLAM_KPT_Vector& kpts,
                parameters &param){


  // Convert images to grayscale
  cv::Mat im2Gray=input.read().clone();
  if(im2Gray.channels()>1){
    cvtColor(im2Gray, im2Gray, cv::COLOR_BGR2GRAY);
  } else{

  }


  int grid_x=param.grid_x;
  int grid_y=param.grid_y;

  //int n_x=(im2Gray.cols)/grid_x+1;
  //int n_y=(im2Gray.rows)/grid_y+1;
  //int n_blocks=n_x*n_y;

  int n=0;

  for(int i=0; i<im2Gray.cols; i=i+grid_x){
      for(int j=0; j<im2Gray.rows; j=j+grid_y){
        int tlx=i-param.marging;;
        if(tlx<0)tlx=0;
        int tly=j-param.marging;;
        if(tly<0)tly=0;

        int brx=tlx+grid_x+param.marging;;
        if(brx>=im2Gray.cols)brx=im2Gray.cols-1;

        int bry=tly+grid_y+param.marging;;
        if(bry>=im2Gray.rows)bry=im2Gray.rows-1;

        if(brx<=tlx ||bry<=tly)
          continue;

        cv::Rect roi_r=cv::Rect(cv::Point(tlx,tly),cv::Point(brx,bry));
        cv::Mat roi= im2Gray(roi_r);

        block_DescriptorExtractor(n,roi,kpts,feature_grid_map,row_kpts,roi_r,param);

        n++;

    }
  }
  //std::cout<<" Kpts "<<kpts.size()<<std::endl;

}

void block_DescriptorExtractor(int bID,
                               cv::Mat& roi,
                               SLAM_KPT_Vector& kpts,
                               cv::Mat& feature_grid_map,
                               std::vector<std::vector<int>>& row_kpts,
                               cv::Rect& r,
                               parameters &param){

    cv::Ptr<cv::DescriptorExtractor> features_block_DescriptorExtractor=cv::ORB::create(param.nfeatures_per_grid_block,
                                                                                        param.scaleFactor,
                                                                                        param.nlevels,
                                                                                        param.edgeThreshold,
                                                                                        param.firstLevel,
                                                                                        param.WTA_K,
                                                                                        cv::ORB::HARRIS_SCORE,
                                                                                        param.patchSize,
                                                                                        param.fastThreshold);

    std::vector<cv::KeyPoint> keypoints_block;
    cv::Mat descriptors_block;

    double omega=pow(param.scaleFactor,param.nlevels);
    Eigen::Matrix3d kptinfoMatrix=Eigen::Matrix3d::Identity()*(1.0f/omega);

    features_block_DescriptorExtractor->detectAndCompute(roi, cv::Mat(), keypoints_block, descriptors_block);

    for(int k=0;k<keypoints_block.size();k++){

      if(keypoints_block[k].response<=0.0f)
        continue;

      int kpt_id=kpts.size();

      cv::KeyPoint kpt=keypoints_block[k];
      kpt.pt.x+=r.tl().x;
      kpt.pt.y+=r.tl().y;

      cv::Mat des= descriptors_block.row(k);

      int xp=(int)keypoints_block[k].pt.x+r.tl().x;
      int yp=(int)keypoints_block[k].pt.y+r.tl().y;

      feature_grid_map.at<int>(yp,xp)=kpt_id;
      row_kpts[yp].push_back(xp);

      keypoint KeyPoint;
      KeyPoint.id=kpt_id;
      KeyPoint.BlockID=bID;
      KeyPoint.xp=xp;
      KeyPoint.yp=yp;
      KeyPoint.LKeyPoint=kpt;
      KeyPoint.descriptor=des.clone();
      KeyPoint.kptinfoMatrix=kptinfoMatrix;
      kpts.push_back(KeyPoint);

      //std::cout<<"Detect "<<des.size()<<std::endl;


    }

}

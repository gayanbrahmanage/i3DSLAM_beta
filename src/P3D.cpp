#include "P3D.h"

float get_P3D(const cv::Mat& depth_image, int xp, int yp){

  return decode_depth_png(depth_image,xp,yp);

}

float decode_depth_png(const cv::Mat& src, int x, int y){

   int b=src.at<cv::Vec3b>(y,x)[0];
   int g=src.at<cv::Vec3b>(y,x)[1];
   int r=src.at<cv::Vec3b>(y,x)[2];
   float depth=r*255*255+g*255+b;
   depth=depth/1000;

   //std::cout<< b<<" "<<g<<" "<<r<<std::endl;
   return depth;
 }

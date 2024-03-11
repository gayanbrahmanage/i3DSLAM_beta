#include "functions.h"

bool project_MapPoint(int& xp, int&yp, Eigen::Vector4d& point4D, Eigen::Matrix4d tf,parameters& param){

  Eigen::Vector4d pointC=tf*point4D;
  double invZ=1.0/pointC(2);
  xp=param.cx + pointC(0)*param.fx*invZ;
  yp=param.cy + pointC(1)*param.fy*invZ;

  if(xp<param.width && xp>=0 && yp<param.height && yp>=0){
    return true;
  }else{
    return false;
  }
}

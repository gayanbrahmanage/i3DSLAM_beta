#ifndef COMPUTESE3_H
#define COMPUTESE3_H

#include "node.h"
#include <cstdlib>
#include <random>
#include "em.h"

struct SE3{

  unsigned int no_of_samples_ = 0;
  float accumulated_weight_ = 0;
  Eigen::Vector3d mean1_ = Eigen::Vector3d::Identity ();
  Eigen::Vector3d mean2_ = Eigen::Vector3d::Identity ();
  Eigen::Matrix3d covariance_ = Eigen::Matrix3d::Identity ();
  double weight = 1.0;

  void compute_statistics(Eigen::Vector3d& point , Eigen::Vector3d& corresponding_point){

    if(point(2)>1){
      weight=1.0f/point(2);
    }else{
      weight = 1.0;
    }

    ++no_of_samples_;
    accumulated_weight_ += weight;
    double alpha = weight/accumulated_weight_;

    Eigen::Vector3d diff1 = point - mean1_;
    Eigen::Vector3d diff2 = corresponding_point - mean2_;

    covariance_ = (1.0f-alpha)*(covariance_ + alpha * (diff2 * diff1.transpose()));

    mean1_ += alpha*(diff1);
    mean2_ += alpha*(diff2);

  }

  Eigen::Matrix4d compute(){

    Eigen::JacobiSVD<Eigen::Matrix<double, 3, 3> > svd (covariance_, Eigen::ComputeFullU | Eigen::ComputeFullV);
    const Eigen::Matrix<double, 3, 3>& u = svd.matrixU(),
                                     & v = svd.matrixV();
    Eigen::Matrix<double, 3, 3> s;
    s.setIdentity();
    if (u.determinant()*v.determinant() < 0.0f)
      s(2,2) = -1.0f;

    Eigen::Matrix<double, 3, 3> r = u * s * v.transpose();
    Eigen::Vector3d t = mean2_ - r*mean1_;

    Eigen::Affine3d ret;
    ret(0,0)=r(0,0); ret(0,1)=r(0,1); ret(0,2)=r(0,2); ret(0,3)=t(0);
    ret(1,0)=r(1,0); ret(1,1)=r(1,1); ret(1,2)=r(1,2); ret(1,3)=t(1);
    ret(2,0)=r(2,0); ret(2,1)=r(2,1); ret(2,2)=r(2,2); ret(2,3)=t(2);
    ret(3,0)=0.0f;   ret(3,1)=0.0f;   ret(3,2)=0.0f;   ret(3,3)=1.0f;

    return ret.matrix();

  }

};

void computeSE3(message* msg,
                em* ExMz,
                int node_id,
                int ref_node_id,
                std::map<int, Node*>& graph_nodes,
                SLAM_Match_Vector& m,
                parameters& param);


//
void ransac(
            int node_id,
            std::map<int, Node*>& graph_nodes,
            SLAM_Match_Vector& m,
            SLAM_Vector<match>& inlier_matches,
            SLAM_variable<Eigen::Matrix4d>& tf,
            message *msg,
            parameters& param);

std::vector<match> init_sample(SLAM_Match_Vector& m,parameters& param);

Eigen::Matrix4d computeTF(std::map<int, Node*>& graph_nodes,
                          std::vector<match>& sample,
                          parameters& param);

//
void select_matches_close(std::map<int, Node*>& graph_nodes,
                          SLAM_Match_Vector& m,
                          Eigen::Matrix4d &tf,
                          std::vector<match>& matches,
                          message *msg,
                          parameters& param);

//
void select_matches_far(std::map<int, Node*>& graph_nodes,
                        SLAM_Match_Vector& m,
                        Eigen::Matrix4d &tf,
                        SLAM_Vector<match>& matches,
                        message *msg,
                        parameters& param);

double error_pix(keypoint& kpt_source,
                 keypoint& kpt_target,
                 Eigen::Matrix4d& tf,
                 message *msg,
                 parameters& param);
#endif

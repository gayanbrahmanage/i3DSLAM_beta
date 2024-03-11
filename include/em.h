#ifndef em_H
#define em_H

#include "message.h"
#include "parameters.h"
#include "node.h"
#include "utils.h"
#include <boost/math/special_functions/digamma.hpp>

class em{

  private:
    double v=3;
    double P=3;
    double lambda=100;
    int iterations=20;
    std::vector<double> Eni;
    std::vector<double> ELogni;
    double SumEni;
    double SumELogni;
    Eigen::Vector3d mu_x;
    Eigen::Vector3d mu_y;


  public:
    em();
    ~em();

    void estimate(int node_id,
                  std::map<int, Node*>& graph_nodes,
                  SLAM_Vector<match>& m,
                  Eigen::Matrix4d& Rtf,
                  parameters& param);

    void compute_Ex(int node_id,
                    std::map<int, Node*>& graph_nodes,
                    SLAM_Vector<match>& m,
                    Eigen::Matrix4d& Rtf,
                    parameters& param);

    Eigen::Vector3d compute_Delta(keypoint& kpt_source,
                                  keypoint& kpt_target,
                                  Eigen::Matrix4d& tf,
                                  parameters& param);

    void compute_Motion(int node_id,
                        std::map<int, Node*>& graph_nodes,
                        SLAM_Vector<match>& m,
                        Eigen::Matrix4d& Rtf,
                        parameters& param);

    void compute_Lambda(int node_id,
                        std::map<int, Node*>& graph_nodes,
                        SLAM_Vector<match>& m,
                        Eigen::Matrix4d& tf,
                        parameters& param);

    void solve_v();
};
#endif

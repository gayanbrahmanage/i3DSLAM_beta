#ifndef motion_model_H
#define motion_model_H
#include <Eigen/Core>
#include "parameters.h"
#include "node.h"
#include <limits>
#include <math.h>

class motion_model{

public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  motion_model();
  ~motion_model();

  void init(parameters);
  void get_predicted_pose(std::map<int,Node*> graph_nodes, int id1, Node* source_node, int filter);
  void get_predicted_pose_from_MA(std::map<int,Node*> graph_nodes, int id1, Node* source_node);
  Eigen::Vector3d  tf_to_RPY(const Eigen::Matrix4d& t);
  Eigen::Vector3d  tf_to_RPY2(const Eigen::Matrix4d& t);
  Eigen::Vector3d  tf_to_xyz(const Eigen::Matrix4d& t);
  Eigen::Matrix4d eular_to_tf(Eigen::Vector3d rpy, Eigen::Vector3d xyz);
  Eigen::Matrix4d eular_to_tf2(Eigen::Vector3d rpy, Eigen::Vector3d xyz);

};

#endif

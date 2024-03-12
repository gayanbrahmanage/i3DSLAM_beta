#ifndef localBA_H
#define localBA_H

#include "message.h"
#include "parameters.h"
#include "node.h"

#include "g2o/core/sparse_optimizer.h"
#include "g2o/core/block_solver.h"
#include "g2o/core/solver.h"
#include "g2o/core/robust_kernel_impl.h"
#include "g2o/core/optimization_algorithm_levenberg.h"
#include "g2o/solvers/cholmod/linear_solver_cholmod.h"
#include "g2o/solvers/dense/linear_solver_dense.h"
#include "g2o/solvers/eigen/linear_solver_eigen.h"

#include "g2o/types/sba/types_six_dof_expmap.h"
#include "g2o/solvers/structure_only/structure_only_solver.h"

#include "g2o/types/slam3d/se3quat.h"
#include "g2o/types/slam3d/edge_se3.h"
#include "g2o/types/slam3d/parameter_camera.h"

#include "g2o/core/optimizable_graph.h"
#include "g2o/solvers/csparse/linear_solver_csparse.h"
#include "g2o/solvers/pcg/linear_solver_pcg.h"
#include "g2o/core/sparse_optimizer.h"
#include "g2o/core/optimization_algorithm_dogleg.h"
#include "g2o/core/optimization_algorithm_levenberg.h"

class localBA{

private:
  parameters param;
  int vertex_id=0;
  const float deltaMono = sqrt(5.991);
  const float deltaStereo = sqrt(7.815);
  const float thHuber3D = sqrt(7.815);

public:
  localBA();
  ~localBA();

  void run(std::map<int, Node*>& graph_nodes, int& id1, int& id2, message* msg);
  void optimize(std::map<int, Node*>& graph_nodes, int& id1, int& id2, message* msg);
  void reset();
}
;
#endif

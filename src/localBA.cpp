#include "localBA.h"

localBA::localBA(){


}

localBA::~localBA(){


}

void localBA::run(std::map<int, Node*>& graph_nodes, int& id1, int& id2, message* msg){

  optimize(graph_nodes,id1,id2, msg);

}

void localBA::optimize(std::map<int, Node*>& graph_nodes, int& id1, int& id2, message* msg){

  //std::cout<<"Optimizing "<<id1<<" "<<id2<<std::endl;

  g2o::SparseOptimizer optimizer;// = new g2o::SparseOptimizer();
  std::unique_ptr<g2o::BlockSolver_6_3::LinearSolverType> linearSolver;
  linearSolver = g2o::make_unique<g2o::LinearSolverDense<g2o ::BlockSolver_6_3::PoseMatrixType>>();
  g2o::OptimizationAlgorithmLevenberg* algo = new g2o::OptimizationAlgorithmLevenberg(g2o::make_unique<g2o::BlockSolver_6_3>(std::move(linearSolver)));
  optimizer.setAlgorithm(algo);

  for(int i=id1; i>=id2; --i){

    Node* node_s=graph_nodes[i];

    //std::cout<<node_s->MapPointID<<" Pose "<<std::endl<<node_s->pose.read()<<std::endl;

    Eigen::Matrix4d tf=node_s->pose.read().inverse();

    g2o::VertexSE3Expmap * vSE3 = new g2o::VertexSE3Expmap();
    vSE3->setEstimate(g2o::SE3Quat(tf.topLeftCorner(3,3),tf.topRightCorner(3,1)));
    vSE3->setId(node_s->MapPointID);
    vSE3->setFixed(false);
    vSE3->setMarginalized(false);
    optimizer.addVertex(vSE3);

    //add points
    for(int j=0; j<node_s->inlier_matches.size();j++){

      match m=node_s->inlier_matches.read_element(j);
      keypoint kps=node_s->kpts1.read_element(m.SkptID);

      // if(kps.far)
      //   continue;

      Node* node_t=graph_nodes[m.TnodeID];

      Eigen::Vector4d Wpoint4D=node_t->pose.read()*node_t->kpts1.read_point4D(m.TkptID);

      g2o::VertexSBAPointXYZ* vPoint = new g2o::VertexSBAPointXYZ();
      vPoint->setEstimate(Eigen::Vector3d(Wpoint4D(0),Wpoint4D(1),Wpoint4D(2)));
      vPoint->setId(node_t->kpts1.read_MapPointID(m.TkptID));
      vPoint->setMarginalized(true);
      vPoint->setFixed(true);
      optimizer.addVertex(vPoint);

      if(kps.depth_type==1){

          Eigen::Matrix<double,3,1> obs;
          obs<< kps.LKeyPoint.pt.x,kps.LKeyPoint.pt.y,kps.RKeyPoint.pt.x;

          g2o::EdgeStereoSE3ProjectXYZ* e = new g2o::EdgeStereoSE3ProjectXYZ();
          e->setVertex(0, dynamic_cast<g2o::OptimizableGraph::Vertex*>(optimizer.vertex(node_t->kpts1.read_MapPointID(m.TkptID))));
          e->setVertex(1, dynamic_cast<g2o::OptimizableGraph::Vertex*>(optimizer.vertex(node_s->MapPointID)));
          e->setMeasurement(obs);

          Eigen::Matrix3d Info = kps.kptinfoMatrix;
          e->setInformation(Info);

          g2o::RobustKernelHuber* rk = new g2o::RobustKernelHuber;
          e->setRobustKernel(rk);
          rk->setDelta(thHuber3D);

          e->fx = msg->fx;
          e->fy = msg->fy;
          e->cx = msg->cx;
          e->cy = msg->cy;
          e->bf = msg->bf;

          optimizer.addEdge(e);

        }else if(kps.depth_type==0){
          //MONOCULAR
          //std::cout<<" Monocular "<<std::endl;
          Eigen::Matrix<double,2,1> obs;
          obs << kps.LKeyPoint.pt.x,kps.LKeyPoint.pt.y;
          g2o::EdgeSE3ProjectXYZ* e = new g2o::EdgeSE3ProjectXYZ();

          e->setVertex(0, dynamic_cast<g2o::OptimizableGraph::Vertex*>(optimizer.vertex(node_t->kpts1.read_MapPointID(m.TkptID))));
          e->setVertex(1, dynamic_cast<g2o::OptimizableGraph::Vertex*>(optimizer.vertex(node_s->MapPointID)));
          e->setMeasurement(obs);

          Eigen::Matrix2d Info = kps.kptinfoMatrix.topLeftCorner(2,2);
          e->setInformation(Info);
          g2o::RobustKernelHuber* rk = new g2o::RobustKernelHuber;
          e->setRobustKernel(rk);
          rk->setDelta(deltaMono);

          e->fx = msg->fx;
          e->fy = msg->fy;
          e->cx = msg->cx;
          e->cy = msg->cy;

          optimizer.addEdge(e);

        }
    }
  }

  optimizer.initializeOptimization();
  bool flag=optimizer.optimize(param.g2o_iterations_local_BA);
  //std::cout<<"BA Optimizer Success "<< flag<<std::endl;

  for(int i=id1; i>=id2; --i){

  g2o::VertexSE3Expmap* vSE3_recov = static_cast<g2o::VertexSE3Expmap*>(optimizer.vertex(graph_nodes[i]->MapPointID));
  g2o::SE3Quat estimated_SE3quat = vSE3_recov->estimate();
  Eigen::Matrix4d estimated_4d = estimated_SE3quat.to_homogeneous_matrix().inverse();

  //std::cout<< "Optimixed "<<std::endl<< estimated_4d<<std::endl;
  graph_nodes[i]->pose.write(estimated_4d);
  msg->traj.write_element(i,estimated_4d);

  if(i==id1){
      msg->current_pose.write(estimated_4d);
    }

  }

  reset();
}

void localBA::reset(){

  vertex_id=0;
}

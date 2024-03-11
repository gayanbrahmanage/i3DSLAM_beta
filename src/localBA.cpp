#include "localBA.h"

localBA::localBA(){


}

localBA::~localBA(){


}

void localBA::run(std::map<int, Node*>& graph_nodes, int& id1, int& id2){

  optimize(graph_nodes,id1,id2);

}

void localBA::optimize(std::map<int, Node*>& graph_nodes, int& id1, int& id2){
  std::cout<<"Optimizing "<<id1<<" "<<id2<<std::endl;

  g2o::SparseOptimizer optimizer;// = new g2o::SparseOptimizer();
  std::unique_ptr<g2o::BlockSolver_6_3::LinearSolverType> linearSolver;
  linearSolver = g2o::make_unique<g2o::LinearSolverDense<g2o ::BlockSolver_6_3::PoseMatrixType>>();
  g2o::OptimizationAlgorithmLevenberg* algo = new g2o::OptimizationAlgorithmLevenberg(g2o::make_unique<g2o::BlockSolver_6_3>(std::move(linearSolver)));
  optimizer.setAlgorithm(algo);

  int vid;

  for(int i=id1; i>=id2; i--){

    std::cout<<"Optimizing Node "<<i<<std::endl;

    vertex_id++;

    Node* node_s=graph_nodes[i];
    std::cout<<"Pose"<<std::endl<<node_s->pose.read()<<std::endl;

    Eigen::Matrix4d tf=node_s->pose.read().inverse();

    g2o::VertexSE3Expmap * vSE3 = new g2o::VertexSE3Expmap();
    vSE3->setEstimate(g2o::SE3Quat(tf.topLeftCorner(3,3),tf.topRightCorner(3,1)));
    vSE3->setId(vertex_id);
    vSE3->setFixed(vertex_id==0);
    vSE3->setMarginalized(false);
    optimizer.addVertex(vSE3);
    node_s->vertex_id.write(vertex_id);
    vid=vertex_id;
    //add points
    for(int j=0; j<node_s->inlier_matches.size();j++){

      match m=node_s->inlier_matches.read_element(j);
      keypoint kps=node_s->kpts1.read_element(m.SkptID);
      Eigen::Matrix<double,3,1> obs;
      obs<< kps.LKeyPoint.pt.x,kps.LKeyPoint.pt.y,kps.RKeyPoint.pt.x;

      if(kps.far)
        continue;

      Node* node_t=graph_nodes[m.TnodeID];

      Eigen::Vector4d Wpoint4D=node_t->pose.read()*node_t->kpts1.read_point4D(m.TkptID);

      vertex_id++;
      g2o::VertexSBAPointXYZ* vPoint = new g2o::VertexSBAPointXYZ();
      vPoint->setEstimate(Eigen::Vector3d(Wpoint4D(0),Wpoint4D(1),Wpoint4D(2)));
      vPoint->setId(vertex_id);
      vPoint->setMarginalized(true);
      vPoint->setFixed(false);
      optimizer.addVertex(vPoint);



      g2o::EdgeStereoSE3ProjectXYZ* e = new g2o::EdgeStereoSE3ProjectXYZ();
      e->setVertex(0, dynamic_cast<g2o::OptimizableGraph::Vertex*>(optimizer.vertex(vertex_id)));
      e->setVertex(1, dynamic_cast<g2o::OptimizableGraph::Vertex*>(optimizer.vertex(node_s->vertex_id.read())));
      e->setMeasurement(obs);

      Eigen::Matrix3d Info = kps.kptinfoMatrix;
      e->setInformation(Info);

      g2o::RobustKernelHuber* rk = new g2o::RobustKernelHuber;
      e->setRobustKernel(rk);
      rk->setDelta(thHuber3D);

      e->fx = param.fx;
      e->fy = param.fy;
      e->cx = param.cx;
      e->cy = param.cy;
      e->bf = param.bf;

      optimizer.addEdge(e);

    }
  }

  optimizer.initializeOptimization();
  bool flag=optimizer.optimize(param.g2o_iterations_local_BA);
  std::cout<<"BA Optimizer Success "<< flag<<std::endl;

  g2o::VertexSE3Expmap* vSE3_recov = static_cast<g2o::VertexSE3Expmap*>(optimizer.vertex(vid));
  g2o::SE3Quat estimated_SE3quat = vSE3_recov->estimate();
  Eigen::Matrix4d estimated_4d = estimated_SE3quat.to_homogeneous_matrix().inverse();

  std::cout<< "Optimixed "<<vertex_id<<" "<<std::endl<< estimated_4d<<std::endl;
  //graph_nodes[id1]->pose.write(estimated_4d);

  reset();
}

void localBA::reset(){

  vertex_id=0;
}

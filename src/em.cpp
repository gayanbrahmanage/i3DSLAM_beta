#include "em.h"

em::em(){

}

em::~em(){

}

void em::estimate(int node_id,
                   std::map<int, Node*>& graph_nodes,
                   SLAM_Vector<match>& matches,
                   Eigen::Matrix4d& Rtf,
                   parameters& param){
  //
  lambda=100;
  Eigen::Matrix3d Sigma=Eigen::Matrix3d::Identity()*lambda;

  std::cout<<"  TF "<<std::endl<<Rtf<<std::endl<<std::endl;
  std::vector<float> vlambda;

  for(int i=0; i<iterations; i++){

    compute_Ex(node_id,graph_nodes,matches,Rtf,param);
    compute_Motion(node_id,graph_nodes,matches,Rtf,param);
    compute_Lambda(node_id,graph_nodes,matches,Rtf,param);
    solve_v();

    vlambda.push_back(lambda);
  }

  std::stringstream name;
  name<<"lambda_"<<node_id;
  save_vector_to_txt(vlambda,name.str());

  std::cout<<"TF EM v="<<v<<" Lambda="<<lambda<<std::endl<<Rtf<<std::endl<<std::endl;
}

void em::compute_Ex(int node_id,
                    std::map<int, Node*>& graph_nodes,
                    SLAM_Vector<match>& matches,
                    Eigen::Matrix4d& Rtf,
                    parameters& param){

  Eni.clear();
  ELogni.clear();
  SumEni=0;
  SumELogni=0;
  mu_x=Eigen::Vector3d(0,0,0);
  mu_y=Eigen::Vector3d(0,0,0);

  for(std::vector<match>::iterator it= matches.vec.begin(); it!=matches.vec.end(); ++it){

    match m=*it;
    keypoint kps=graph_nodes[m.SnodeID]->kpts1.read_element(m.SkptID);
    keypoint kpt=graph_nodes[m.TnodeID]->kpts1.read_element(m.TkptID);

    Eigen::Vector3d delta=compute_Delta(kps,kpt,Rtf,param);

    Eigen::Matrix3d sigma=Eigen::Matrix3d::Identity()*lambda;
    double eni=(v+P)/(v+delta.transpose()*sigma*delta);
    Eni.push_back(eni);
    SumEni+=eni;

    double b=(v+delta.transpose()*sigma*delta)/2;
    b=log(b);
    double y=(v+P)/2;
    double elogni=boost::math::digamma(y)-b;
    ELogni.push_back(elogni);
    SumELogni+=elogni;

    mu_x+=eni*kps.point4D.head(3);
    mu_y+=eni*kpt.Rpoint4D.head(3);

  }
  mu_x=mu_x/SumEni;
  mu_y=mu_y/SumEni;
}

Eigen::Vector3d em::compute_Delta(keypoint& kpt_source,
                                   keypoint& kpt_target,
                                   Eigen::Matrix4d& tf,
                                   parameters& param){

  Eigen::Vector4d x=kpt_source.point4D;
  Eigen::Vector4d y=kpt_target.Rpoint4D;
  Eigen::Vector4d delta=y-tf*x;
  return delta.head(3);

}

void em::compute_Motion(int node_id,
                        std::map<int, Node*>& graph_nodes,
                        SLAM_Vector<match>& matches,
                        Eigen::Matrix4d& tf,
                        parameters& param){
  //
  Eigen::Matrix3d A=Eigen::Matrix3d::Zero();
  int n=0;
  for(std::vector<match>::iterator it= matches.vec.begin(); it!=matches.vec.end(); ++it){

    match m=*it;
    keypoint kps=graph_nodes[m.SnodeID]->kpts1.read_element(m.SkptID);
    keypoint kpt=graph_nodes[m.TnodeID]->kpts1.read_element(m.TkptID);

    Eigen::Vector3d x_hat=kps.point4D.head(3)-mu_x;
    Eigen::Vector3d y_hat=kpt.Rpoint4D.head(3)-mu_y;

    A=A+y_hat*x_hat.transpose()*Eni[n];
    n++;
  }

  Eigen::JacobiSVD<Eigen::Matrix<double, 3, 3> > svd (A, Eigen::ComputeFullU | Eigen::ComputeFullV);
  const Eigen::Matrix<double, 3, 3>& u = svd.matrixU(),
                                   & v = svd.matrixV();
  Eigen::Matrix<double, 3, 3> s;
  s.setIdentity();
  if (u.determinant()*v.determinant() < 0.0f)
    s(2,2) = -1.0f;

  Eigen::Matrix<double, 3, 3> r = u * s * v.transpose();
  Eigen::Vector3d t = mu_y - r*mu_x;

  Eigen::Affine3d ret;
  ret(0,0)=r(0,0); ret(0,1)=r(0,1); ret(0,2)=r(0,2); ret(0,3)=t(0);
  ret(1,0)=r(1,0); ret(1,1)=r(1,1); ret(1,2)=r(1,2); ret(1,3)=t(1);
  ret(2,0)=r(2,0); ret(2,1)=r(2,1); ret(2,2)=r(2,2); ret(2,3)=t(2);
  ret(3,0)=0.0f;   ret(3,1)=0.0f;   ret(3,2)=0.0f;   ret(3,3)=1.0f;

  tf=ret.matrix();

}

void em::compute_Lambda(int node_id,
                        std::map<int, Node*>& graph_nodes,
                        SLAM_Vector<match>& matches,
                        Eigen::Matrix4d& tf,
                        parameters& param){

  //
  double denominator=0;
  int n=0;

  for(std::vector<match>::iterator it= matches.vec.begin(); it!=matches.vec.end(); ++it){

    match m=*it;
    keypoint kps=graph_nodes[m.SnodeID]->kpts1.read_element(m.SkptID);
    keypoint kpt=graph_nodes[m.TnodeID]->kpts1.read_element(m.TkptID);

    Eigen::Vector3d delta=compute_Delta(kps,kpt,tf,param);

    denominator+=delta.transpose()*Eni[n]*delta;
    n++;
    lambda=matches.size()/denominator;
  }
}

void em::solve_v(){
  int N=Eni.size();
  double c=SumELogni-SumEni;
  c=c/N;
  double a=1;
  double b=3;
  double x;

  double fa=c+1-boost::math::digamma(a/2)+log(a/2);
  double fb=c+1-boost::math::digamma(b/2)+log(b/2);

  //std::cout<<fa<<" v "<<fb<<" di a b "<<boost::math::digamma(a/2)<<" "<<boost::math::digamma(b/2)<<std::endl;

  if(fa>0 && fb<0){
    double e=100;

    while (e>0.001){
      x=(a+b)/2;
      double f=c-boost::math::digamma(x/2)+log(x/2)+1;
      e=abs(f);

      if(f>0)a=x;
      else if(f<0)b=x;
      else{
        a=x;
        b=x;
      }
    }
    v=x;
  }
}

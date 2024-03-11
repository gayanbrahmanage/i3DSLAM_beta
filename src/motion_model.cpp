#include <motion_model.h>

motion_model::motion_model(){

}

motion_model::~motion_model(){

}

void motion_model::init(parameters param){

}

void motion_model::get_predicted_pose(std::map<int,Node*> graph_nodes, int id1, Node* source_node, int filter){

  Eigen::Matrix4d predicted_pose=Eigen::Matrix4d::Identity();
  Eigen::Matrix4d predicted_tf=Eigen::Matrix4d::Identity();

  if(filter==1){
    get_predicted_pose_from_MA(graph_nodes, id1, source_node);
  }else{

  }


}

void motion_model::get_predicted_pose_from_MA(std::map<int,Node*> graph_nodes, int id1, Node* source_node){

  Eigen::Matrix4d predicted_pose;
  Eigen::Matrix4d predicted_tf;

  if(graph_nodes.size()<=2){
      predicted_pose=Eigen::Matrix4d::Identity();
      predicted_tf=Eigen::Matrix4d::Identity();
  }else{


    std::vector<Eigen::Matrix4d,Eigen::aligned_allocator<Eigen::Matrix4d> > d_pose;
    std::vector<double> dtime;

    Eigen::Vector3d rps_dot(0,0,0);
    Eigen::Vector3d xyz_dot(0,0,0);

    int id2=source_node->id.read();

    for(int i=id2-1; i>id1 /* latest node is not yet added to the graph */; i--){

      Node* node_t_1=graph_nodes[i];
      Node* node_t_2=graph_nodes[i-1];

      Eigen::Matrix4d delta_pose=node_t_2->pose.read().inverse()*node_t_1->pose.read();

      d_pose.push_back(delta_pose);

      float dt=node_t_1->time_stamp.read()-node_t_2->time_stamp.read();
      dtime.push_back((double)dt);
      //
      // std::cout<<"Motion Model "<<id1<<" "<<id2<<std::endl;
      // std::cout<<delta_pose<<std::endl;
      // std::cout<<node_t_1->time_stamp-node_t_2->time_stamp<<std::endl;
    }

    for(int i=0; i<d_pose.size();i++){
      Eigen::Vector3d rpy=tf_to_RPY(d_pose[i]);
      Eigen::Vector3d xyz=tf_to_xyz(d_pose[i]);
      rps_dot+=rpy/dtime[i];
      xyz_dot+=xyz/dtime[i];

    }

    rps_dot=rps_dot/d_pose.size();
    xyz_dot=xyz_dot/d_pose.size();

    Node* node_t_1=graph_nodes[id2-1];

    Eigen::Vector3d rps_p=rps_dot*(source_node->time_stamp.read()-node_t_1->time_stamp.read());
    Eigen::Vector3d xyz_p=xyz_dot*(source_node->time_stamp.read()-node_t_1->time_stamp.read());

    Eigen::Matrix4d tf=eular_to_tf( rps_p, xyz_p);
    predicted_tf=tf;
    predicted_pose=node_t_1->pose.read()*tf;


   }

   source_node->predicted_tf.write(predicted_tf);
   source_node->predicted_pose.write(predicted_pose);
   //std::cout<<"Predicted Motion "<<std::endl<<predicted_tf<<std::endl;

}

Eigen::Matrix4d motion_model::eular_to_tf(Eigen::Vector3d rpy, Eigen::Vector3d xyz){

  Eigen::AngleAxisd rollAngle(rpy(0), Eigen::Vector3d::UnitX());
  Eigen::AngleAxisd pitchAngle(rpy(1), Eigen::Vector3d::UnitY());
  Eigen::AngleAxisd yawAngle(rpy(2), Eigen::Vector3d::UnitZ());


  Eigen::Quaternion<double> q =  yawAngle * pitchAngle*rollAngle ;

  Eigen::Matrix3d rotationMatrix = q.matrix();

  Eigen::Matrix4d tf=Eigen::Matrix4d::Identity();

  tf.block<3,3>(0,0) = rotationMatrix;
  tf.block<3,1>(0,3) = xyz;

  return tf;
}

///Get euler angles from affine matrix (helper for isBigTrafo)
Eigen::Vector3d motion_model::tf_to_RPY(const Eigen::Matrix4d& t) {

    double roll = atan2(t(2,1),t(2,2));
    double pitch = atan2(-t(2,0),sqrt(t(2,1)*t(2,1)+t(2,2)*t(2,2)));
    double yaw = atan2(t(1,0),t(0,0));

    Eigen::Vector3d  rpy(roll, pitch, yaw);
    return rpy;

}

Eigen::Vector3d  motion_model::tf_to_xyz(const Eigen::Matrix4d& t){
    Eigen::Vector3d  xyz(t(0,3), t(1,3), t(2,3));
    return xyz;
}

Eigen::Matrix4d motion_model::eular_to_tf2(Eigen::Vector3d rpyinput, Eigen::Vector3d xyz){

  // %     'szxy See the table below // ORB SLAM ,3D SALM, DSLAM
  //
  // %     'sxyz': (1, 0, 0, 0), 'sxyx': (1, 0, 1, 0), 'sxzy': (1, 1, 0, 0),
  // %     'sxzx': (1, 1, 1, 0), 'syzx': (2, 0, 0, 0), 'syzy': (2, 0, 1, 0),
  // %     'syxz': (2, 1, 0, 0), 'syxy': (2, 1, 1, 0), 'szxy': (3, 0, 0, 0),
  // %     'szxz': (3, 0, 1, 0), 'szyx': (3, 1, 0, 0), 'szyz': (3, 1, 1, 0),
  // %     'rzyx': (1, 0, 0, 1), 'rxyx': (1, 0, 1, 1), 'ryzx': (1, 1, 0, 1),
  // %     'rxzx': (1, 1, 1, 1), 'rxzy': (2, 0, 0, 1), 'ryzy': (2, 0, 1, 1),
  // %     'rzxy': (2, 1, 0, 1), 'ryxy': (2, 1, 1, 1), 'ryxz': (3, 0, 0, 1),
  // %     'rzxz': (3, 0, 1, 1), 'rxyz': (3, 1, 0, 1), 'rzyz': (3, 1, 1, 1)

  Eigen::Vector4d setting(3,0,0,0);
  int firstaxis=setting(0);
  int parity=setting(1);
  int repetition=setting(2);
  int frame=setting(3);

  Eigen::Vector4d NEXT_AXIS(2, 3, 1, 2);

  int i=firstaxis-1; //2
  int j=NEXT_AXIS(i+parity)-1; //0
  int k=NEXT_AXIS(i-parity+1)-1; //1

  Eigen::Vector3d rpy;

  if(frame==1){
    rpy(0)=rpyinput(2);
    rpy(1)=rpyinput(1);
    rpy(2)=rpyinput(0);
  }

  if(parity==1){
    rpy(0)=-rpyinput(0);
    rpy(1)=-rpyinput(1);
    rpy(2)=-rpyinput(2);
  }

  double si = sin(rpy(0));
  double sj = sin(rpy(1));
  double sk = sin(rpy(2));

  double ci = cos(rpy(0));
  double cj = cos(rpy(1));
  double ck = cos(rpy(2));

  double cc = ci*ck;
  double cs = ci*sk;

  double sc = si*ck;
  double ss = si*sk;

  Eigen::Matrix4d M =Eigen::Matrix4d::Identity();
  if(repetition==1){
      M(i, i) = cj;
      M(i, j) = sj*si;
      M(i, k) = sj*ci;
      M(j, i) = sj*sk;
      M(j, j) = -cj*ss+cc;
      M(j, k) = -cj*cs-sc;
      M(k, i) = -sj*ck;
      M(k, j) = cj*sc+cs;
      M(k, k) = cj*cc-ss;
  }else{
      M(i, i) = cj*ck;
      M(i, j) = sj*sc-cs;
      M(i, k) = sj*cc+ss;
      M(j, i)= cj*sk;
      M(j, j) = sj*ss+cc;
      M(j, k) = sj*cs-sc;
      M(k, i) = -sj;
      M(k, j) = cj*si;
      M(k, k) = cj*ci;
  }

  M.block<3,1>(0,3) = xyz;
  return M;
}

///Get euler angles from affine matrix (helper for isBigTrafo)
Eigen::Vector3d motion_model::tf_to_RPY2(const Eigen::Matrix4d& R) {

  // %     'szxy See the table below // ORB SLAM ,3D SALM, DSLAM
  //
  // %     'sxyz': (1, 0, 0, 0), 'sxyx': (1, 0, 1, 0), 'sxzy': (1, 1, 0, 0),
  // %     'sxzx': (1, 1, 1, 0), 'syzx': (2, 0, 0, 0), 'syzy': (2, 0, 1, 0),
  // %     'syxz': (2, 1, 0, 0), 'syxy': (2, 1, 1, 0), 'szxy': (3, 0, 0, 0),
  // %     'szxz': (3, 0, 1, 0), 'szyx': (3, 1, 0, 0), 'szyz': (3, 1, 1, 0),
  // %     'rzyx': (1, 0, 0, 1), 'rxyx': (1, 0, 1, 1), 'ryzx': (1, 1, 0, 1),
  // %     'rxzx': (1, 1, 1, 1), 'rxzy': (2, 0, 0, 1), 'ryzy': (2, 0, 1, 1),
  // %     'rzxy': (2, 1, 0, 1), 'ryxy': (2, 1, 1, 1), 'ryxz': (3, 0, 0, 1),
  // %     'rzxz': (3, 0, 1, 1), 'rxyz': (3, 1, 0, 1), 'rzyz': (3, 1, 1, 1)

  Eigen::Vector4d setting(3,0,0,0);
  float eps = std::numeric_limits<float>::denorm_min();
  float EPS=4*eps;

  int firstaxis=setting(0);
  int parity=setting(1);
  int repetition=setting(2);
  int frame=setting(3);

  Eigen::Vector4d NEXT_AXIS(2, 3, 1, 2);

  int i=firstaxis-1; //2
  int j=NEXT_AXIS(i+parity)-1; //0
  int k=NEXT_AXIS(i-parity+1)-1; //1

  double ax,ay,az;

  if(repetition==1){
    double sy=sqrt(R(i,j)*R(i,j)+R(i,k)*R(i,k));
    if(sy>EPS){
            ax = atan2( R(i,j),  R(i,k));
            ay = atan2( sy,      R(i,i));
            az =atan2( R(j,i), -R(k,i));
    }else{
            ax = atan2(-R(j,k),  R(j,j));
            ay = atan2( sy,      R(i,i));
            az = 0.0;
    }

  }else{

    double cy = sqrt(R(i, i)*R(i, i) + R(j, i)*R(j, i));

    cy = sqrt(R(i, i)*R(i, i) + R(j, i)*R(j, i));

    if(cy > EPS){
        ax = atan2( R(k, j),  R(k, k));
        ay = atan2(-R(k, i),  cy);
        az = atan2( R(j, i),  R(i, i));
    }else{
        ax =atan2(-R(j, k),  R(j, j));
        ay = atan2(-R(k, i),  cy);
        az = 0.0;
    }
  }

  if(parity==1){
      ax=-ax;
      ay=-ay;
      az=-az;
  }

  if(frame==1){
      ax=az;
      az=ax;
  }

  Eigen::Vector3d  rpy(ax, ay, az);
  return rpy;

}

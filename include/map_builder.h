#ifndef MAP_BUILDER_H
#define MAP_BUILDER_H

#include "node.h"
#include <utils.h>
#include <motion_model.h>
#include "functions.h"
#include "computeSE3.h"
#include "em.h"
#include "localBA.h"
#include "gui.h"

class MapBuilder{

  private:
    parameters param;
    int node_id=-1;
    int n_blocks;

  public:
    std::map<int,Node*> graph_nodes;
    std::vector<keypoint> kpts_in_temporal_window;
    SLAM_Match_Vector Matches;

    localBA BA;

    em* ExMz=new em();

    MapBuilder();
    ~MapBuilder();

    void run(message* msg, gui* caller);
    void align_source_node(message* msg,Node* source_node);
    void search_obzs_over_window(int nsID,int ntID1, int ntID2,message* msg);
    void search_obzs_frame(int nsID,int ntID, int refID,message* msg);
    void search_obz(Node* ns, Node* nt, keypoint& kpt,cv::Mat descriptor, cv::Mat feature_grid_map1, Eigen::Matrix4d& Delta_Pose, message* msg);

};

#endif

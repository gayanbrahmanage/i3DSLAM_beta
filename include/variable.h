#ifndef VARIABLE_H
#define VARIABLE_H

#include <mutex>
#include "keypoint.h"

template <class T> class SLAM_variable{

  private:
    T value;
    std::mutex mtx;

  public:

    SLAM_variable(){

    }
    ~SLAM_variable(){

    }

    void write(const T &t){

      std::lock_guard<std::mutex> lock(mtx);
      value=t;
    }

    T read(){
      std::lock_guard<std::mutex> lock(mtx);
      return value;
    }


};


class SLAM_KPT_Vector{

  private:

    std::mutex mtx;
    std::vector<keypoint> vec;

  public:



    SLAM_KPT_Vector(){

    }

    ~SLAM_KPT_Vector(){

    }

    int size(){
      std::lock_guard<std::mutex> lock(mtx);
      return vec.size();
    }

    void write(const std::vector<keypoint> &v){

      std::lock_guard<std::mutex> lock(mtx);
      vec=v;
    }

    std::vector<keypoint> read(){
      std::lock_guard<std::mutex> lock(mtx);
      return vec;
    }

    void push_back(const keypoint &e){
      std::lock_guard<std::mutex> lock(mtx);
      vec.push_back(e);
    }

    keypoint read_element(int index){
      std::lock_guard<std::mutex> lock(mtx);
      return vec[index];
    }

    void write_element(int index, keypoint& e){
      std::lock_guard<std::mutex> lock(mtx);
      vec[index]=e;
    }

    void clear(){
      std::lock_guard<std::mutex> lock(mtx);
      vec.clear();
    }

    bool read_valid(int index){
      std::lock_guard<std::mutex> lock(mtx);
      return vec[index].valid;
    }

    bool read_assigned(int index){
      std::lock_guard<std::mutex> lock(mtx);
      return vec[index].assigned;
    }

    void write_assigned(int index, bool e){
      std::lock_guard<std::mutex> lock(mtx);
      vec[index].assigned=e;
    }

    int read_MapPointID(int index){
      std::lock_guard<std::mutex> lock(mtx);
      return vec[index].MapPointID;
    }

    void write_MapPointID(int index,int MapPointID){
      std::lock_guard<std::mutex> lock(mtx);
      vec[index].MapPointID=MapPointID;
    }

    int read_xp(int index){
      std::lock_guard<std::mutex> lock(mtx);
      return vec[index].xp;
    }

    int read_yp(int index){
      std::lock_guard<std::mutex> lock(mtx);
      return vec[index].yp;
    }

    Eigen::Vector4d read_point4D(int index){
      std::lock_guard<std::mutex> lock(mtx);
      return vec[index].point4D;
    }

    Eigen::Vector4d read_Rpoint4D(int index){
      std::lock_guard<std::mutex> lock(mtx);
      return vec[index].Rpoint4D;
    }

    void write_Rpoint4D(int index, Eigen::Vector4d& v){
      std::lock_guard<std::mutex> lock(mtx);
      vec[index].Rpoint4D=v;
    }

    int descriptor_dist(int index, cv::Mat& target_Mat, bool& valid, bool& far, int& BlockID){
      std::lock_guard<std::mutex> lock(mtx);
      keypoint kp=vec[index];
      int dist=DescriptorDistance(target_Mat, kp.descriptor);
      valid=kp.valid;
      far=kp.far;
      BlockID=kp.BlockID;

      return dist;
    }

    int DescriptorDistance(const cv::Mat &a, const cv::Mat &b){
        const int *pa = a.ptr<int32_t>();
        const int *pb = b.ptr<int32_t>();

        int dist=0;

        for(int i=0; i<8; i++, pa++, pb++)
        {
            unsigned  int v = *pa ^ *pb;
            v = v - ((v >> 1) & 0x55555555);
            v = (v & 0x33333333) + ((v >> 2) & 0x33333333);
            dist += (((v + (v >> 4)) & 0xF0F0F0F) * 0x1010101) >> 24;
        }

        return dist;
    }


};

class SLAM_Match_Vector{

  private:

    std::mutex mtx;
    int n_blocks;

  public:
    int n_matches=0;
    std::vector<std::vector<match>> vec_close;
    std::vector<int> vec_close_BlockIDs;

    std::vector<std::vector<match>> vec_far;
    std::vector<int> vec_far_BlockIDs;

    SLAM_Match_Vector(){

    }

    ~SLAM_Match_Vector(){

    }

    void init(int n_blcks){

      std::lock_guard<std::mutex> lock(mtx);
      vec_close.clear();
      vec_close_BlockIDs=std::vector<int>(n_blcks,-1);

      vec_far.clear();
      vec_far_BlockIDs=std::vector<int>(n_blcks,-1);

      n_blocks=n_blcks;
      n_matches=0;
    }

    int size(){
      std::lock_guard<std::mutex> lock(mtx);
      return n_matches;
    }

    match read_element(int BlockID){
      std::lock_guard<std::mutex> lock(mtx);

    }

    void write_element_close(int BlockID, match& m){
      std::lock_guard<std::mutex> lock(mtx);
      n_matches++;
      int position=vec_close_BlockIDs[BlockID];

      if(position==-1){

        vec_close_BlockIDs[BlockID]=vec_close.size();
        std::vector<match> mv;
        mv.push_back(m);
        vec_close.push_back(mv);

      }else{

        vec_close[position].push_back(m);
      }
    }

    void write_element_far(int BlockID, match& m){
      std::lock_guard<std::mutex> lock(mtx);

      int position=vec_far_BlockIDs[BlockID];

      if(position==-1){

        vec_far_BlockIDs[BlockID]=vec_far.size();
        std::vector<match> mv;
        mv.push_back(m);
        vec_far.push_back(mv);

      }else{

        vec_far[position].push_back(m);
      }
    }

    void reset(){

      std::lock_guard<std::mutex> lock(mtx);
      vec_close.clear();
      vec_close_BlockIDs=std::vector<int>(n_blocks,-1);

      vec_far.clear();
      vec_far_BlockIDs=std::vector<int>(n_blocks,-1);
      n_matches=0;
    }

    void remove_unused_BlockIDs(){
      vec_close_BlockIDs.erase( std::remove( vec_close_BlockIDs.begin(),
            vec_close_BlockIDs.end(), -1 ), vec_close_BlockIDs.end() ) ;
    }
};


template <class T> class SLAM_Vector{

  private:

    std::mutex mtx;

  public:

    std::vector<T> vec;

    SLAM_Vector(){

    }

    ~SLAM_Vector(){

    }

    int size(){
      std::lock_guard<std::mutex> lock(mtx);
      return vec.size();
    }

    void write(const std::vector<T> &v){

      std::lock_guard<std::mutex> lock(mtx);
      vec=v;
    }

    void push_back(T v){

      std::lock_guard<std::mutex> lock(mtx);
      vec.push_back(v);
    }

    std::vector<T> read(){
      std::lock_guard<std::mutex> lock(mtx);
      return vec;
    }


    T read_element(int index){
      std::lock_guard<std::mutex> lock(mtx);
      return vec[index];
    }

    void write_element(int index, T& e){
      std::lock_guard<std::mutex> lock(mtx);
      vec[index]=e;
    }

    void merge(std::vector<T> v){
      std::lock_guard<std::mutex> lock(mtx);
      vec.insert(vec.end(), v.begin(), v.end());
    }

    void clear(){
      std::lock_guard<std::mutex> lock(mtx);
      vec.clear();
    }
};


template <class T> class SLAM_Map{

  private:
    std::map<int,T> map;
    std::mutex mtx;

  public:

    SLAM_Map(){

    }

    ~SLAM_Map(){

    }

    int size(){
      return map.size();
    }

    void write(const std::map<int,T> &m){

      std::lock_guard<std::mutex> lock(mtx);
      map=m;
    }

    std::map<int,T> read(){
      std::lock_guard<std::mutex> lock(mtx);
      return map;
    }


    T read_element(int index){
      std::lock_guard<std::mutex> lock(mtx);
      return map[index];
    }

    void write_element(int index, T& e){
      std::lock_guard<std::mutex> lock(mtx);
      map[index]=e;
    }

    void clear(){
      std::lock_guard<std::mutex> lock(mtx);
      map.clear();
    }
};

#define EigenMat4dMap std::map<int, Eigen::Matrix4d, std::less<int>, Eigen::aligned_allocator<std::pair<const int, Eigen::Matrix4d>>>

class Trajectory{

  private:
    EigenMat4dMap map;
    std::mutex mtx;

  public:

    Trajectory(){

    }

    ~Trajectory(){

    }

    int size(){
      return map.size();
    }

    void write(const EigenMat4dMap &m){

      std::lock_guard<std::mutex> lock(mtx);
      map=m;
    }

    EigenMat4dMap read(){
      std::lock_guard<std::mutex> lock(mtx);
      return map;
    }


    Eigen::Matrix4d read_element(int index){
      std::lock_guard<std::mutex> lock(mtx);
      return map[index];
    }

    void write_element(int index, Eigen::Matrix4d& e){
      std::lock_guard<std::mutex> lock(mtx);
      map[index]=e;
    }

    void clear(){
      std::lock_guard<std::mutex> lock(mtx);
      map.clear();
    }
};


#endif

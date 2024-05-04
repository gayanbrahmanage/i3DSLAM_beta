#include "video_widget.h"
#include <cairomm/context.h>
#include <giomm/resource.h>
#include <gdkmm/general.h>
#include <glibmm/fileutils.h>
#include <iostream>


 video_widget::video_widget(const std::string& filename)
 {
   try{

     read_background_image(filename);
     video_image=Gdk::Pixbuf::create_from_data(bg_image.data,Gdk::COLORSPACE_RGB, false, 8, bg_image.cols, bg_image.rows, bg_image.step);

   }catch(...)
   {
     std::cerr<<"Video Widget::An error occured while loading the image file."<<std::endl;
   }

   if(video_image)
   set_size_request(video_image->get_width(), video_image->get_height());
   set_halign(Gtk::ALIGN_CENTER);
   set_valign(Gtk::ALIGN_CENTER);
 }

 video_widget::~video_widget()
 {
   //dtor
 }

 bool video_widget::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
 {
   if(!video_image)
     return false;

   Gtk::Allocation allocation = get_allocation();
   const int height = allocation.get_height();
   const int width = allocation.get_width();

   Gdk::Cairo::set_source_pixbuf(cr, video_image, width-video_image->get_width(), height-video_image->get_height());

   cr->paint();
   return true;
 }

 void video_widget::read_background_image(const std::string& filename){

   bg_image=cv::Mat(cv::Size(param->video_width, param->video_height), CV_8UC3, param->gui_b_color);

   cv::Mat image=cv::imread(filename);
   double ratio=0.4;
   int width=(int)(ratio*image.cols);
   int height=(int)(ratio*image.rows);

   cv::resize(image, image, cv::Size(width,height), 0, 0, cv::INTER_NEAREST);

   int r0=(bg_image.rows-image.rows)/2;
   int c0=(bg_image.cols-image.cols)/2;

   for(int i=0; i<image.rows; i++){
     for(int j=0; j<image.cols; j++){
       if((int)image.at<cv::Vec3b>(i,j)[0]>100){
         image.at<cv::Vec3b>(i,j)=cv::Vec3b(param->gui_b_color(0),param->gui_b_color(1),param->gui_b_color(2));
       }
     }
   }

   image.copyTo(bg_image(cv::Rect(c0, r0, image.cols, image.rows)));


   cv::putText(bg_image,
             "2D Camera",
             cv::Point(120, 220),
             cv::FONT_HERSHEY_SIMPLEX ,
             0.4,
             CV_RGB(200, 200, 200),
             1);

 }

 void video_widget::update(cv::Mat frame){

   video_image=Gdk::Pixbuf::create_from_data(frame.data,Gdk::COLORSPACE_RGB, false, 8, frame.cols, frame.rows, frame.step);
   if(video_image)
   set_size_request(video_image->get_width(), video_image->get_height());
   set_halign(Gtk::ALIGN_CENTER);
   set_valign(Gtk::ALIGN_CENTER);
   queue_draw();

 }

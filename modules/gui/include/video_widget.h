#ifndef video_widget_H
#define video_widget_H

#include <gtkmm/drawingarea.h>
#include <gdkmm/pixbuf.h>
#include "message.h"
#include "parameters.h"

 class video_widget : public Gtk::DrawingArea{
   private:
     void read_background_image(const std::string& filename);
   public:
     parameters* param=new parameters();
     cv::Mat bg_image;
     video_widget(const std::string& filename);
     virtual ~video_widget();

     void update(cv::Mat);

   protected:
     Glib::RefPtr<Gdk::Pixbuf> video_image;
     bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

 };

 #endif

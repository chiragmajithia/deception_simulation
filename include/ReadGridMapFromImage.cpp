#include <ros/ros.h>
#include <grid_map_ros/grid_map_ros.hpp>
#include <grid_map_msgs/GridMap.h>
#include <cmath>
#include <iostream>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Image.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace grid_map;
using namespace cv;

namespace ReadGridMapFromImage {

  void populateMap(GridMap &map, string layer,string file_path, float scale,float res)
  {
    cv_bridge::CvImage cv_image;
    Mat img = imread(file_path,CV_LOAD_IMAGE_GRAYSCALE);
    cv::Size img_size= img.size();
    cv::resize(img,img,cv::Size(),scale,scale, CV_INTER_CUBIC);
    map.setGeometry(Length(img.rows*res,img.cols*res),res);
    for (GridMapIterator it(map); !it.isPastEnd(); ++it) 
    {
      Position position;
      map.getPosition(*it, position);
      int x = (img.rows/2) + position.x()/res+res/2.0; 
      int y = (img.cols/2) + position.y()/res+res/2.0;
      map.at(layer, *it) = img.at<uchar>(x,y)<200?10:0;    
    }
    map.setTimestamp(ros::Time::now().toNSec());
    ROS_INFO("Created map with size %f x %f m (%i x %i cells).",map.getLength().x(), map.getLength().y(),map.getSize()(0), map.getSize()(1));
  }
}


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
using namespace std;
using namespace cv;
void populateMap(GridMap &map);
void populateMap(GridMap &map,string path);

int main(int argc, char** argv)
{
  // Initialize node and publisher.
  ros::init(argc, argv, "grid_map_simple_demo");
  ros::NodeHandle nh("~");
  ros::Publisher publisher = nh.advertise<grid_map_msgs::GridMap>("grid_map", 1, true);

  // Create grid map.
  GridMap map({"elevation"});
  map.setFrameId("map");
  //map.setGeometry(Length(1.2, 2.0), 0.03);
  ROS_INFO("Created map with size %f x %f m (%i x %i cells).",
    map.getLength().x(), map.getLength().y(),
    map.getSize()(0), map.getSize()(1));

  // Work with grid map in a loop.
  ros::Rate rate(0.1);
  // populateMap(map);
  populateMap(map,"mapping done");
  while (nh.ok()) {

    // Add data to grid map.
    // Publish grid map.

    grid_map_msgs::GridMap message;
    GridMapRosConverter::toMessage(map, message);
    publisher.publish(message);
    ROS_INFO_THROTTLE(1.0, "Grid map (timestamp %f) published.", message.info.header.stamp.toSec());

    // Wait for next cycle.
    rate.sleep();
  }

  return 0;
}

void populateMap(GridMap &map)
{
  ros::Time time = ros::Time::now();
  for (GridMapIterator it(map); !it.isPastEnd(); ++it) {
    Position position;
    map.getPosition(*it, position);
    map.at("elevation", *it) = -0.04 + 0.2 * std::sin(3.0 * time.toSec() + 5.0 * position.y()) * position.x();
  }
  map.setTimestamp(time.toNSec());
}

void populateMap(GridMap &map, string path)
{
  map.setBasicLayers({"elevation"});
  cv_bridge::CvImage cv_image;
  path = "/home/chirag/test/src/deception_simulation/files/exp_11-sparse.png";
  Mat img = imread(path,CV_LOAD_IMAGE_GRAYSCALE);
  cout<<"image size ="<<img.rows<<","<<img.cols;
  map.setGeometry(Length(img.rows,img.cols),1.0);
  //imshow("Image",img);
  //waitKey(0);
  for (GridMapIterator it(map); !it.isPastEnd(); ++it) {
    Position position;
    map.getPosition(*it, position);
    //cout<<"position"<<position.x()<<","<<position.y()<<"\n";
    int x = (img.rows/2) + position.x()+0.5; 
    int y = (img.cols/2) + position.y()+0.5;
    map.at("elevation", *it) = img.at<uchar>(x,y)<200?10:0;    
  }

  // int index = 0;
  // for(int i = 0; i < img.rows; i++)
  // {
  //   for(int j = 0; j < img.cols;j++)
  //   { 
  //     Position pos(j,i);
  //     Index index;
  //     map.getIndex(pos,index);
  //     map.at("elevation",index) = img.at<uchar>(i,j)<200?10:0;
  //   }
  // }
  
  std::cout<<"Map generated = "<<map.getLength().x()<<map.getLength().y()<<map.getSize();
}
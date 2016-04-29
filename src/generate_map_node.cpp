#include <ros/ros.h>
#include <grid_map_ros/grid_map_ros.hpp>
#include <grid_map_msgs/GridMap.h>
#include <cmath>
#include <iostream>
#include <tf/transform_broadcaster.h>
#include <ReadGridMapFromImage.cpp>
#include "Agent.cpp"
using namespace grid_map;
using namespace std;
using namespace cv;

void populateMap(GridMap&,string,float,float);
void setupTransform(tf::Transform&,GridMap&);

int main(int argc, char** argv)
{
  float scale = 0.5;
  float res = 0.5;
  string file_path = "/home/chirag/test/src/deception_simulation/files/exp_11-sparse.png";

 
  tf::Transform transform;

  ros::init(argc, argv, "grid_map_simple_demo");
  static tf::TransformBroadcaster br;
  ros::NodeHandle nh("~");
  ros::Publisher publisher = nh.advertise<grid_map_msgs::GridMap>("grid_map", 1, true);

  // Create grid map from image at path, scaling factor 'scale' and resolution 'res'.
  GridMap map({"terrain"});
  map.setBasicLayers({"terrain"});
  map.setFrameId("/odom");
  ReadGridMapFromImage::populateMap(map,"terrain",file_path,scale,res);
  
  Agent squirrel(&map,"squirrel");

  ros::Rate rate(1);
  ros::Time startTime = ros::Time::now();
  ros::Duration duration(0.0);
  while (nh.ok()) {
    //ros::Time time = ros::Time::now();
    //duration = time - startTime;
    //const double t = duration.toSec();
    grid_map_msgs::GridMap message;
    squirrel.spawn();
    //Position newPosition = 1* t * Position(cos(t), sin(t));
    //map.setPosition(newPosition);
    setupTransform(transform,map);
    br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "/odom", "/map"));
    GridMapRosConverter::toMessage(map, message);
    publisher.publish(message);
    setupTransform(transform,map);
    ROS_INFO_THROTTLE(1.0, "Grid map (timestamp %f) published.", message.info.header.stamp.toSec());
    rate.sleep();
  }

  return 0;
}

void setupTransform(tf::Transform &transform, GridMap &map)
{
  Position p = map.getPosition();
  transform.setOrigin( tf::Vector3(p.x(),p.y(), 0.0));
  tf::Quaternion q;
  q.setRPY(0, 0, 0);
  transform.setRotation(q);
}
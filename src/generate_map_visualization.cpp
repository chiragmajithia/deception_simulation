#include <ros/ros.h>
#include "grid_map_visualization/GridMapVisualization.hpp"

int main(int argc, char** argv)
{
  ros::init(argc, argv, "generate_grid_map_visualizations");

  ros::NodeHandle nodeHandle("~");

  grid_map_visualization::GridMapVisualization gridMapVisualization(nodeHandle, "grid_map_visualizations");

  ros::spin();
  return 0;
}

#include <ros/ros.h>
#include "Agent.cpp"

using namespace std;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "squirrel_1");

  ros::NodeHandle nh("~");

  Agent squirrel_1(nh,"squirrel_1","/grid_map_node/grid_map");
  ros::Rate rate(10);
  ros::Time startTime = ros::Time::now();
  ros::Duration duration(0.0);
  while(nh.ok())
  {

  	// if (!squirrel_1.isSubscribed_)
  	// {
  	// squirrel_1.updateSubscriptionCallback(ros::TimerEvent());
  	// ros::spin()
  	// }
  	ros::Time time = ros::Time::now();
    duration = time - startTime;
    const double t = duration.toSec();
    Position p = 1* t * Position(cos(t), sin(t));
  	squirrel_1.setPosition(Position(p));
  	squirrel_1.publishTransform();
  }
  //grid_map_visualization::GridMapVisualization gridMapVisualization(nodeHandle, "grid_map_visualizations");

  ros::spin();
  return 0;
}

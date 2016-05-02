#include <grid_map_ros/grid_map_ros.hpp>
#include <grid_map_msgs/GridMap.h>
#include <tf/transform_broadcaster.h>
#include <cmath>
#include <iostream>
#include <ctime>
#include <ros/ros.h>

using namespace std;
using namespace grid_map;
typedef Eigen::Vector2d Position;


class Agent
{
private:
	string name;
	string map_topic;
	tf::Transform transform;
    tf::TransformBroadcaster br;
	struct MapProperties
	{
		float h; //in m
		float w;  // in m
		int rows;	//indx
		int cols;	//indx
		float res;

	} map_prop;


	Position pos;
	ros::NodeHandle& nodeHandle_;
	ros::Subscriber mapSubscriber_;

	
	

	Position generateRandomPosition()
	{
		int max_x = map_prop.rows;
		int max_y = map_prop.cols;
		int res = map_prop.res;
		srand(time(NULL));
		int  x_ = rand() % max_x - (max_x/2);
		int  y_ = rand() % max_y - (max_y/2);
		return Position(x_*res,y_*res);
	}

	void displayProp()
	{
		cout<<map_prop.h<<"\n";
		cout<<map_prop.w<<"\n";
		cout<<map_prop.rows<<"\n";
		cout<<map_prop.cols<<"\n";
		cout<<map_prop.res<<"\n";
	}

	void updateMapProp()
	{
		map_prop.h = map->getLength().y();
		map_prop.w = map->getLength().x();
		map_prop.rows = map->getSize()[0];
		map_prop.cols = map->getSize()[1];
		map_prop.res = map->getResolution();	
	}

public:
	bool isSubscribed_;
	GridMap* map;
	Agent(ros::NodeHandle& nodeHandle,const string name_, const string topic_name) : nodeHandle_(nodeHandle)
	{
		isSubscribed_ = false;
		name = name_;
		map_topic = topic_name;
		ROS_INFO("%s node started",name.c_str());
		initialize();
	}

	~Agent()
	{
		mapSubscriber_.shutdown();
	}

	void initialize()
	{
		if (!isSubscribed_){
			ROS_INFO("In initialize");
			updateSubscriptionCallback(ros::TimerEvent());
			ROS_INFO("Grid map visualization initialized.");
			updateMapProp();
			displayProp();
		}
	}

	void updateSubscriptionCallback(const ros::TimerEvent&)
	{
		ROS_INFO("In updateSubscriptionCallback");

		if (!isSubscribed_) {

			mapSubscriber_ = nodeHandle_.subscribe(map_topic, 1, &Agent::callback, this);
			if(isSubscribed_)
			{
				//isSubscribed_ = true;
				ROS_INFO("Subscribed to grid map at '%s'.", map_topic.c_str());
			}
			else
			{
				ROS_INFO("CANNOT Subscribed to grid map at '%s'.", map_topic.c_str());	
			}
		}
	}

	void callback(const grid_map_msgs::GridMap& message)
	{
		ROS_INFO("In call back %f",map_prop.res);
		std::cout<<"in callback";
		grid_map::GridMap map_;
		grid_map::GridMapRosConverter::fromMessage(message, map_);
		map=&map_;
		updateMapProp();
		displayProp();
		if (map_prop.res>0.0)
		{
			isSubscribed_=true;	
			//mapSubscriber_.shutdown();
		}
	}

	void setPosition(Position p)
	{
		pos = p;
		setTransform(p);
	}

	Position getPosition()
	{
		return pos;
	}

	void spawn()
	{	
		Position p = generateRandomPosition();
		setTransform(p);
	}

	void setTransform(Position p)
	{
		transform.setOrigin( tf::Vector3(pos.x(),pos.y(), 0.0));
		tf::Quaternion q;
		q.setRPY(0, 0, 0);
		transform.setRotation(q);
	}

	void publishTransform()
	{
		//ROS_INFO("Publish Transform");
		br.sendTransform(tf::StampedTransform(transform, ros::Time::now(),"/map", name));
	}
};

/**
	THINGS TO DO:
		Check if subscriber topic exists?
		Wait for topic - if not existing
		Retry subscription
		Callback Rate more than Published 
**/

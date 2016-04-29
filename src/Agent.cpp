#include <grid_map_ros/grid_map_ros.hpp>
#include <grid_map_msgs/GridMap.h>
#include <tf/transform_broadcaster.h>
#include <cmath>
#include <iostream>
#include <ctime>

using namespace std;
using namespace grid_map;
typedef Eigen::Vector2d Position;

//void init(GridMap&);

class Agent
{
private:
	string name;
	Position pos;
	
	Position generateRandomPosition(int max_x,int max_y,double res)
	{
		srand(time(NULL));
		int  x_ = rand() % max_x - (max_x/2);
		int  y_ = rand() % max_y - (max_y/2);
		
		return Position(x_*res,y_*res);
	}
public:
	GridMap* map;
	Agent(GridMap* map_,const string name_)
	{
		map = map_;
		name = name_;
		map->add(name_,0.0);
		map->setTimestamp(ros::Time::now().toNSec());
	}

	~Agent()
	{}

	void setPosition(Position p)
	{
		pos = p;
	}

	Position getPosition()
	{
		return pos;
	}

	void spawn()
	{	
		cout<<"Spawn!";
		grid_map::Size s = map->getSize();
		double res = map->getResolution();
		std::cout<<"size"<<s(0)*res<<","<<s(1)*res<<"\n";
		Position random = generateRandomPosition(s(0),s(1),res);
		std::cout<<"random pos = "<<random(0)<<","<<random(1)<<"\n";
		for (grid_map::CircleIterator iterator(*map,random, 5.0);!iterator.isPastEnd(); ++iterator) 
		{
			map->at(name, *iterator) = 10.0;
		}
		map->setTimestamp(ros::Time::now().toNSec());
	}
};

// void init(GridMap &map)
// {
// 	float scale = 0.5;
//   	float res = 0.5;
//   	string file_path = "/home/chirag/test/src/deception_simulation/files/exp_11-sparse.png";
// 	ReadGridMapFromImage::populateMap(map,"terrain",file_path,scale,res);
// }
// int main(int argc, char** argv)
// {
// 	GridMap map({"terrain"});
//   	map.setBasicLayers({"terrain"});
//   	map.setFrameId("/odom");
//  	init(map);

// 	std::cout<<"Hello World!";
// 	Agent a(map);
// 	a.setPosition(Position(2,3));
// 	a.spawn();
// 	std::cout<<a.getPosition()<<"\n";
// }
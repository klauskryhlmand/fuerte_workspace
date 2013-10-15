/*
 * Pilot.cpp
 *
 *  
 *      Author: Klaus Kryhlmand
 */
#include <iostream>
#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "sensor_msgs/PointCloud.h"
#include <tf/transform_listener.h>
#include <laser_geometry/laser_geometry.h>
#include "beginner_tutorials/laser_info.h"
#include "FroboMsgs/pwm_o.h"

#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "math.h"
#include "vector"

using namespace std;

string laser_info_topic, wheel_speeds_topic;

struct return_msg
{
	bool blocked;
	bool end_of_row;
	double force;
	double carefull;
};


ros::Subscriber laser_sub;
ros::Publisher speed_pub;
void laserInfoCallback(const beginner_tutorials::laser_info ls_inf)
{
	FroboMsgs::pwm_o msg;
	//ROS_INFO("Laser Scan Callback");
	if(ls_inf.blocked)
	{
		//blocked: stop
		msg.speed_left = 0;
		msg.speed_right = 0;
	}
	else if(ls_inf.end_of_row)
	{
		//end of row: turn
		msg.speed_left = 0;
		msg.speed_right = 0;
	}
	else
	{
		//end of row: turn
		msg.speed_left = 20;
		msg.speed_right = 20;

		//left = positive;
		if(ls_inf.force > 0)
		{
			//obstacles to the left;
			msg.speed_right -= ls_inf.force*4;
		}
		else
		{
			//obstacles to the right;
			msg.speed_left += ls_inf.force*4;
		}

		if(ls_inf.carefull*3 < 10)
		{
			msg.speed_left -= ls_inf.carefull*3;
			msg.speed_right -= ls_inf.carefull*3;
		}
		else
		{
			msg.speed_left -= 10;
			msg.speed_right -= 10;
		}

		if(msg.speed_left < 5) msg.speed_left = 5;
		if(msg.speed_right < 5) msg.speed_right = 5;
	}
	msg.direction = 5;
	speed_pub.publish(msg);
}


int main(int argc, char** argv){
	// Initialize ros
	ros::init(argc, argv, "AI");

	ros::NodeHandle n = ros::NodeHandle();
	ros::NodeHandle nh = ros::NodeHandle("~");
	// ROS PARAMETERS
	nh.param<string>("object_topic", laser_info_topic, "object_topic");
	nh.param<string>("wheel_speeds_topic", wheel_speeds_topic, "wheel_speeds_topic");

	// ROS PUBLISHERS AND SUBSCRIBERS
	laser_sub = n.subscribe<beginner_tutorials::laser_info>(laser_info_topic.c_str(), 1, &laserInfoCallback);
	speed_pub = n.advertise<FroboMsgs::pwm_o>(wheel_speeds_topic.c_str(), 1);


	// Spin
	ros::spin();
	return 0;
}

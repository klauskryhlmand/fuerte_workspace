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

#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "math.h"
#include "vector"

using namespace std;

string laser_scan_topic, laser_info_topic;
int show_image;


struct return_msg
{
	bool blocked;
	bool end_of_row;
	double force;
	double carefull;
};


ros::Subscriber laser_subscriber;
ros::Publisher laser_pub;

void laserScanCallback(const sensor_msgs::LaserScan::ConstPtr& laser_scan)
{
	//ROS_INFO("Laser Scan Callback");
	
	// Create PointCloud from laser scan
	sensor_msgs::PointCloud cloud;
	laser_geometry::LaserProjection projector;
	projector.projectLaser(*laser_scan, cloud);
	
	int size = cloud.points.size();
	
	return_msg laser_info;	
	laser_info.blocked = false;
	laser_info.end_of_row = false;
	laser_info.force = 0.0;
	laser_info.carefull = 0.0;
	double force_down = 0;
	double force_up = 0;
	
	int blockers = 0;
	double dist;
	// create img data from cloud data

	double max_hole = 0;
	double hole = 0;
	double angle = 0;
	double temp_angle = 0;
	double temp_angle2 = 0;
	for(int i = 0; i < laser_scan->ranges.size(); i++)
	{
		if(laser_scan->ranges[i] > 0.5)
		{
			hole++;
		}
		else if(hole*270.0/laser_scan->ranges.size() > 35.0)
		{
			temp_angle = (i - hole/2.0)*270/laser_scan->ranges.size();
			temp_angle = temp_angle - 135;

			if(hole*270.0/laser_scan->ranges.size() > 88.0)
			{
				temp_angle2 = (i - 44.0)*270/laser_scan->ranges.size();
				temp_angle2 = temp_angle2 - 135;
				if(abs(temp_angle2) < abs(temp_angle))
				{
					temp_angle = temp_angle2;
				}

				temp_angle2 = (i - hole + 44.0)*270/laser_scan->ranges.size();
				temp_angle2 = temp_angle2 - 135;
				if(abs(temp_angle2) < abs(temp_angle))
				{
					temp_angle = temp_angle2;
				}
			}

			if(abs(temp_angle) < abs(angle))
			{
				max_hole = hole;
				angle = temp_angle;
			}
			hole = 0;
		}
		else
		{
			hole = 0;
		}
	}
	laser_info.force = -angle;
	if(max_hole*270.0/laser_scan->ranges.size() > 180.0)
	{
		laser_info.end_of_row = true;
	}

	if(max_hole == 0)
	{
		laser_info.blocked = true;
	}

	for (int i = 0; i < size; i++)
	{
		//blocking point
//		if(abs((cloud.points[i].y))< 0.25 && cloud.points[i].x > 0 && cloud.points[i].x < (0.25 - abs((cloud.points[i].y))))
//		{
//			blockers++;
//			if(blockers > 3)
//			{
//				laser_info.blocked = true;
//				break;
//			}
//		}
		if (cloud.points[i].x > 0.1 && cloud.points[i].x < 0.6 && abs((cloud.points[i].y))<0.75)
		{
//			for(int i = 0; i < size; i++)
//
//			dist = sqrt(cloud.points[i].y*cloud.points[i].y  +  cloud.points[i].x*cloud.points[i].x);
//			if((-cloud.points[i].y) < force_down)
//			{
//				force_down = (-cloud.points[i].y);
//			}
//			if((-cloud.points[i].y) > force_up)
//			{
//				force_up = (-cloud.points[i].y);
//			}
//
			if(1 - cloud.points[i].x - abs(cloud.points[i].y)  > laser_info.carefull)
			{
				laser_info.carefull = 1 - cloud.points[i].x - abs(cloud.points[i].y);
				if(laser_info.carefull < 0)
				{
					laser_info.carefull = 0;
				}
			}
		}
	}
	
	//publish data
	beginner_tutorials::laser_info msg;
	msg.blocked = laser_info.blocked;
	msg.end_of_row = laser_info.end_of_row;
	msg.force = laser_info.force;
	msg.carefull = laser_info.carefull;

	laser_pub.publish(msg);

	//show data
	if(show_image) {}
}


int main(int argc, char** argv){
	// Initialize ros
	ros::init(argc, argv, "planner");

	ros::NodeHandle n = ros::NodeHandle();
	ros::NodeHandle nh = ros::NodeHandle("~");
	// ROS PARAMETERS
	nh.param<string>("laser_scan_topic", laser_scan_topic, "laser_scan_topic");
	nh.param<int>("show_image", show_image, 1);
	nh.param<string>("object_topic", laser_info_topic, "object_topic");

	// ROS PUBLISHERS AND SUBSCRIBERS
	laser_subscriber = n.subscribe<sensor_msgs::LaserScan>(laser_scan_topic.c_str(), 1, &laserScanCallback);
	laser_pub = n.advertise<beginner_tutorials::laser_info>(laser_info_topic.c_str(), 1);


	// Spin
	ros::spin();
	return 0;
}

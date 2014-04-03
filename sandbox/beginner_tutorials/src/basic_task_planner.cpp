/**
 *	Project Name:     FREFrobo basic task controler
 *	File name:        basic_task_planner.cpp
 *	Creation date:    Thu Apr  3 12:29:58 CEST 2014
 *	Author:           Troels Bo Jørgensen and Daniel Lindekilde Ravn
 *	SDU email:        darav08@student.sdu.dk
 *	gmail:            daniel.ravn43@gmail.com
**/
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

	double close_hole = 0;
	double close_hole_dist = 0.13;
	double close_hole_angle = 110;
	//double close_angle = 180;

	double max_hole = 0;
	double hole = 0;
	double hole_angle = 20;
	double angle = 180;
	double temp_angle = 0;
	double temp_angle2 = 0;
	double view_interest = 1;

	double highest = 0;
	double lowest = 100000;
	for(int i = 0; i < laser_scan->ranges.size(); i++)
	{
		if(laser_scan->ranges[i] > highest)
		{
			highest = laser_scan->ranges[i];
		}
		if(laser_scan->ranges[i] < lowest)
		{
			lowest = laser_scan->ranges[i];
		}

		//determine close hole size
		if(laser_scan->ranges[i] > close_hole_dist)
		{
			close_hole++;
		}

		//determine hole size
		if(laser_scan->ranges[i] > view_interest)
		{
			hole++;
		}
		//end of hole determine how good it is
		if((laser_scan->ranges[i] <= view_interest || i == laser_scan->ranges.size()-1) 
&& hole*270.0/laser_scan->ranges.size() > hole_angle
 && close_hole*270.0/laser_scan->ranges.size() > close_hole_angle)
		{
			//calculate center of the hole
			temp_angle = (i - hole/2.0)*270/laser_scan->ranges.size();
			temp_angle = temp_angle - 135;
			
			//if the hole is really big follow a side instead of center
			if(hole*270.0/laser_scan->ranges.size() > 30.0)
			{
				//double left_dist = 100;
				//double right_dist = 100;
//
//				if( i > laser_scan->ranges.size()*3/6)
//				left_dist = laser_scan->ranges[i]*cos(i*Pi*3.0/2.0/laser_scan->ranges.size()-Pi/2)				
//				right_dist = laser_scan->ranges[i-hole-1]*cos(i*Pi*3.0/2.0/laser_scan->ranges.size())				
				temp_angle2 = (i - 15)*270/laser_scan->ranges.size();
				temp_angle2 = temp_angle2 - 135;
				if(abs(temp_angle2) < abs(temp_angle))
				{
					temp_angle = temp_angle2;
				}
				temp_angle2 = (i - hole + 15)*270/laser_scan->ranges.size();
				temp_angle2 = temp_angle2 - 135;
				if(temp_angle2 * temp_angle < 0)
				{
					temp_angle = 0;
				}					
				else if(abs(temp_angle2) < abs(temp_angle))
				{
					temp_angle = temp_angle2;
				}
			}
			
			//if the hole is more straight than previous holes chose that one
			if(abs(temp_angle) < abs(angle))
			{
				max_hole = hole;
				angle = temp_angle;
			}
		}

		//reset holes
		if(laser_scan->ranges[i] <= view_interest)
		{
			hole = 0;
		}
		if(laser_scan->ranges[i] <= close_hole_dist)
		{
			close_hole = 0;
		}

	}
	laser_info.force = -angle/30;
	if(laser_info.force > 1) laser_info.force = 1;
	if(laser_info.force < -1) laser_info.force = -1;
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
		if (cloud.points[i].x > 0.1 && cloud.points[i].x < 0.600 && abs((cloud.points[i].y))< 0.750)
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
			if(1.000 - cloud.points[i].x - abs(cloud.points[i].y)  > laser_info.carefull)
			{
				laser_info.carefull = (1.000 - cloud.points[i].x - abs(cloud.points[i].y))/1.000;
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

	ROS_INFO("force: %f", msg.force);
	ROS_INFO("careful: %f", msg.carefull);
	ROS_INFO("endrow: %d", msg.end_of_row);
	ROS_INFO("blocked: %d", msg.blocked);
	ROS_INFO("highest: %f", highest);
	ROS_INFO("lowest: %f", lowest);
	ROS_INFO("angle: %f", angle);

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

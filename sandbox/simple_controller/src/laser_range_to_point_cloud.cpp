/**
 *	Project Name:     simple_controler
 *	File name:        laser_range_to_point_cloud.cpp
 *	Creation date:    Thu Apr 10 14:34:38 CEST 2014
 *	Author:           Daniel Lindekilde Ravn
 *	SDU email:        darav08@student.sdu.dk
 *	gmail:            daniel.ravn43@gmail.com
**/

#include <iostream>
#include <string>
#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "sensor_msgs/PointCloud.h"
#include <tf/transform_listener.h>
#include <laser_geometry/laser_geometry.h>
#include "FroboMsgs/point_cloud.h"
#include "vector"

using namespace std;

ros::Publisher cordinats_publisher;

void laserScanCallback(const sensor_msgs::LaserScan::ConstPtr& laser_scan){
	// Create PointCloud from laser scan
	sensor_msgs::PointCloud cloud;
	laser_geometry::LaserProjection projector;
	projector.projectLaser(*laser_scan, cloud);

	cordinats_publisher.publish(cloud);
}

int main(int argc, char** argv){
	// Initialize ros
	ros::init(argc, argv, "Laser_range2point_cloud");

	// ROS NodeHandles
	ros::NodeHandle n = ros::NodeHandle();
	ros::NodeHandle nh = ros::NodeHandle("~");
	// ROS PARAMETERS
	
	std::string laser_scan_topic = "";
	std::string point_cloud_topic = "";
	
	nh.param<string>("laser_scan_topic", laser_scan_topic, "/frobo/laser_msgs_1");
	nh.param<string>("point_cloud_topic", point_cloud_topic, "/frobo/cordinats");

	// ROS PUBLISHERS AND SUBSCRIBERS
	ros::Subscriber laser_subscriber = n.subscribe<sensor_msgs::LaserScan>(laser_scan_topic.c_str(), 1, &laserScanCallback);
	cordinats_publisher = n.advertise<sensor_msgs::PointCloud>(point_cloud_topic.c_str(), 1);


	ros::spin();
	return 0;
}

/*
 * SimpleObjectAvoidance.h
 *
 *  Created on: Apr 20, 2012
 *      Author: Morten Ege Jensen
 */

#ifndef PILOT_H_
#define PILOT_H_
#include <iostream>
#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "sensor_msgs/PointCloud.h"
#include <tf/transform_listener.h>
#include <laser_geometry/laser_geometry.h>
//#include "laser_geometry/laser_geometry.h"
//#include "/opt/ros/fuerte/stacks/laser_pipeline/laser_geometry/include/laser_geometry/laser_geometry.h"
#include "FroboMsgs/detected_objects.h"
#include "FroboMsgs/object_row.h"
#include "FroboMsgs/float_data.h"

// OpenCv includes
//#include <cv_bridge/cv_bridge.h>
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "math.h"
#include "vector"

using namespace std;

class Pilot {
private:
	IplImage* raw_img;
	IplImage* grey_img;
	// Image parameters
	const char* framedWindowName;

	// ROS
	// Nodehandle
	ros::NodeHandle n;
	ros::NodeHandle nh;
	// Subscribers & publishers
	ros::Subscriber laser_subscriber;
	ros::Publisher object_publisher;
	ros::Publisher object_row_publisher;
	//::Subscriber wheel_speeds_subscriber;
	
	// msgs
	FroboMsgs::detected_objects object_msg;
	FroboMsgs::object_row object_row_msg;
	
	// Parameters
	string laser_scan_topic, object_topic, object_row_topic, wheel_speeds_topic;
	int show_image;
	double robot_clearence_width, robot_stop_zone, robot_turn_zone, robot_turn_zone_extra_width;
	
	// Rows of boxes
	double row_box_start_value, row_box_width, row_box_height, row_individual_box_height;
	int row_box_count;
	
	// Callbacks
	void laserScanCallback(const sensor_msgs::LaserScan::ConstPtr& laser_scan);
	//void wheelCallback(const fmMsgs::float_data::ConstPtr& speeds);
	
	// Wheel speeds
	//int new_speeds;
	//double wheel_speed_right, wheel_speed_left;
	
	// Detected rows
	 vector<int> left_row;
	 vector<int> right_row;
public:
	// Constructor & Destructor
	Pilot();
	//virtual ~Pilot();

};

#endif /* PILOT_H_ */

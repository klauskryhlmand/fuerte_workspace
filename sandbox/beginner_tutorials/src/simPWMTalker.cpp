/*
 * turningCal.cpp
 *
 *  Created on: Oct 8, 2013
 *      Author: Daniel Lindekilde Ravn
 */

#include <iostream>
#include <ros/ros.h>
#include <FroboMsgs/pwm_o.h>



int main(int argc, char **argv) {
	ros::init(argc, argv, "simPWMtest");
	ROS_INFO("simPWMtest node started");
	ros::NodeHandle n, nh("~");

	FroboMsgs::pwm_o_ lol;

	lol.direction = 0;
	lol.direction_left = 0;
	lol.direction_right = 0;
	lol.enable_left = 1;
	lol.enable_right = 2;
	lol.speed_left = 50;
	lol.speed_right = 25;

	std::string pwmTopic;

	nh.param<std::string>("pwm", pwmTopic, "/pwm_o");

	ros::Publisher control_pub;

	control_pub.Publisher(lol);

	ROS_INFO("kinematics_node ready");
	ros::spin();
	return 0;
}




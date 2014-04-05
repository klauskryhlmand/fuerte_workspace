/**
 *	Project Name:     FREFrobo basic task controler
 *	File name:        basic_task_AI.cpp
 *	Creation date:    Thu Apr 3 12:28:11 CEST 2014
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
#include "FroboMsgs/pwm_micro.h"
#include "FroboMsgs/fpga_data.h"

#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "math.h"
#include "vector"

using namespace std;

string laser_info_topic, wheel_speeds_topic;
const double Max_speed = 2;//0.0034;
const double Max_speed_rot = 0.6;//0.0034;
const double forward_turn_ticks = 0.5;
const double sharp_turn_ticks = 1.8;
const double sharp_turn_ratio = 0.3;
const double turn180_ticks = 1700;

struct return_msg
{
	bool blocked;
	bool end_of_row;
	double force;
	double carefull;
};

//R = right, L = left, O = turn_back, F = finished
struct turn_act
{
	char action;
	int skips;
};
vector<turn_act> end_of_row_act;
int eoraPTR = 0;

enum gen_states {drive_in_line, drive_at_end_left, drive_at_end_right, drive_in_left, drive_in_right, avoid_obst, finished};
enum turn_states {Init, Drive_forward, Turn_fast, Tune_turn};
turn_states turn_state = Init;
gen_states state = drive_in_line;

bool turning = false;
double encL_old = 0;
double encR_old = 0;
double encL = 0;
double encR = 0;
bool turn_left;
void turn(FroboMsgs::pwm_micro &msg)
{
	switch (turn_state)
	{
		case Init:
			//save encoder val
			encL_old = encL;
			encR_old = encR;
			msg.speed_left = Max_speed_rot;
			msg.speed_right = Max_speed_rot;

			turning = true;
			turn_state = Drive_forward;

			if(end_of_row_act[eoraPTR].action == 'F')
			{
				ROS_WARN("completed task");
				turn_state = Init;
				msg.speed_left = 0;
				msg.speed_right = 0;
			}
			break;

		case Drive_forward:
			msg.speed_left = Max_speed_rot;
			msg.speed_right = Max_speed_rot;
			if(encL-encL_old > forward_turn_ticks && encL-encL_old < 100)
			{
				msg.speed_left = 0;
			}

			if(encR - encR_old > forward_turn_ticks && encR - encR_old < 100)
			{
				msg.speed_right = 0;
			}

			if(msg.speed_left == 0 || msg.speed_right == 0)
			{
				turn_state = Turn_fast;
				//remember enc_values
				encL_old = encL;
				encR_old = encR;
			}
			break;

		case Turn_fast:
			if(end_of_row_act[eoraPTR].action == 'R')
			{
				msg.speed_left = Max_speed_rot;
				msg.speed_right = msg.speed_left * sharp_turn_ratio;
				if(encL - encL_old > sharp_turn_ticks && encL - encL_old < 100)
				{
					turn_state = Tune_turn;
					msg.speed_left = 0;
				}
				if(encR - encR_old > sharp_turn_ticks * sharp_turn_ratio && encR - encR_old < 100)
				{
					turn_state = Tune_turn;
					msg.speed_right = 0;
				}
			}
			else if(end_of_row_act[eoraPTR].action == 'L')
			{
				msg.speed_right = Max_speed_rot;
				msg.speed_left = msg.speed_right * sharp_turn_ratio;
				if(encR - encR_old > sharp_turn_ticks && encR - encR_old < 100)
				{
					turn_state = Tune_turn;
					msg.speed_right = 0;
				}
				if(encL - encL_old > sharp_turn_ticks * sharp_turn_ratio && encL - encL_old < 100)
				{
					turn_state = Tune_turn;
					msg.speed_left = 0;
				}
			}
			else if(end_of_row_act[eoraPTR].action == 'O')
			{
				msg.speed_left = Max_speed/2;
				msg.speed_right = Max_speed/2;
				msg.direction_right = 0;
				msg.direction_left = 1;
				if(encR_old - encR > turn180_ticks && encR_old - encR < 100)
				{
					msg.speed_right = 0;
				}
				if(encL - encL_old > turn180_ticks && encL - encL_old < 100)
				{
					msg.speed_left = 0;
				}
				if(msg.speed_left == 0 && msg.speed_right == 0)
				{
					turn_state = Tune_turn;
				}
			}
			else
			{
				msg.speed_left = 0;
				msg.speed_right = 0;
				ROS_ERROR("finished");
			}
			break;

		case Tune_turn:
			//turn complete
			turn_state = Init;
			turning = false;
			eoraPTR++;
			break;

		default:
			break;
	}
}

ros::Subscriber laser_sub;
ros::Subscriber enc_sub;
ros::Publisher speed_pub;
void laserInfoCallback(const beginner_tutorials::laser_info ls_inf)
{
	FroboMsgs::pwm_micro msg;
	msg.direction_left = 1;
	msg.direction_right = 1;
	msg.enable_left = 1;
	msg.enable_right = 1;
	msg.speed_left = 0;
	msg.speed_right = 0;

	switch (state)
	{
		case drive_in_line:
/**********************************************************************************************
Switch state logic
***********************************************************************************************/
//			if(ls_inf.blocked)
//			{
//				//blocked: stop
//				msg.speed_left = 0;
//				msg.speed_right = 0;
//				state = avoid_obst;
//				ROS_WARN("state: avoid_obst");
//				break;
//			}
			if(false){}
			else if(ls_inf.end_of_row)
			{
				//end of row: turn
				msg.speed_left = 0;
				msg.speed_right = 0;
				if(end_of_row_act[eoraPTR].action == 'F')
				{
					state = finished;
					ROS_WARN("state: finished");
					break;
				}
				else if(end_of_row_act[eoraPTR].action == 'O')
				{
					state = drive_at_end_right;
					ROS_WARN("state: drive_at_end_right");
					break;
				}
				else if(end_of_row_act[eoraPTR].action == 'R')
				{
					state = drive_at_end_right;
					ROS_WARN("state: drive_at_end_right");
					break;
				}
				else if(end_of_row_act[eoraPTR].action == 'L')
				{
					state = drive_at_end_left;
					ROS_WARN("state: drive_at_end_left");
					break;
				}
			}
			else
			{
///**********************************************************************************************
//Drive in the line
//***********************************************************************************************/
				msg.speed_left = Max_speed;
				msg.speed_right = Max_speed;

				//left = positive;
				if(ls_inf.force > 0)
				{
					
					//obstacles to the left;
					msg.speed_left = Max_speed - ls_inf.force*Max_speed;
				}
				else
				{
					//obstacles to the right;
					msg.speed_right = Max_speed + ls_inf.force*Max_speed;
				}

				if(ls_inf.carefull+1 < 2)
				{
					msg.speed_left = msg.speed_left/(ls_inf.carefull + 1.0);
					msg.speed_right = msg.speed_right/(ls_inf.carefull + 1.0);
				}
				else
				{
					msg.speed_left = msg.speed_left/2;
					msg.speed_right = msg.speed_right/2;
				}

				if(msg.speed_left < Max_speed/4) msg.speed_left = Max_speed/4;
				if(msg.speed_right < Max_speed/4) msg.speed_right = Max_speed/4;

				ROS_ERROR("state: row drive");
				ROS_ERROR("left speed: %f", msg.speed_left);
				ROS_ERROR("right speed: %f", msg.speed_right);
			}
			break;



		case drive_at_end_left:
///**********************************************************************************************
//temporary Switch state logic
//***********************************************************************************************/
//			if(ls_inf.blocked)
//			{
//				//blocked: stop
//				msg.speed_left = 0;
//				msg.speed_right = 0;
//				state = avoid_obst;
//				ROS_WARN("state: avoid_obst");
//				break;
//			}
/**********************************************************************************************
Drive at end of line left
***********************************************************************************************/
			//msg.speed_left = Max_speed;
			//msg.speed_right = Max_speed;
//
//			if(ls_inf.carefull > 3)
//			{
//				msg.speed_left = msg.speed_left/(ls_inf.carefull + 1.0)/3.0;
//				msg.speed_right = msg.speed_right/(ls_inf.carefull + 1.0);
//			}
//			else
//			{
//				msg.speed_left = msg.speed_left/12.0;
//				msg.speed_right = msg.speed_right/4.0;
//			}
//
//			if(msg.speed_left < Max_speed/32.0) msg.speed_left = Max_speed/32.0;
//			if(msg.speed_right < Max_speed/8.0) msg.speed_right = Max_speed/8.0;
//			msg.speed_left = 0;
//
//			//switch state logic
//			if(!ls_inf.end_of_row && ls_inf.force < -0.8)
//			{
//				msg.speed_left = 0;
//				msg.speed_right = 0;
//				state = drive_in_line;
//				ROS_WARN("state: drive in line");
//			}
			turn(msg);
			if(!turning) state = drive_in_line;

			ROS_ERROR("state: turn left");
			ROS_ERROR("left speed: %f", msg.speed_left);
			ROS_ERROR("right speed: %f", msg.speed_right);

			//msg.speed_left = 0;
			//msg.speed_right = 0;
			break;




		case drive_at_end_right:
/**********************************************************************************************
Drive at end of line right
***********************************************************************************************/
//			msg.speed_left = Max_speed;
//			msg.speed_right = Max_speed;
//
//			if(ls_inf.carefull > 3)
//			{
//				msg.speed_left = msg.speed_left/(ls_inf.carefull + 1.0);
//				msg.speed_right = msg.speed_right/(ls_inf.carefull + 1.0)/3.0;
//			}
//			else
//			{
//				msg.speed_left = msg.speed_left/4.0;
//				msg.speed_right = msg.speed_right/12.0;
//			}
//
//			if(msg.speed_left < Max_speed/8.0) msg.speed_left = Max_speed/8.0;
//			if(msg.speed_right < Max_speed/32.0) msg.speed_right = Max_speed/32.0;
//			msg.speed_right = 0;
//
//			//switch state logic
//			if(!ls_inf.end_of_row && ls_inf.force > 0.8)
//			{
//				msg.speed_left = 0;
//				msg.speed_right = 0;
//				state = drive_in_line;
//				ROS_WARN("state: drive in line");
//			}
			turn(msg);

			if(!turning) state = drive_in_line;

			ROS_ERROR("state: turn right");
			ROS_ERROR("left speed: %f", msg.speed_left);
			ROS_ERROR("right speed: %f", msg.speed_right);

			//msg.speed_left = 0;
			//msg.speed_right = 0;
			break;

		case drive_in_left:
/**********************************************************************************************
temporary Switch state logic
***********************************************************************************************/
			if(ls_inf.blocked)
			{
				//blocked: stop
				msg.speed_left = 0;
				msg.speed_right = 0;
				state = avoid_obst;
				ROS_WARN("state: avoid_obst");
				break;
			}
			else if(!ls_inf.end_of_row)
			{
				msg.speed_left = 0;
				msg.speed_right = 0;
				state = drive_in_line;
				ROS_WARN("state: drive in line");
			}


			//msg.speed_left = 0;
			//msg.speed_right = 0;
			break;

		case drive_in_right:
/**********************************************************************************************
temporary Switch state logic
***********************************************************************************************/
			if(ls_inf.blocked)
			{
				//blocked: stop
				msg.speed_left = 0;
				msg.speed_right = 0;
				state = avoid_obst;
				ROS_WARN("state: avoid_obst");
				break;
			}
			else if(!ls_inf.end_of_row)
			{
				msg.speed_left = 0;
				msg.speed_right = 0;
				state = drive_in_line;
				ROS_WARN("state: drive in line");
			}
			msg.speed_left = 0;
			msg.speed_right = 0;
			break;

		case avoid_obst:
/**********************************************************************************************
temporary Switch state logic
***********************************************************************************************/
			if(!ls_inf.end_of_row && !ls_inf.blocked)
			{
				msg.speed_left = 0;
				msg.speed_right = 0;
				state = drive_in_line;
				ROS_WARN("state: drive in line");
			}


			msg.speed_left = 0;
			msg.speed_right = 0;
			break;

		case finished:
			ROS_ERROR("state: finished");
			msg.speed_left = 0;
			msg.speed_right = 0;
			break;
		
		default:
			ROS_ERROR("state: default WTF!!!!");
			break;
	}
	speed_pub.publish(msg);
}

void encInfoCallback(const FroboMsgs::micro_data enc_inf)
{
	encL = enc_inf.encoder_l;
	encR = enc_inf.encoder_r;

	ROS_ERROR("enc l: %f", enc_inf.encoder_l);
	ROS_ERROR("enc r: %f", enc_inf.encoder_r);
}



int main(int argc, char** argv){
	//Get actions
	for(int i = 0; i < 60; i += 6)
	{
		end_of_row_act.push_back(turn_act());
		end_of_row_act[i].action = 'L';
		end_of_row_act.push_back(turn_act());
		end_of_row_act[i+1].action = 'R';
		end_of_row_act.push_back(turn_act());
		end_of_row_act[i+2].action = 'L';
		end_of_row_act.push_back(turn_act());
		end_of_row_act[i+3].action = 'L';
		end_of_row_act.push_back(turn_act());
		end_of_row_act[i+4].action = 'R';
		end_of_row_act.push_back(turn_act());
		end_of_row_act[i+5].action = 'L';
	}
	end_of_row_act.push_back(turn_act());
	end_of_row_act[60].action = 'F';
	eoraPTR = 0;

	// Initialize ros
	ros::init(argc, argv, "AI");

	ros::NodeHandle n = ros::NodeHandle();
	ros::NodeHandle nh = ros::NodeHandle("~");
	// ROS PARAMETERS
	nh.param<string>("object_topic", laser_info_topic, "object_topic");
	nh.param<string>("wheel_speeds_topic", wheel_speeds_topic, "pwm");

	// ROS PUBLISHERS AND SUBSCRIBERS
	laser_sub = n.subscribe<beginner_tutorials::laser_info>(laser_info_topic.c_str(), 1, &laserInfoCallback);
	enc_sub = n.subscribe<FroboMsgs::micro_data>("distance", 2, &encInfoCallback);
	speed_pub = n.advertise<FroboMsgs::pwm_micro>(wheel_speeds_topic.c_str(), 1);


	// Spin
	ros::spin();
	return 0;
}

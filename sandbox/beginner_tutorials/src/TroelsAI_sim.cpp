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
#include "FroboMsgs/fpga_data.h"

#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "math.h"
#include "vector"

using namespace std;

string laser_info_topic, wheel_speeds_topic;
const double Max_speed = 0.1;//0.0034;
const int forward_turn_ticks = 500;
const int sharp_turn_ticks = 1500;
const int turn180_ticks = 1700;

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
unsigned int encL_old;
unsigned int encR_old;
unsigned int encL;
unsigned int encR;
bool turn_left;
void turn(FroboMsgs::pwm_o &msg)
{
	switch (turn_state)
	{
		case Init:
			//save encoder val
			encL_old = encL;
			encR_old = encR;
			msg.speed_left = Max_speed/2;
			msg.speed_right = Max_speed/2;

			turn_left = 10;
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
			msg.speed_left = Max_speed/2;
			msg.speed_right = Max_speed/2;
			if(encL-encL_old > forward_turn_ticks && encL-encL_old < 1000000)
			{
				msg.speed_left = 0;
			}

			if(encR - encR_old > forward_turn_ticks && encR - encR_old < 1000000)
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
				msg.speed_left = Max_speed;
				msg.speed_right = 0;
				if(encL - encL_old > sharp_turn_ticks && encL - encL_old < 1000000)
				{
					turn_state = Tune_turn;
					msg.speed_left = 0;
				}
			}
			else if(end_of_row_act[eoraPTR].action == 'L')
			{
				msg.speed_right = Max_speed;
				msg.speed_left = 0;
				if(encR - encR_old > sharp_turn_ticks && encR - encR_old < 1000000)
				{
					turn_state = Tune_turn;
					msg.speed_right = 0;
				}
			}
			else if(end_of_row_act[eoraPTR].action == 'O')
			{
				msg.speed_left = Max_speed/2;
				msg.speed_right = Max_speed/2;
				msg.direction = 4;
				if(encR_old - encR > turn180_ticks && encR_old - encR < 1000000)
				{
					msg.speed_right = 0;
				}
				if(encL - encL_old > turn180_ticks && encL - encL_old < 1000000)
				{
					msg.speed_left = 0;
				}
				if(msg.speed_left == 0 && msg.speed_right == 0)
				{
					turn_state = Tune_turn;
				}
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
	FroboMsgs::pwm_o msg;
	msg.direction = 5;
	msg.enable_left = 0;
	msg.enable_right = 0;


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
//			else if(ls_inf.end_of_row)
//			{
//				//end of row: turn
//				msg.speed_left = 0;
//				msg.speed_right = 0;
//				if(end_of_row_act[eoraPTR].action == 'F')
//				{
//					state = finished;
//					ROS_WARN("state: finished");
//					break;
//				}
//				else if(end_of_row_act[eoraPTR].action == 'O')
//				{
//					state = drive_at_end_right;
//					ROS_WARN("state: drive_at_end_right");
//					break;
//				}
//				else if(end_of_row_act[eoraPTR].action == 'R')
//				{
//					state = drive_at_end_right;
//					ROS_WARN("state: drive_at_end_right");
//					break;
//				}
//				else if(end_of_row_act[eoraPTR].action == 'L')
//				{
//					state = drive_at_end_left;
//					ROS_WARN("state: drive_at_end_left");
//					break;
//				}
//			}
			if(false){}
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

				ROS_WARN("state: row drive");
				ROS_WARN("left speed: %f", msg.speed_left);
				ROS_WARN("right speed: %f", msg.speed_right);
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
//			else if(!ls_inf.end_of_row)
//			{
//				msg.speed_left = 0;
//				msg.speed_right = 0;
//				state = drive_in_line;
//				ROS_WARN("state: drive in line");
//			}

/**********************************************************************************************
Drive at end of line left
***********************************************************************************************/
			msg.speed_left = Max_speed;
			msg.speed_right = Max_speed;

			//left = positive;
			if(ls_inf.end_of_row || ls_inf.force > 0)
			{
				msg.speed_left = 0;
			}
			else
			{
				//obstacles to the right;
				msg.speed_right += ls_inf.force*Max_speed*2;
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

			ROS_WARN("state: row drive");
			ROS_WARN("left speed: %d", msg.speed_left);
			ROS_WARN("right speed: %d", msg.speed_right);

			msg.speed_left = 0;
			msg.speed_right = 0;
			break;

		case drive_at_end_right:
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


			msg.speed_left = 0;
			msg.speed_right = 0;
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

		default:
			break;
	}
	speed_pub.publish(msg);
}

void encInfoCallback(const FroboMsgs::fpga_data enc_inf)
{
	encL = enc_inf.encoder_l;
	encR = enc_inf.encoder_r;
}



int main(int argc, char** argv){
	//Get actions
	for(int i = 0; i < 20; i += 2)
	{
		end_of_row_act.push_back(turn_act());
		end_of_row_act[i].action = 'O';
		end_of_row_act.push_back(turn_act());
		end_of_row_act[i+1].action = 'O';
	}
	end_of_row_act.push_back(turn_act());
	end_of_row_act[20].action = 'F';
	eoraPTR = 0;

	// Initialize ros
	ros::init(argc, argv, "AI");

	ros::NodeHandle n = ros::NodeHandle();
	ros::NodeHandle nh = ros::NodeHandle("~");
	// ROS PARAMETERS
	nh.param<string>("object_topic", laser_info_topic, "object_topic");
	nh.param<string>("wheel_speeds_topic", wheel_speeds_topic, "wheel_speeds_topic");

	// ROS PUBLISHERS AND SUBSCRIBERS
	laser_sub = n.subscribe<beginner_tutorials::laser_info>(laser_info_topic.c_str(), 1, &laserInfoCallback);
	enc_sub = n.subscribe<FroboMsgs::fpga_data>("encoder_l", 1, &encInfoCallback);
	speed_pub = n.advertise<FroboMsgs::pwm_o>(wheel_speeds_topic.c_str(), 1);


	// Spin
	ros::spin();
	return 0;
}

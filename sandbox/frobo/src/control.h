/*
 * PotDecision.h
 *
 *  Created on: Apr 16, 2012
 *      Author: Nis Sarup
 */

#ifndef CONTROL_H_
#define CONTROL_H_
#include "ros/ros.h"
#include "FroboMsgs/detected_objects.h"
#include "math.h"
#include "pid.h"
#include "FroboMsgs/object_row.h"
#include "FroboMsgs/state.h"
//#include "fmMsgs/wii_state.h"
#include "vector"
#include "std_msgs/Int8.h"
//#include "FroboMsgs/pwm_o.h"
#include "FroboMsgs/micro_data.h"
#include "FroboMsgs/pwm_micro.h"
	
	 
using namespace std;

class Control {
private:
	
	int speed_resolution;
	int speed_desired_left;
	int speed_desired_right;
	int direction;
	double error;
	
	// New Data flags
	// Remember to initialize to zero in the constructor
	int new_speeds, new_gyro, new_l_row, new_r_row, new_object_row, new_object_message_received;
	
	double wheel_speed_right, wheel_speed_left;
	double gyro_z;
	
	// Row data
	double rightdistance, rightangle;
	double leftdistance, leftangle;
	
	// Navigation data
	double cross_track_error;
	int next_turn_direction;
	
	// Object data
	int new_left_object, new_right_object, new_stop;
	double object_left_distance, object_right_distance, object_left_angle, object_right_angle;
	
	// Object Row Data
	int end_l, end_r;
	vector<int> object_row_left;
	vector<int> object_row_right;
	double object_row_resolution;
	double object_row_start_position;
	double object_row_end_position_left;
	double object_row_end_position_right;
	double object_row_fill_percent_left;
	double object_row_fill_percent_right;
	ros::Time object_row_data_last_update;
	int row_state;
	//row_hole_finder left_row_finder, right_row_finder;
		
public:
	Control();
	virtual ~Control();
	
	// Parameters
	int end_of_row;
	double linear_mean_velocity, mean_driving_distance_from_rows;
	double cte_weight_angle, cte_weight_distance;
	double base_link_radius_to_wheels;
	double time_s;
	double cte_kp, cte_ki, cte_kd;
	int object_row_box_filled_threshold;
	double object_row_threshold;
	double dead_reckoning_turn_rate;
	FroboMsgs::pwm_micro control;
//	FroboMsgs::state stateMsg;
	// Functions
	//void rowCallback(const fmMsgs::row::ConstPtr& row);
	//void wiiCallback(const fmMsgs::wii_state::ConstPtr& wii_state);
	void initEncoders();
	void controlLoop();
	void makePath(string pathstr);
	void objectRowCallback(const FroboMsgs::object_row::ConstPtr& row);
	void encoderCallback(const FroboMsgs::micro_data data);
	//void wheelCallback(const fmMsgs::float_data::ConstPtr& speeds);
	//void gyroCallback(const fmMsgs::gyroscope::ConstPtr& gyro);
	void objectCallback(const FroboMsgs::detected_objects::ConstPtr& objects);
	//void markerCallback(const std_msgs::Int8::ConstPtr& marker);
	void timerCallback(const ros::TimerEvent& event);
	//void calculate_odometry();
	//void calculate_twist_from_object_boxes();
	//void extract_object_row_data();
	//void run_state_machine();

	// Publisher
	ros::Publisher control_pub;
	
	// PID
	PID cte_pid; 
};

#endif /* CONTROL_H_ */

/*
 * PotDecision.cpp
 *
 *  Created on: Apr 16, 2012
 *      Author: Nis Sarup
 */


#include "PotDecision.h"
#include <vector>
using namespace std;

// Statemachine defines
	enum { 
		STM_START, 
		STM_DETECT_ROW, 
		STM_DRIVE, 
		STM_STOP,
		STM_TURNING,
		STM_END_OF_ROW,
		STM_EXIT_ROW,
		STM_HEADLAND,
		STM_HEADLAND_ROW,
		STM_HEADLAND_HOLE,
	};
 
 	enum { 
		RST_BETWEEN_ROWS,
		RST_RIGHT_ROW,
		RST_LEFT_ROW,
		RST_END_ROW,
		RST_NO_ROW
	 };
	 
	 enum {
		 RIGHT,
		 LEFT
	 };
	int test [4] = {2,2,2,2};
	vector<int> prevpath;
	vector<int> rempath(test,test + sizeof(test) / sizeof(int));
	
/*
 * Constructor
 */
PotDecision::PotDecision() {
	new_speeds = 0;
	new_gyro = 0;
	new_l_row = 0;
	new_r_row = 0;
	new_object_row = 0;
	new_object_message_received = 0;
		
	// Object row data initialization
	object_row_resolution = 0;
	object_row_start_position = 0;
	object_row_data_last_update = ros::Time(0);
	object_row_end_position_left = 1000;
        object_row_end_position_right = 1000;
	row_state = RST_NO_ROW;
	
	// Navigation
	if (rempath[0] == 1) {
	next_turn_direction = RIGHT;
	}
	else if (rempath[0] == 2) {
	next_turn_direction = LEFT;
	}
	// state space
	state_space.b = 0.18;
}

/*
 * Destructor
 */
PotDecision::~PotDecision() {

}

	// Build twist
	++twist_msg.header.seq;
	twist_msg.header.stamp = ros::Time::now();
	twist_msg.twist.linear.x = 0;
	twist_msg.twist.angular.z = 0;
	
	
void PotDecision::extract_object_row_data() {
	int l_end_found = -1, r_end_found = -1;
	double fill_rate_left = 0, fill_rate_right = 0;
	int left_hole_width = 0, right_hole_width = 0, left_hole_valid = 0, right_hole_valid = 0;
	
	left_row_finder.clear_values();
	right_row_finder.clear_values();
	for(int i = 0; i < object_row_left.size(); i++)
	{	
		if(object_row_left[i] > object_row_box_filled_threshold) {
			l_end_found = i;
			fill_rate_left++;
			if(left_hole_width > 0)
			{
				left_hole_valid = 1;
			}
			left_row_finder.list.push_back(true);
		} else {
			left_row_finder.list.push_back(false);
			left_hole_valid = 0;
			left_hole_width++;
		}
		if(object_row_right[i] > object_row_box_filled_threshold) {
			r_end_found = i;
			fill_rate_right++;
			if(right_hole_width > 0)
			{
				right_hole_valid = 1;
			}
			right_row_finder.list.push_back(true);
		} else {
			right_row_finder.list.push_back(false);
			right_hole_valid = 0;
			right_hole_width++;
		}
	}

	right_row_finder.run_scheme();
	left_row_finder.run_scheme();
	
	object_row_end_position_left =  (l_end_found+1) * object_row_resolution;
	object_row_end_position_right = (r_end_found+1) * object_row_resolution;
	
	object_row_fill_percent_left = fill_rate_left / object_row_left.size();
	object_row_fill_percent_right = fill_rate_right / object_row_right.size();
	
	if(object_row_fill_percent_left > object_row_threshold && object_row_fill_percent_right > object_row_threshold ) //&& !left_hole_valid && !right_hole_valid)
	{
		// Between rows
		row_state = RST_BETWEEN_ROWS;
		control_pub.
	} else if(object_row_fill_percent_left > object_row_threshold) // && !left_hole_valid)
	{
		// Row to the left
		row_state = RST_LEFT_ROW;
	} else if(object_row_fill_percent_right > object_row_threshold) // && !right_hole_valid)
	{
		// Row to the left
		row_state = RST_RIGHT_ROW;
	} else if(object_row_end_position_left < 0.2 && object_row_end_position_right < 0.2) {
		row_state = RST_END_ROW;
	
	}
	else {
		// No rows! Panic!
		row_state = RST_NO_ROW;
	}
	
	object_row_data_last_update = ros::Time::now();
	
}

void PotDecision::objectRowCallback(const FroboMsgs::object_row::ConstPtr& row) {
	
	object_row_resolution = row -> resolution;
	
	for(int n = 0; n < row->size; n++) {
		
		object_row_left[n]	= row->left_row[n];
		object_row_right[n]	= row->right_row[n];
	}
	
	new_object_row = 1;
}

void PotDecision::objectCallback(const FroboMsgs::detected_objects::ConstPtr& objects) {
	
	new_object_message_received = 1;
	new_left_object = objects->left_blocked;
	new_right_object = objects->right_blocked;
	new_stop = objects->stop_zone_occupied;
	
	object_left_distance	= objects->closest_object_distance_left;
	object_right_distance	= objects->closest_object_distance_right;
	object_left_angle		= objects->closest_object_angle_left;
	object_right_angle		= objects->closest_object_angle_right;
}	

void PotDecision::timerCallback(const ros::TimerEvent& event) {
	
	if(new_object_row)
	{
		new_object_row = 0;
		extract_object_row_data();
	}
	
	//run_state_machine();
}

void PotDecision::calculate_twist_from_object_boxes() {
	static double dt = time_s;
	//cross_track_error = 0;
	double dist_cte = 0, ang_cte = 0;
	
	// Start driving decision
	if (new_left_object || new_right_object) {
		//if(new_left_object && new_right_object)
		//{
		//	dist_cte = (object_left_distance < object_right_distance) ? object_left_distance: object_right_distance;
		//	ang_cte = (object_left_distance < object_right_distance) ? object_left_angle: object_right_angle;
		//} else 
		if(new_left_object) {
			dist_cte 	= object_left_distance;
			ang_cte 	= -object_left_angle;
		} else {
			dist_cte 	= object_right_distance;
			ang_cte 	= object_right_angle;
		}
		
		//dist_cte = (ang_cte > 0) ? -dist_cte : dist_cte;
	//	cross_track_error = ((1 / ang_cte) * (1/(dist_cte * dist_cte)) / cte_weight_distance); 
		
	} 
	//else {
	//	dist_cte = 0;
	//	ang_cte = 0;
		//cross_track_error = 0;
	//}
	
	
	//if(abs(cross_track_error) > (M_PI / 4))
	//{
	//	cross_track_error = (cross_track_error < 0) ? -M_PI / 4 : M_PI / 4;
	//}
	
	//double cte_t = cross_track_error;
	
	// Build twist
	twist_msg.twist.angular.z = cross_track_error;
	
	new_left_object = 0;
	new_right_object = 0;
}


int main(int argc, char** argv){
	// Initialize ROS
	ros::init(argc, argv, "PotDecision");
	
	// Ros nodehandles
	ros::NodeHandle nh("~");
	ros::NodeHandle n;
	
	// Instatiate object
	PotDecision pd;
	
	// Ros params
	string wheel_topic, row_topic, gyro_topic, twist_topic, object_topic, object_row_topic, row_state_topic;
	
	//nh.param<string>("laser_scan_topic", laser_scan_topic, "laser_scan_topic");
//	nh.param<string>("row_topic", row_topic, "row_topic");
//	nh.param<string>("wheel_topic", wheel_topic, "wheel_topic");
//	nh.param<string>("gyro_topic", gyro_topic, "gyro_topic");
//	nh.param<string>("twist_topic", twist_topic, "/cmd_vel");
	nh.param<string>("object_topic", object_topic, "object_topic");
	nh.param<string>("control topic", control_pub_topic, "control_topic");
	nh.param<string>("object_row_topic", object_row_topic, "object_row_topic");
//	nh.param<string>("row_state_topic", row_state_topic, "row_state_topic");
	nh.param<double>("time_s", pd.time_s, 0.02);
//	nh.param<double>("linear_mean_velocity", pd.linear_mean_velocity, 0.5);
//	nh.param<double>("mean_driving_distance_from_rows", pd.mean_driving_distance_from_rows, 0.35);
//	nh.param<double>("cte_weight_angle", pd.cte_weight_angle, 0.5);
//	nh.param<double>("cte_weight_distance", pd.cte_weight_distance, 0.5);
////	nh.param<double>("base_link_radius_to_wheels", pd.base_link_radius_to_wheels, 0.185);
//	nh.param<double>("cte_kp", pd.cte_kp, 1);
//	nh.param<double>("cte_ki", pd.cte_ki, 0);
//	nh.param<double>("cte_kd", pd.cte_kd, 0);
	nh.param<int>("object_row_box_filled_threshold", pd.object_row_box_filled_threshold, 0);
	nh.param<double>("object_row_threshold", pd.object_row_threshold, 0.5);
	
//	nh.param<double>("dead_reckoning_turn_rate", pd.dead_reckoning_turn_rate, 0.5);

	
	// Subscribes and publishers
//	ros::Subscriber row_subscriber = n.subscribe<fmMsgs::row>(row_topic.c_str(), 1, &PotDecision::rowCallback, &pd);
//	ros::Subscriber wheel_subscriber = n.subscribe<fmMsgs::float_data>(wheel_topic.c_str(), 10, &PotDecision::wheelCallback, &pd);
//	ros::Subscriber gyro_subscriber = n.subscribe<fmMsgs::gyroscope>(gyro_topic.c_str(), 10, &PotDecision::gyroCallback, &pd);
	ros::Subscriber object_subscriber = n.subscribe<FroboMsgs::detected_objects>(object_topic.c_str(), 10, &PotDecision::objectCallback, &pd);
	ros::Subscriber object_row_subscriber = n.subscribe<FroboMsgs::object_row>(object_row_topic.c_str(), 10, &PotDecision::objectRowCallback, &pd);

//	pd.twist_pub = n.advertise<geometry_msgs::TwistStamped>(twist_topic.c_str(), 1);
//	pd.row_state_pub = n.advertise<fmMsgs::hilde_states>(row_state_topic.c_str(), 1);
	pd.control_pub = n.advertise<FroboMsgs::error>(control_pub_topic.c_str(), 1);
	
	ros::Timer timer = n.createTimer(ros::Duration(pd.time_s), &PotDecision::timerCallback, &pd);

//	pd.cte_pid(pd.cte_kp, pd.cte_ki, pd.cte_kd);
	
	
	// Spin
	ros::spin();
	return 0;
}

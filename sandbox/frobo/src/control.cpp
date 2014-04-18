
#include "control.h"
#include <vector>
#include "boost/tokenizer.hpp"

using namespace std;

double moveSpeed;
double regSpeed;
double maxSpeed;
double difSpeed = maxSpeed - moveSpeed;
double Preg;
double speed_left;
double speed_right;

struct pathelement{
	int rowsize;
	int turn_direction;
};

typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
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
		STM_RETURN,
		STM_ENTER_ROW,
	};
 
 	enum { 
		RST_BETWEEN_ROWS,
		RST_RIGHT_ROW,
		RST_LEFT_ROW,
		RST_NO_ROW
	 };
	 
	 enum {
		 RETURN,
		 RIGHT,
		 LEFT

	 };
//	int test [4] = {2,2,2,2};
//	vector<int> prevpath;
	vector<pathelement> path; //rempath(test,test + sizeof(test) / sizeof(int));
	int next_turn_direction;
	int int_rows;
	int rowSize;
	int rowcount = 0;
	int holecount = 0;
	int state = STM_START;
	//double left_error, right_error;
	int previous_state = STM_START;
	double p_led;
	double encoder_left;
	double encoder_right;
	double encoder_right_moved;
	double encoder_right_offset, encoder_left_offset;
	double encoder_left_moved;
	double turn90;
	double exitRow_m;
	unsigned int returnTicks;
	bool startEnc = false;
	bool first_new_stop = true;
	int fill_left, fill_right;
	double fill_rate_right;
	double fill_rate_left;
	int finding_row;
	//string pathstr = "2R-1L-2R-3L-4L-0-2R-4R-1L";
	string pathstr = "1R-1R-1R-1R-1R-1R-1R-1R-1R-1R-1R-1R-1R";

Control::Control() {
	new_object_row = 0;
	new_object_message_received = 0;
	// Object row data initialization
	object_row_resolution = 0;
	object_row_start_position = 0;
	object_row_data_last_update = ros::Time(0);
	object_row_end_position_left = 1000;
   	object_row_end_position_right = 1000;
	//row_state = RST_NO_ROW;
	//makePath(pathstr);
}

Control::~Control() {

}

void Control::makePath(string pathstr) {
	boost::char_separator<char> sep("-");
	tokenizer::iterator tok_iter;
	tokenizer tokens(pathstr,sep);
	//tok_iter = tokens.begin();
	for (tok_iter = tokens.begin(); tok_iter != tokens.end();tok_iter++) {
		pathelement p;
		string temp = *tok_iter;

		p.rowsize = int(temp[0]-'0');
		//cout << temp[0] << endl;
		if(p.rowsize != 0) {
			if (temp[1] == 'R') {
				p.turn_direction = RIGHT;
			}
			else if (temp[1] == 'L') {
				p.turn_direction = LEFT;
			}
		}
		else {
			p.turn_direction = RETURN;
		}
		ROS_INFO("Rowsize: %d Direction: %d",p.rowsize,p.turn_direction );
		path.push_back(p);
	}

}

void Control::controlLoop() {
	switch (state) {
		case STM_START:
			state = STM_DETECT_ROW;
			ROS_WARN("Started, switching to state: DETECT ROW");
			makePath(pathstr);
			control.enable_left = 1;
			control.enable_right = 1;
			//	between_row_counter = 0;
			//	right_row_counter = 0;
			//	left_row_counter = 0;
			//	gyro_offset = gyro_z;
			break;
		case STM_DETECT_ROW:

			int_rows = path.at(0).rowsize;
			next_turn_direction = path.at(0).turn_direction;

			path.erase(path.begin());
			state = STM_DRIVE;
			ROS_WARN("STM_DRIVE");
			break;
		case STM_DRIVE:
			control.direction_left = 1;
			control.direction_right = 1;
			if (end_l < 1 && end_r < 1 && new_object_row == 1) {
				state = STM_EXIT_ROW;
				startEnc = true;
				ROS_WARN("end of row detected");
				if (next_turn_direction == LEFT)
					ROS_WARN("GOING LEFT");
				else if (next_turn_direction == RIGHT)
					ROS_WARN("GOING RIGHT");
				else
					ROS_WARN("MAKING U-TURN");
			}
			else if (new_right_object == 1 || new_left_object == 1){
				//control.direction_left = 1;
				//control.direction_right = 1;
				if (new_right_object == 1 && new_left_object == 1){
					ROS_WARN("Object_right_angle: %f Object_left_angle: %f Object_left_distance: %f Object_right_distance: %f", object_right_angle, object_left_angle, object_left_distance, object_right_distance);/*
					right_error = atan((0.25 - sin(object_right_angle)*(object_right_distance+0.00001))/(cos	(object_right_angle)*(object_right_distance+0.00001)))*Preg;
					left_error = atan((0.25 - sin(object_left_angle)*object_left_distance+0.00001)/(cos(object_left_angle)*(object_left_distance+0.00001)))*Preg;
					error = left_error - right_error;*/
					if (object_right_angle > object_left_angle){
						error = (object_right_angle - object_left_angle)*p_led;
						speed_right = (moveSpeed - error);
						speed_left = (moveSpeed + error);
						if (speed_left > maxSpeed) {
							speed_left = maxSpeed;
						}
						if (speed_right < 0){
							speed_right = 0.1;
						}
					}
					else if(object_right_angle < object_left_angle){
						error = (object_left_angle - object_right_angle)*p_led;
						speed_right = (moveSpeed + error);
						speed_left = (moveSpeed - error);
						if (speed_right > maxSpeed) {
							speed_right = maxSpeed;
						}
						if (speed_left < 0){
							speed_left = 0.1;
						}
					}
					else {
						speed_right = moveSpeed;
						speed_left = moveSpeed;
					}
					control.speed_right = speed_right;
					control.speed_left = speed_left;
					ROS_WARN("speed_right: %f speed_left: %f", speed_right, speed_left);
				
				}
				else if (new_right_object) {
					if (object_right_angle > 1.0) {
						speed_right = moveSpeed-0.1;
						speed_left = moveSpeed;
					}
					else {
						error = 1.0 - object_right_angle;
						speed_right = moveSpeed + error;
						speed_left = moveSpeed - error;
						if (speed_right > maxSpeed) {
							speed_right = maxSpeed;
						}
						if (speed_left < 0) {
							speed_left = 0.1;
						}
					}
					ROS_WARN("speed_right: %f speed_left: %f", speed_right, speed_left);
					control.speed_right = speed_right;
					control.speed_left = speed_left;
				/*
				control.direction = 4;
				control.speed_right = moveSpeed;
					control.speed_left = moveSpeed;*/
					/*right_error = atan((0.25 - sin(object_right_angle)*object_right_distance)/(cos(object_right_angle)*object_right_distance))*Preg;
					ROS_WARN("right error: %f", right_error);
					if (right_error < moveSpeed && right_error > 0) {
						ROS_INFO("Slow turn left");
						control.speed_right = moveSpeed + right_error;
						control.speed_left = moveSpeed - right_error;
					}
					else if(right_error >= moveSpeed){
						ROS_INFO("fast turn left");
						control.speed_right = regSpeed;
						control.speed_left = 0;
					}*/
				}
				else if(new_left_object) {
					if (object_left_angle > 1.0) {
						speed_left = moveSpeed-0.1;
						speed_right = moveSpeed;
					}
					else {
						error = 1.0 - object_left_angle;
						speed_left = moveSpeed + error;
						speed_right = moveSpeed - error;
						if (speed_left > maxSpeed) {
							speed_left = maxSpeed;
						}
						if (speed_right < 0) {
							speed_right = 0.1;
						}
					}
					ROS_WARN("speed_right: %f speed_left: %f", speed_right, speed_left);
					control.speed_right = speed_right;
					control.speed_left = speed_left;
				/*	control.direction = 4;
					control.speed_right = moveSpeed;
					control.speed_left = moveSpeed; */
					/*left_error = atan((0.25 - sin(object_left_angle)*object_left_distance)/(cos(object_left_angle)*object_left_distance))*Preg;
					ROS_WARN("left error: %f", left_error);
					if (left_error < moveSpeed && left_error > 0) {
						ROS_INFO("Slow turn right");
						control.speed_left = moveSpeed + left_error;
						control.speed_right = moveSpeed - left_error;
					}
					else if(left_error >= moveSpeed){
						ROS_INFO("fast turn right");
						control.speed_left = regSpeed;
						control.speed_right = 0;
					}*/		
				}
			}
			else if (new_right_object == 0 && new_left_object == 0){
				ROS_INFO("I'M LOST!'");
				//control.direction = 5;
				control.speed_right = moveSpeed;
				control.speed_left = moveSpeed;
			}

			break;
		case STM_EXIT_ROW:
			//ROS_INFO("EXIT ROW");
			control.direction_left = 1;
			control.direction_right = 1,
			control.speed_right = moveSpeed;
			control.speed_left = moveSpeed;
			if (startEnc) {
				initEncoders();
				startEnc = false;			
			}
			ROS_WARN("Encoders left moved: %f Encoder right moved: %f", encoder_left_moved, encoder_right_moved);
			if (encoder_left_moved  > exitRow_m) {
				control.speed_left = 0;
			}
			if (encoder_right_moved  > exitRow_m) {
				control.speed_right = 0;
			}
			
			if (encoder_left_moved  > exitRow_m && encoder_right_moved > exitRow_m) {
				state = STM_HEADLAND;
				ROS_WARN("ENTERING HEADLAND!");
			}
			break;
		case STM_HEADLAND:
			if (next_turn_direction == RETURN) {
				state = STM_RETURN;
				startEnc = true;			
			}
			else {
				state = STM_TURNING;
				startEnc = true;
				ROS_WARN("TURNING!");
			}
			break;
		case STM_TURNING:
			if (startEnc) {
				initEncoders();
				startEnc = false;
			}
			ROS_WARN("Encoders left moved: %f Encoder right moved: %f", encoder_left_moved, encoder_right_moved);
			if (next_turn_direction == RIGHT) {
				control.direction_left = 1;
				control.direction_right = 0,
				control.speed_right = moveSpeed;
				control.speed_left = moveSpeed;
			}
			else if (next_turn_direction == LEFT){
				control.direction_left = 0;
				control.direction_right = 1,
				control.speed_right = moveSpeed;
				control.speed_left = moveSpeed;
			}
			
			if (encoder_left_moved > turn90){
				control.speed_left = 0;
			}
			if (encoder_right_moved > turn90){
				control.speed_right = 0;
			}
			if (encoder_left_moved > turn90 && encoder_right_moved > turn90) {
				ROS_WARN("90 degrees turned, entering headland_row");
				state = STM_HEADLAND_ROW;
			}
			
			/*
			}
			if (next_turn_direction == RIGHT) {
				if (new_right_object) {
					control.speed_right = regSpeed;
					control.speed_left = 0;				
				}
				//else {
				//	control.speed_right = moveSpeed;
				//	control.speed_left = moveSpeed;								
				//}
				//ROS_WARN("Encoder value = %u", encoder_right_moved);
				if (previous_state == STM_ENTER_ROW) {
					if (encoder_left_moved > turn90ticks) {
						state = STM_DETECT_ROW;					
					}
				}
				else if (end_r > -1 || encoder_left_moved > turn90ticks) {
					state = STM_HEADLAND_ROW;
					ROS_WARN("90 degrees turned, entering headland_row");
				}
			}
			else if (next_turn_direction == LEFT) {
				if (new_left_object) {
					control.speed_right = 0;
					control.speed_left = regSpeed;				
				}
				//else {
				//	control.speed_right = moveSpeed;
				//	control.speed_left = moveSpeed;								
				//}				
				if (previous_state == STM_ENTER_ROW) {
					if (encoder_right_moved > turn90ticks) {
						state = STM_DETECT_ROW;					
					}
				}
				else if (end_l > -1 || encoder_right_moved > turn90ticks) {
					state = STM_HEADLAND_ROW;
					ROS_WARN("90 degrees turned, entering headland_row");
				}
			}*/
			break;
		case STM_RETURN:
			
			break;
		case STM_HEADLAND_ROW:
			control.speed_right = 0;
			control.speed_left = 0;/*
			control.direction_left = 1;
			control.direction_right = 1;
			ROS_WARN("Inside Row, looking for hole");
			if (next_turn_direction == RIGHT) {
				
				control.speed_right = moveSpeed;
				control.speed_left = moveSpeed;
				
				if (new_right_object) {
					ROS_WARN("TURN ZONE REACHED, TURNING SLOW");
					control.speed_right = 0;
					control.speed_left = moveSpeed;				
				}
									

				if (end_r > rowSize/2) {
					if (int_rows == rowcount) {
						state = STM_ENTER_ROW;
						finding_row = 0;
						ROS_WARN("Hole found, starting to turn");
					}
					else {
						state = STM_HEADLAND_HOLE;
						ROS_WARN("Another row needed, continuing");
					}
				}
			}
			else if (next_turn_direction == LEFT) {
				control.speed_right = moveSpeed;
				control.speed_left = moveSpeed;
				
				if (new_left_object) {
					ROS_WARN("TURN ZONE REACHED, TURNING SLOW");
					control.speed_right = moveSpeed;
					control.speed_left = 0;				
				}
				
				if (end_r > rowSize/2) {
					if (int_rows == rowcount) {
						state = STM_ENTER_ROW;
						finding_row = 0;
						ROS_WARN("Hole found, starting to turn");
					}
					else {
						state = STM_HEADLAND_HOLE;
						ROS_WARN("Another row needed, continuing");
					}
				}

			}
			break;
		case STM_HEADLAND_HOLE:

			if (next_turn_direction == RIGHT) {
		
				if (new_right_object) {
					ROS_WARN("TURN ZONE REACHED, TURNING SLOW");
					control.speed_right = 0;
					control.speed_left = moveSpeed;				
				}
				else {
					control.speed_right = moveSpeed;
					control.speed_left = moveSpeed;				
				}				

				if (end_r < rowSize/5) {
					state = STM_HEADLAND_ROW;
					rowcount++;
					ROS_WARN("New row found");
				}
			}
			else if (next_turn_direction == LEFT) {
			
				if (new_left_object) {
					ROS_WARN("TURN ZONE REACHED, TURNING SLOW");
					control.speed_right = moveSpeed;
					control.speed_left = 0;				
				}
				else {
					control.speed_right = moveSpeed;
					control.speed_left = moveSpeed;				
				}				
				
				if (end_l < rowSize/5) {
					state = STM_HEADLAND_ROW;
					rowcount++;
					ROS_WARN("New row found");
				}
			}*/
			break;
		case STM_ENTER_ROW:
			previous_state = STM_ENTER_ROW;
			startEnc = true;
			state = STM_TURNING;
			/*if (next_turn_direction == RIGHT) {
				control.speed_right = 0;
				control.speed_left = moveSpeed;
				if (end_r > -1 && finding_row != 2) finding_row = 1;
				if (finding_row == 1 && end_l > -1) finding_row = 2;
				if (finding_row == 2 && fill_rate_right > 0.5) {
					ROS_WARN("NEW ROW FOUND, GETTING NEW PATH");
					state = STM_DETECT_ROW;
					control.speed_right = 0;
					control.speed_left = 0;
				}
			}
			else if (next_turn_direction == LEFT) {
				control.speed_right = 0;
				control.speed_left = moveSpeed;
				if (end_l > -1 && finding_row != 2) finding_row = 1;
				if (finding_row == 1 && end_r > -1) finding_row = 2;
				if (finding_row == 2 && fill_rate_left > 0.5) {
					ROS_WARN("NEW ROW FOUND, GETTING NEW PATH");
					state = STM_DETECT_ROW;
					control.speed_right = 0;
					control.speed_left = 0;
				}
			}	 */
			break;
		case STM_STOP:
			/*if (new_stop == 0) {
				ROS_WARN("ROS STATE: %d" , previous_state);
				state = previous_state;
				first_new_stop = true;
			}
			control.direction = 5;
			control.speed_right = 0;
			control.speed_left = 0;
			//end_of_row = 0;
			ROS_WARN("State: STOP!");*/
			control.enable_left = 0;
			control.enable_right = 0;
			state = STM_DRIVE;
			break;
	}
	
control_pub.publish(control);
		
}

void Control::initEncoders() {
	encoder_right_offset = encoder_right;
	encoder_left_offset = encoder_left;
	ROS_WARN("Encoder_left_off: %f Encoder_right_off: %f",encoder_right_offset,encoder_left_offset);
	encoder_left_moved = 0;
	encoder_right_moved = 0;
}

void Control::objectRowCallback(const FroboMsgs::object_row::ConstPtr& row) {
	//ROS_WARN("Row received");
	//ROS_INFO("size: %d",row->size);
//	object_row_resolution = 0;
//	object_row_resolution = row->resolution;
	int l = 0;
	int r = 0;
	rowSize = row->size;
	fill_left = 0;
	fill_right = 0;
	for (int i = 0; i < row->size;i++) {
		if (row->left_row[i] > 0) {
			end_l = i;
			fill_left++;
		}
		else
			l++;
		if(row->right_row[i] > 0) {
			end_r = i;
			fill_right++;
		}
		else
			r++;
	}
	if (l == row->size)
		end_l = -1;
	if (r == row->size)
		end_r = -1;
	
	fill_rate_right = fill_right / rowSize;
	fill_rate_left = fill_left / rowSize;
	
	//ROS_INFO("endl: %d endr: %d",end_l,end_r);
	
	new_object_row = 1;	

	/*for(int n = 0; n < row->size; n++) {
		object_row_left[n] = 0;
		object_row_right[n] = 0;
	}
	//for(int i = 0; i < row->size; i++) {
	//	object_row_left[i]	= row->left_row[i];
	//	object_row_right[i]	= row->right_row[i];
	//}
	
	end_l = calcEnd(object_row_left);
	end_r = calcEnd(object_row_right);
	
	new_object_row = 1;*/
}
void Control::encoderCallback(const FroboMsgs::micro_data data) {
	encoder_left = data.encoder_l;
	encoder_right = data.encoder_r;
	
	//ROS_WARN("encoder left: %d encoder right: %d",encoder_left,encoder_right);
	
	encoder_right_moved = abs(encoder_right - encoder_right_offset);
	encoder_left_moved = abs(encoder_left - encoder_left_offset);
	
}

void Control::objectCallback(const FroboMsgs::detected_objects::ConstPtr& objects) {	
	//ROS_WARN("Object received");
	//new_object_message_received = 1;
	new_left_object = objects->left_blocked;
	new_right_object = objects->right_blocked;
	new_stop = objects->stop_zone_occupied;
	
	if (new_stop == 1) {
		if (first_new_stop) {
			previous_state = state;
			first_new_stop = false;
		}
		state = STM_STOP;
	}	
			
	object_left_distance	= objects->closest_object_distance_left;
	object_right_distance	= objects->closest_object_distance_right;
	object_left_angle		= -objects->closest_object_angle_left;
	object_right_angle		= objects->closest_object_angle_right;
	
	controlLoop();

}

int main(int argc, char** argv){
	// Initialize ROS
	ros::init(argc, argv, "Control");
	
	// Ros nodehandles
	ros::NodeHandle nh("~");
	ros::NodeHandle n;
	
	// Instatiate object
	Control pd;
	string object_topic,object_row_topic,pwm,micro_data;
	nh.param<string>("object_topic", object_topic, "object_topic");
//	nh.param<string>("state", state, "state");
	nh.param<string>("object_row_topic", object_row_topic, "object_row_topic");
	//nh.param<string>("fpga_data", fpga_data, "fpga_data");
	nh.param<string>("pwm", pwm, "pwm");
	nh.param<double>("turn90", turn90, 3400);
	nh.param<double>("p_led", p_led, 0.8);
	nh.param<double>("exitRow_m", exitRow_m, 1000);
	nh.param<string>("micro_data", micro_data, "/frobo/distance");
	//nh.param<int>("returnTicks", returnTicks, 6000);
	nh.param<double>("moveSpeed", moveSpeed, 0.5);
	nh.param<double>("maxSpeed", maxSpeed, 1.0);
	//nh.param<int>("regSpeed", regSpeed, 10);
	
	ros::Subscriber object_subscriber = n.subscribe<FroboMsgs::detected_objects>(object_topic.c_str(), 10, &Control::objectCallback, &pd);
	ros::Subscriber object_row_subscriber = n.subscribe<FroboMsgs::object_row>(object_row_topic.c_str(), 10, &Control::objectRowCallback, &pd);
	ros::Subscriber encoder_subscriber = n.subscribe<FroboMsgs::micro_data>(micro_data.c_str(),10,&Control::encoderCallback,&pd);
	pd.control_pub = n.advertise<FroboMsgs::pwm_micro>(pwm.c_str(), 1);
	//pd.state_pub = n.advertise<FroboMsgs::state>(state.c_str(), 1);

	// Spin
	ros::spin();
	return 0;
}

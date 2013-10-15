
#include "control.h"
#include <vector>
#include "boost/tokenizer.hpp"

using namespace std;

int moveSpeed;
int regSpeed;
double Preg;

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
	vector<int> prevpath;
	vector<pathelement> path; //rempath(test,test + sizeof(test) / sizeof(int));
	int next_turn_direction;
	int int_rows;
	int state = STM_START;
	int previous_state = STM_START;
	int encoder_right_moved;
	int encoder_right_offset, encoder_left_offset;
	int encoder_left_moved;
	int turn90ticks;
	int exitRowTicks;
	int returnTicks;
	bool startEnc = false;
	string pathstr = "2R-1L-2R-3L-4L-0-2R-4R-1L";

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
    makePath(pathstr);
	
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
	
	
	int rowcount = 0;
	int holecount = 0;

	switch (state) {
		case STM_START:
				state = STM_DETECT_ROW;
				ROS_WARN("State: DETECT ROW");
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
			break;
		case STM_DRIVE:
			// Driving. Calculate twist when new box-info comes in
			control.direction = 5;
			
			if (end_l < 1 && end_r < 1) {
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

			
			if (new_right_object == 1 || new_left_object == 1)
			{
				if (new_right_object == 1 && new_left_object == 1)
				{
					double right_error = atan((0.25 - sin(object_right_angle)*object_right_distance)/(cos(object_right_angle)*object_right_distance))*Preg;
					double left_error = atan((0.25 - sin(object_left_angle)*object_left_distance)/(cos(object_left_angle)*object_left_distance))*Preg;
					error = left_error - right_error;
					int speed_right = regSpeed + error;
					int speed_left = regSpeed - error;
					if (speed_right > regSpeed) {
						control.speed_right = regSpeed;
						control.speed_left = 0;
					}
					else if (speed_left > regSpeed){
						control.speed_right = 0;
						control.speed_left = regSpeed;
					}
					else {
						control.speed_right = speed_right;
						control.speed_left = speed_left;
					}
				}
				else if (new_right_object) {
double right_error = atan((0.25 - sin(object_right_angle)*object_right_distance)/(cos(object_right_angle)*object_right_distance))*Preg;
					if (right_error < moveSpeed && right_error > 0) {
						ROS_INFO("Slow turn left");
						control.speed_right = regSpeed + right_error;
						control.speed_left = regSpeed - right_error;
					}
					else if(right_error >= moveSpeed){
						ROS_INFO("fast turn left");
						control.speed_right = regSpeed;
						control.speed_left = 0;
					}
				}
				else if(new_left_object) {
double left_error = atan((0.25 - sin(object_left_angle)*object_left_distance)/(cos(object_left_angle)*object_left_distance))*Preg;
					if (left_error < moveSpeed && left_error > 0) {
						ROS_INFO("Slow turn right");
						control.speed_left = regSpeed + left_error;
						control.speed_right = regSpeed - left_error;
					}
					else if(left_error >= moveSpeed){
						ROS_INFO("fast turn right");
						control.speed_left = regSpeed;
						control.speed_right = 0;
					}
				}
			}
			else if (new_right_object == 0 && new_left_object == 0)
			{
				ROS_INFO("drive straight");
				control.speed_right = moveSpeed;
				control.speed_left = moveSpeed;
			}
									
			break;
		case STM_EXIT_ROW:
			ROS_INFO("EXIT ROW");
			control.direction = 5;
			control.speed_right = moveSpeed;
			control.speed_left = moveSpeed;
			if (startEnc) {
				initEncoders();
				startEnc = false;			
			}	
			if ((encoder_left_moved + encoder_right_moved) > exitRowTicks) {
			state = STM_HEADLAND;
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
			}
			break;
		case STM_TURNING:
			if (startEnc) {
				initEncoders();
				if (next_turn_direction == RIGHT) {
					control.direction = 4;
					control.speed_right = regSpeed;
					control.speed_left = regSpeed;
				}
				else if (next_turn_direction == LEFT){
					control.direction = 1;
					control.speed_right = regSpeed;
					control.speed_left = regSpeed;
				}
				startEnc = false;
			}
			if (next_turn_direction == RIGHT) {
				if (end_r > -1 || encoder_right_moved + encoder_left_moved > turn90ticks) {
					state = STM_HEADLAND_ROW;
				}
			}
			else if (next_turn_direction == LEFT) {
				if (end_l > -1 || encoder_left_moved + encoder_right_moved > turn90ticks) {
					state = STM_HEADLAND_ROW;
				}
			}
			break;
		case STM_RETURN:
			
			break;
		case STM_HEADLAND_ROW:

			break;
		case STM_STOP:
			if (new_stop == 0) {
				state = previous_state;
			}
			control.direction = 5;
			control.speed_right = 0;
			control.speed_left = 0;
			end_of_row = 0;
			ROS_WARN("State: STOP!");
			break;
	}
	
		control_pub.publish(control);
		
	}

void Control::initEncoders() {
	encoder_right_offset = encoder_right;
	encoder_left_offset = encoder_left;
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
	for (int i = 0; i < row->size;i++) {
		if (row->left_row[i] > 0)
			end_l = i;
		else
			l++;
		if(row->right_row[i] > 0)
			end_r = i;
		else
			r++;
	}
	if (l == row->size)
		end_l = -1;
	if (r == row->size)
		end_r = -1;

	ROS_INFO("endl: %d endr: %d",end_l,end_r);
	
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
void Control::encoderCallback(const FroboMsgs::fpga_data::ConstPtr& data) {
	encoder_left = data->encoder_l;
	encoder_right = data->encoder_r;
	
	if (encoder_right_moved > 1000000 && encoder_left_moved < 1000000) {
		encoder_right_moved = encoder_left_moved;
	}
	else if (encoder_left_moved > 1000000 && encoder_right_moved < 1000000) {
		encoder_left_moved = encoder_right_moved;
	}
	
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
		previous_state = state;
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
	string object_topic,object_row_topic,pwm,fpga_data;
	nh.param<string>("object_topic", object_topic, "object_topic");
//	nh.param<string>("state", state, "state");
	nh.param<string>("object_row_topic", object_row_topic, "object_row_topic");
	nh.param<string>("fpga_data", fpga_data, "fpga_data");
	nh.param<string>("pwm", pwm, "pwm");
	nh.param<int>("turn90", turn90ticks, 3400);
	nh.param<int>("exitRowTicks", exitRowTicks, 1000);
	nh.param<int>("returnTicks", returnTicks, 6000);
	nh.param<int>("moveSpeed", moveSpeed, 5);
	nh.param<double>("Preg", Preg, 1.0);
	nh.param<int>("regSpeed", regSpeed, 10);
	
	ros::Subscriber object_subscriber = n.subscribe<FroboMsgs::detected_objects>(object_topic.c_str(), 10, &Control::objectCallback, &pd);
	ros::Subscriber object_row_subscriber = n.subscribe<FroboMsgs::object_row>(object_row_topic.c_str(), 10, &Control::objectRowCallback, &pd);
	ros::Subscriber encoder_subscriber = n.subscribe<FroboMsgs::fpga_data>(fpga_data.c_str(),10,&Control::encoderCallback,&pd);
	pd.control_pub = n.advertise<FroboMsgs::pwm_o>(pwm.c_str(), 1);
	//pd.state_pub = n.advertise<FroboMsgs::state>(state.c_str(), 1);

	// Spin
	ros::spin();
	return 0;
}

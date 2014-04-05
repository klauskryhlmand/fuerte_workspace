#!/usr/bin/env python
#-*- coding:utf-8 -*-

'''
	Project Name:     distance_test
	File name:        goFullCorrected.py
	Creation date:    Sat Apr 5 20:04:30 CEST 2014
	Author:           Daniel Lindekilde Ravn
	SDU email:        darav08@student.sdu.dk
	gmail:            daniel.ravn43@gmail.com
'''

import roslib; roslib.load_manifest('beginner_tutorials')#; roslib.load_manifest('microcontroller_connector')
import rospy
from std_msgs.msg import Int16, Float64
from FroboMsgs.msg import micro_data
from FroboMsgs.msg import pwm_micro
from FroboMsgs.msg import travel

class StopControler (object):

	def __init__(self,start_point, end_point):
		self.__start_point = start_point
		self.__end_point = end_point
		pass


class Traveler (object):

	def __init__(self):
		rospy.init_node('distance_test')
		rospy.loginfo('distance_test created')
		
		rospy.Subscriber('distance', micro_data, self.traveling)
		rospy.Subscriber('travel', travel, self.go)
		
		self.pub_pwm = rospy.Publisher('pwm', pwm_micro)
		
		self.__desired_destination_left = 0.0
		self.__desired_destination_right = 0.0
		
		self.__stop_criteria_left
		self.__stop_criteria_right
		
		self.__desired_speed_left = 0.0
		self.__desired_speed_right = 0.0
		self.__direction_left = 1
		self.__direction_right = 1
		self.__copy_of_current_dist_left = 0.0
		self.__copy_of_current_dist_right = 0.0
		self.__go_left = False
		self.__go_right = False
		
		self.main_loop()
		pass


	def go(self,msg):
		self.__desired_destination_left = float(msg.dist_left) + self.__copy_of_current_dist_left
		self.__desired_destination_right = float(msg.dist_right) + self.__copy_of_current_dist_right
		
		if self.__distance_to_go_to_left < self.__copy_of_current_dist_left:
			self.__direction_left = 0
		else:
			self.__direction_left = 1
		
		if self.__distance_to_go_to_right < self.__copy_of_current_dist_right:
			self.__direction_right = 0
		else:
			self.__direction_right=1
		
		self.__desired_speed_left = float(msg.speed_left)
		self.__desired_speed_right = float(msg.speed_right)
		self.__go_left = True
		self.__go_right = True
		pass


	def traveling(self,msg):
		self.__copy_of_current_dist_left = float(msg.encoder_l)
		self.__copy_of_current_dist_right = float(msg.encoder_r)
		pass

	def stop(self):
		msg_pwm = pwm_micro()
		msg_pwm.speed_left = 0.0
		msg_pwm.speed_right = 0.0
		msg_pwm.direction_left = 1
		msg_pwm.direction_right = 1
		msg_pwm.enable_left = 0
		msg_pwm.enable_right = 0
		self.pub_pwm.publish(msg_pwm)
		pass

	def main_loop(self):
		rospy.loginfo('distance_test node running')
		while not rospy.is_shutdown():
			msg_pwm = pwm_micro()
			if self.__go_left:
				rospy.loginfo('go left!!!')
				msg_pwm.direction_left = self.__direction_left
				msg_pwm.enable_left = 1
				
			else:
				msg_pwm.speed_left = 0.0
				msg_pwm.direction_left = 1
				msg_pwm.enable_left = 0
			
			
			if self.__go_right:
				rospy.loginfo('go right!!!')
				msg_pwm.direction_right = self.__direction_right
				msg_pwm.enable_right = 1
				

			else:
				msg_pwm.speed_right = 0.0
				msg_pwm.direction_right = 1
				msg_pwm.enable_right = 0
				
			self.pub_pwm.publish(msg_pwm)
			rospy.sleep(0.02)
		pass

if __name__ == '__main__':
	try:
		traveler = Traveler()
	except rospy.ROSInterruptException:
		pass




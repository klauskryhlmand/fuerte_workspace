#!/usr/bin/env python
#-*- coding:utf-8 -*-

'''
	Project Name:     distance_test
	File name:        go.py
	Creation date:    Thu Apr 3 11:00:51 CEST 2014
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


class Traveler (object):

	def __init__(self):
		rospy.init_node('distance_test')
		rospy.loginfo('distance_test started')
		rospy.Subscriber('distance', micro_data, self.traveling)
		rospy.Subscriber('travel', travel, self.go)
		
		self.pub_pwm = rospy.Publisher('pwm', pwm_micro)
		
		self.__distance_to_go_to_left = 0.0
		self.__distance_to_go_to_right = 0.0
		self.__desired_speed_left = 0.0
		self.__desired_speed_right = 0.0
		self.__direction_left = 1
		self.__direction_right = 1
		self.__copy_of_current_dist_left = 0.0
		self.__copy_of_current_dist_right = 0.0
		self.__go = False
		
		self.main_loop()
		pass


	def go(self,msg):
		self.__distance_to_go_to_left = float(msg.dist_left) + self.__copy_of_current_dist_left
		self.__distance_to_go_to_right = float(msg.dist_right) + self.__copy_of_current_dist_right
		
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
		self.__go = True
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
		while not rospy.is_shutdown():
			if self.__go:
				rospy.loginfo('go!!!')
				msg_pwm = pwm_micro()
				
				msg_pwm.direction_left = self.__direction_left
				msg_pwm.enable_left = 1
				
				if self.__copy_of_current_dist_left < self.__distance_to_go_to_left - 0.1 - self.__desired_speed_left or self.__copy_of_current_dist_left > self.__distance_to_go_to_left + 0.1 + self.__desired_speed_left :
					msg_pwm.speed_left = self.__desired_speed_left
				elif self.__copy_of_current_dist_left < self.__distance_to_go_to_left - 0.1 or self.__copy_of_current_dist_left > self.__distance_to_go_to_left + 0.1:
					msg_pwm.speed_left = self.__desired_speed_left/2.0
				elif self.__copy_of_current_dist_left < self.__distance_to_go_to_left - 0.01 or self.__copy_of_current_dist_left > self.__distance_to_go_to_left + 0.01:
					msg_pwm.speed_left = 0.1
				else:
					msg_pwm.speed_left = 0.0
				
				msg_pwm.direction_right = self.__direction_right
				msg_pwm.enable_right = 1
				
				if self.__copy_of_current_dist_right < self.__distance_to_go_to_right - 0.1 - self.__desired_speed_right or self.__copy_of_current_dist_right > self.__distance_to_go_to_right + 0.1 + self.__desired_speed_right:
					msg_pwm.speed_right = self.__desired_speed_right
				elif self.__copy_of_current_dist_right < self.__distance_to_go_to_right - 0.1 or self.__copy_of_current_dist_right > self.__distance_to_go_to_right + 0.1:
					msg_pwm.speed_right = self.__desired_speed_right/2.0
				elif self.__copy_of_current_dist_right < self.__distance_to_go_to_right - 0.01 or self.__copy_of_current_dist_right > self.__distance_to_go_to_right + 0.01:
					msg_pwm.speed_right = 0.1
				else:
					msg_pwm.speed_right = 0.0
				
				if (self.__copy_of_current_dist_right > self.__distance_to_go_to_right - 0.1 and self.__copy_of_current_dist_right < self.__distance_to_go_to_right + 0.1) and (self.__copy_of_current_dist_left > self.__distance_to_go_to_left - 0.1 and self.__copy_of_current_dist_left < self.__distance_to_go_to_left + 0.1):
					self.__go = False
				
				self.pub_pwm.publish(msg_pwm)
			else:
				self.stop()
			rospy.sleep(0.02)
		pass

if __name__ == '__main__':
	try:
		traveler = Traveler()
	except rospy.ROSInterruptException:
		pass




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

import roslib; roslib.load_manifest('beginner_tutorials'); roslib.load_manifest('microcontroller_connector')
import rospy
from std_msgs.msg import Int16, Float64
from FroboMsgs.msg import micro_data

class Traveler (object):

	def __init__(self):
		rospy.init_node('distance_test')
		rospy.loginfo('distance_test started')
		rospy.Subscriber('distance', pwm_micro, self.pwmCallback)
		pass




def main():
	traveler = Traveler()
	pass

if __name__ == '__main__':
	sys.exit(main())




#!/usr/bin/env python
'''
	Project Name:     simple_controler Visualization
	File name:        visualization.py
	Creation date:    Tue Apr 8 21:57:26 CEST 2014
	Author:           Daniel Lindekilde Ravn
	SDU email:        darav08@student.sdu.dk
	gmail:            daniel.ravn43@gmail.com
'''

import roslib; roslib.load_manifest('beginner_tutorials')
import rospy
import sensor_msgs.msg

class Visualization (object):

	def __init__(self):
		rospy.init_node('Visualization')
		rospy.loginfo(rospy.get_name() + ": I heard %s")
		laserMsg = rospy.get_param("laser_scan_topic", '/frobo/laser_msgs_1')
		rospy.Subscriber(laserMsg, sensor_msgs.msg.LaserScan , self.laserCallback)
		
		rospy.spin()
		pass

	def laserCallback(self,data):
		rospy.loginfo('laser callback')
		pass



class LaserNoise(object):
	'''
	classdocs
	'''
	
	def laserCallback(self,data):
#		 rospy.loginfo("laserCallback")
#		 rospy.loginfo(str(len(data.ranges)))
#		 value = gauss(0, 0.5)
#		 rospy.loginfo('values' + str(value))
		localTuple = ()
		for i in range(len(data.ranges)):
#			 rospy.loginfo('data:' + str(data.ranges[i]))
			localTuple += (data.ranges[i] + gauss(0, 0.5)/100,)
		data.ranges = localTuple
		self.laserNoise_pub.publish(data)
			

	def __init__(self):
		pass



if __name__ == '__main__':
	lolus = Visualization()
	pass
	
	

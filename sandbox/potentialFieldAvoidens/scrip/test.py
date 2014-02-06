#!/usr/bin/env python
'''
Created on 24/06/2013

@author: Daniel Lindekilde Ravn
'''
import roslib; roslib.load_manifest('potentialFieldAvoidens')
import rospy
from FroboMsgs.msg import fpga_data


class objectAvoidens():
    def __init__(self):
        rospy.init_node('objectAvodens')
        rospy.loginfo('Object avoidance is running')

if __name__ == '__main__':
    rospy.init_node('test_node')
    rospy.loginfo('Started test')
   # rospy.wait_for_service('/Qs')
   # prox = rospy.ServiceProxy('/Qs', PA10Qs)
   # no = int(raw_input('Et tal: '))
   # rospy.loginfo('Sending command')
   # res = prox([no,0,0,0,0,0,0])
   # rospy.loginfo('received result')
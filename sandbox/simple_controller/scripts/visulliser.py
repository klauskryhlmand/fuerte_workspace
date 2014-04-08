#!/usr/bin/env python


author

import roslib; roslib.load_manifest('beginner_tutorials')
import rospy
import sensor_msgs.msg
from random import gauss
# from sensor_msgs.msg import LaserScan

class LaserNoise(object):
    '''
    classdocs
    '''
    
    def laserCallback(self,data):
#         rospy.loginfo("laserCallback")
#         rospy.loginfo(str(len(data.ranges)))
#         value = gauss(0, 0.5)
#         rospy.loginfo('values' + str(value))
        localTuple = ()
        for i in range(len(data.ranges)):
#             rospy.loginfo('data:' + str(data.ranges[i]))
            localTuple += (data.ranges[i] + gauss(0, 0.5)/100,)
        data.ranges = localTuple
        self.laserNoise_pub.publish(data)
            

    def __init__(self):
        '''
        Constructor
        '''
        rospy.init_node('laserNoise')
        rospy.loginfo(rospy.get_name() + ": I heard %s")
        laserMsg = rospy.get_param("laser_scan_topic", 'laser_msgs_1')
        laserNoiseMsg = rospy.get_param("laser_scan_topic_noise", 'laser_msgs_1_noise')
        rospy.Subscriber(laserMsg, sensor_msgs.msg.LaserScan , self.laserCallback)
        
        self.laserNoise_pub = rospy.Publisher(laserNoiseMsg, sensor_msgs.msg.LaserScan)
        
        rospy.spin()
        
        



if __name__ == '__main__':
    lolus = LaserNoise()
    pass

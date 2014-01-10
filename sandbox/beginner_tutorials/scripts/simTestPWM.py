#!/usr/bin/env python
import sys
import serial
import time
import roslib; roslib.load_manifest('beginner_tutorials')
import rospy
from std_msgs.msg import String
from FroboMsgs.msg import fpga_data
from FroboMsgs.msg import pwm_o
import FroboMsgs



def talker():
     pub = rospy.Publisher('frobo/pwm', pwm_o)
     rospy.init_node('pwm_talker')    
     while not rospy.is_shutdown():
         rospy.loginfo("pwm_talker running")
         pub.publish(pwm_o(1,3,1,1,1,1,1))
         rospy.sleep(0.1)
 

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
         pass



# def talker():
#     pub = rospy.Publisher('pwm', pwm_o)
#     rospy.init_node('simTestPWM')
#     pwm_o.direction_left = 50
#     pwm_o.direction_right = 50
# 
#     pub.publish(pwm_o)
# 
# 
# 
# if __name__ == '__main__':
#     try:
#         #talker()
#         pub = rospy.Publisher('pwm', pwm_o)
#         rospy.init_node('simTestPWM')
#         r = rospy.Rate(10) # 10hz
#         myMsg = {}
#         
#         while not rospy.is_shutdown():
#         #    print "running"
#             stir = '12'
#             pwm_o.speed_left = int(stir,10)
#         #    print str
#             stir = str(pwm_o.speed_left)
#             #rospy.loginfo(stir)
#             rospy.loginfo(pwm_o.speed_left)
#             pwm_o.speed_right = int(stir,10)
#             pub.publish(pwm_o)
#             r.sleep()
#     except rospy.ROSInterruptException:
#         pass

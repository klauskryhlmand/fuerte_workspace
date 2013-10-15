'''
Created on 13/06/2013

@author: frobo
'''
import roslib; roslib.load_manifest('py_scripts')
import rospy
from std_msgs.msg import String


def talker():
    pub = rospy.Publisher('chatter', String)
    rospy.init_node('talker')
    while not rospy.is_shutdown():
        str = "hello world %s" % rospy.get_time()
        rospy.loginfo(str)
        pub.publish(String(str))
        rospy.sleep(1.0)


if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass


if __name__ == '__main__':
    pass

print "hello"
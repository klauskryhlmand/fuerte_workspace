#!/usr/bin/env python
import sys
import serial
import time
import roslib; roslib.load_manifest('beginner_tutorials')
import rospy
from std_msgs.msg import String
from FroboMsgs.msg import fpga_data
from FroboMsgs.msg import pwm_o

ser = serial.Serial(
'/dev/ttyUSB0',
baudrate=115200,
parity=serial.PARITY_NONE,
stopbits=serial.STOPBITS_ONE,
bytesize=serial.EIGHTBITS)
ser.isOpen()
str = "Connection open"
rospy.loginfo(str)

def pwmCallback(msg):
    #20Hex -> 10m/s
    speed_desired_left = hex(msg.speed_left)[2:]
    speed_desired_right = hex(msg.speed_right)[2:]
    dir_left = hex(msg.direction_left)[2:]
    dir_right = hex(msg.direction_right)[2:]
    e_left = hex(msg.enable_left)[2:]
    e_right = hex(msg.enable_right)[2:]
    
    in_cmd = "w04 " + speed_desired_left.zfill(4) + speed_desired_right.zfill(4) + dir_left + dir_right + e_left + e_right
    ser.write(in_cmd)

def talker():
    pub = rospy.Publisher('encoder_l', fpga_data)
    rospy.Subscriber('pwm', pwm_o, pwmCallback)
    rospy.init_node('seriel_data')
  #  while not rospy.is_shutdown():
        
    ser.write(b'r00')
    encoder_le = ser.read(size=9)
    str = "left encoder %s" % encoder_le
    #rospy.loginfo(str)
    fpga_data.encoder_l = int(encoder_le,16)
    
    ser.write(b'r01')
    encoder_ri = ser.read(size=9)
    str = "right encoder %s" % encoder_ri
    #rospy.loginfo(str)
    fpga_data.encoder_r = int(encoder_ri,16)
    
  #  ser.write(b'r04')
  #  pwm = ser.read(size=9)
  #  str = "pwm value %s" % pwm
  #  rospy.loginfo(str)
    #fpga_data.pwm_value = int(pwm,16)
  #  str = "hello worlds %s" % rospy.get_time()
    #rospy.loginfo(str)
    pub.publish(fpga_data)
  #  rospy.sleep(0.02)

"""
def talker():
    input = hex(40)[2:]
    print input.zfill(4)
    #print input
    input2 = hex(33)[2:]
    print input2.zfill(4)
    input3 = input.zfill(4) + input2.zfill(4)
    print input3
    #print input
   # while not rospy.is_shutdown():
    #    in_cmd = "w04 " + input.zfill(8)
   #     ser.write(in_cmd)
   #     rospy.sleep(1.0)
"""
if __name__ == '__main__':
    try:
        talker()
        ros.spin()
    except rospy.ROSInterruptException:
        pass



 #   pos = ser.read(size=9) #between 20000 - 6666 hex
 #   pos = hex(int(pos,16))
   
  #      in_cmd = bytes('w05 ' + perror.zfill(8), 'UTF-8')

    
   # ser.flushOutput()
   

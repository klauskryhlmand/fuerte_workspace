#!/usr/bin/env python
import rospy
from std_msgs.msg import Int16, Float64
   
import serial
import threading
import re
import time
import signal

class Microcontroller_connector:
	"""Microcontroller_connector"""
	def __init__(self):
		rospy.init_node('Microcontroller_connector')
		rospy.loginfo('Microcontroller connector started')
		self.serial = serial.Serial('/dev/ttyUSB0', baudrate=57600, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, bytesize=serial.EIGHTBITS)
		if self.serial.isOpen():
			print "Connection open"
#		pub = rospy.Publisher('encoder_l', fpga_data)
#		rospy.Subscriber('pwm', pwm_o, pwmCallback)
		self.talker()

	def pwmCallback(self,msg):
		pass
#		speed_desired_left = hex(msg.speed_left)[2:]
#		speed_desired_right = hex(msg.speed_right)[2:]
#		dir = hex(msg.direction)[2:]
#		en = hex(msg.enable)[2:]
#	
#		in_cmd = "w04 " + speed_desired_left.zfill(3) + speed_desired_right.zfill(3) + dir + en
#		ser.write(in_cmd)

	def talker(self):
		leftcounter = 0
		rightcounter = 0
		while not rospy.is_shutdown():
			firstl = self.serial.read(size=1)
			secondl = self.serial.read(size=1)
			high = ord(self.serial.read(size=1))
			low = ord(self.serial.read(size=1))
			if (high > 127):
				total = 65535-(high*256+low)			
			else:
				total = high*256+low
			rospy.loginfo(str(firstl)+str(secondl)+str(total))

			if (secondl == 'L'):
				if (high > 127):
					leftcounter = leftcounter - total
				else:
					leftcounter = total + leftcounter
			elif (secondl == 'R'):
				if (high > 127):
					rightcounter = rightcounter - total
				else:
					rightcounter = total + rightcounter
			rospy.loginfo('left: '+str(leftcounter*0.0005))
			rospy.loginfo('right: '+str(rightcounter*0.0005))
			rospy.loginfo('high: '+str(high))
			rospy.loginfo('low: '+str(low))
				
#			rospy.loginfo(str(self.serial.read(size=2)))
#			rospy.loginfo(str(int(self.serial.read(size=1),16)))
			#rospy.loginfo(str(int(self.serial.read(size=1),16)))
#			ser.write("r00\n")
#			Sleep(0.001)
#			encoder_le = int(ser.read(size=9),16)
#			str = "left encoder %s" % encoder_le
#			rospy.loginfo(str)
#			myFpga_data.encoder_l = encoder_le
#		
#			ser.write("r01\n")
#			Sleep(0.001)
#			encoder_ri = int(ser.read(size=9),16)
#			str = "right encoder %s" % encoder_ri
#			rospy.loginfo(str)
#			myFpga_data.encoder_r = encoder_ri
#		
#			pub.publish(myFpga_data)
			rospy.sleep(0.02)


if __name__ == '__main__':
	try:
		Microcontroller_connector = Microcontroller_connector()
	except rospy.ROSInterruptException:
		pass

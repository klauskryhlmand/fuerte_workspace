#!/usr/bin/env python
import roslib; roslib.load_manifest('beginner_tutorials')
import rospy
from std_msgs.msg import Int16, Float64
from FroboMsgs.msg import pwm_micro
import serial
import time


class Microcontroller_connector:
	"""Microcontroller_connector"""
	def __init__(self):
		rospy.init_node('Microcontroller_connector')
		rospy.loginfo('Microcontroller connector started')
		self.serial = serial.Serial('/dev/ttyUSB0', baudrate=57600, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, bytesize=serial.EIGHTBITS)
		if self.serial.isOpen():
			rospy.loginfo('Connection is open')
		else:
			rospy.loginfo('Connection failed to open')
#		pub = rospy.Publisher('encoder_l', fpga_data)
		rospy.Subscriber('pwm', pwm_micro, self.pwmCallback)
		self.talker()
		pass

	def pwmCallback(self,msg):
		rospy.loginfo('left pwmCallback was: ' + str(msg.speed_left))
		rospy.loginfo('right pwmCallback was: ' + str(msg.speed_right))
		speed_left = int(msg.speed_left * 100)
		speed_right = int(msg.speed_right * 100)
		if speed_left > 255:
			speed_left = 255
		if speed_right > 255:
			speed_right = 255
		msgMicroControler = 'SDSD' + chr(speed_left) + chr(speed_right) + 'f' +'f'
		self.serial.write(msgMicroControler)
#		rospy.loginfo('send msgMicroControler: ' + str(msgMicroControler))
		pass

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

			rospy.sleep(0.02)


if __name__ == '__main__':
	try:
		Microcontroller_connector = Microcontroller_connector()
	except rospy.ROSInterruptException:
		pass

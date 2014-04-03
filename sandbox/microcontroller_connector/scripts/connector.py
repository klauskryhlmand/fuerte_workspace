#!/usr/bin/env python
import roslib; roslib.load_manifest('beginner_tutorials')
import rospy
from std_msgs.msg import Int16, Float64
from FroboMsgs.msg import pwm_micro
from FroboMsgs.msg import micro_data
import serial
import time
import re

class Microcontroller_connector:
	"""Microcontroller_connector"""
	def __init__(self):
		rospy.init_node('Microcontroller_connector')
		rospy.loginfo('Microcontroller connector started')
		self.serial = serial.Serial('/dev/ttyUSB0', baudrate=57600, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, timeout=0, bytesize=serial.EIGHTBITS)
		
		if self.serial.isOpen():
			rospy.loginfo('Connection is open')
		else:
			rospy.loginfo('Connection failed to open')
		
		rospy.Subscriber('pwm', pwm_micro, self.pwmCallback)
		
		self.pub_distance = rospy.Publisher('distance', micro_data)
		
		self.__messagesLeftPartEnd = ''
		self.leftcounter = 0
		self.rightcounter = 0
		
		self.talker()
		pass

	def pwmCallback(self,msg):
		dir_l = ''
		dir_r = ''
		rospy.loginfo('left speed pwmCallback was: ' + str(msg.speed_left))
		rospy.loginfo('right speed pwmCallback was: ' + str(msg.speed_right))
		rospy.loginfo('left direction pwmCallback was: ' + str(msg.direction_left))
		rospy.loginfo('right direction pwmCallback was: ' + str(msg.direction_right))
		rospy.loginfo('left enable pwmCallback was: ' + str(msg.enable_left))
		rospy.loginfo('right enable pwmCallback was: ' + str(msg.enable_right))
		speed_left = int(msg.speed_left * 100)
		speed_right = int(msg.speed_right * 100)
		direction_l = int(msg.direction_left)
		direction_r = int(msg.direction_right)
		enable_l = int(msg.enable_left)
		enable_r = int(msg.enable_right)
		
		if enable_l == 1:
			enable_l = 'e'
		else:
			enable_l = 'd'
		
		if enable_r == 1:
			enable_r = 'e'
		else:
			enable_r = 'd'
		
		if direction_l == 0:
			dir_l = 'b'
		elif direction_l == 1:
			dir_l = 'f'
		if(direction_r == 0):
			dir_r = 'b'
		elif(direction_r  == 1):
			dir_r = 'f'
		if speed_left > 255:
			speed_left = 255
		if speed_right > 255:
			speed_right = 255
		msgMicroControler = 'SDSD' + chr(speed_left) + chr(speed_right) + dir_l + dir_r + enable_l + enable_r
		self.serial.write(msgMicroControler)
		self.serial.write(msgMicroControler)
		rospy.loginfo('send msgMicroControler: ' + str(msgMicroControler))
		pass



	def findMessages(self,messages):
		messages = self.__messagesLeftPartEnd + messages
		self.__messagesLeftPartEnd = ''
		handles = ['EL','ER','LP','RP','fl','bl','fr','br','er','el']
		messagesFound = []
		for i in handles:
			if i in messages:
				temp = [m.start() for m in re.finditer(i, messages)]
				for j in temp:
					if len(messages) < j + 4:
#						rospy.loginfo('j is: ' + str(j))
						self.__messagesLeftPartEnd = messages[j:len(messages)]
#						rospy.loginfo('rest was' + self.__messagesLeftPartEnd)
					else:
						messagesFound.append(messages[j:j+2] + str(self.hexReASemple(ord(messages[j+2]),ord(messages[j+3]))))
		return messagesFound
		pass

	def hexReASemple(self,high,low):
		temp = 0;
		if (high > 127):
			temp = 65535-(high*256+low)
		else:
			temp = high*256+low
		if (high > 127):
			temp = -temp
		return temp
		pass

	def messagesHandler(self,messages):
		for i in messages:
			if 'EL' in i:
				self.leftcounter = self.leftcounter + int(i[2:])
				msg = micro_data()
				msg.encoder_l = self.leftcounter * 0.0005
				msg.encoder_r = self.rightcounter * 0.0005
				self.pub_distance(msg)
			if 'ER' in i:
				self.rightcounter = int(i[2:]) + self.rightcounter
				msg = micro_data()
				msg.encoder_l = self.leftcounter * 0.0005
				msg.encoder_r = self.rightcounter * 0.0005
				self.pub_distance(msg)
#			if 'fl' in i:
#				rospy.loginfo('go forward left desired: ' + str(int(i[2:])*0.0005))
#				pass
#			if 'bl' in i:
#				rospy.loginfo('go back left desired: ' + str(int(i[2:])*0.0005))
#				pass
#			if 'fr' in i:
#				rospy.loginfo('go forward right desired: ' + str(int(i[2:])*0.0005))
#				pass
#			if 'br' in i:
#				rospy.loginfo('go back right desired: ' + str(int(i[2:])*0.0005))
#				pass
			if 'er' in i:
				rospy.loginfo('right error: ' + str(int(i[2:])))
				pass
			if 'el' in i:
				rospy.loginfo('left error: ' + str(int(i[2:])))
				pass
			if 'LP' in i:
				rospy.loginfo('Left PWM: ' + str(int(i[2:])))
				pass
			if 'RP' in i:
				rospy.loginfo('Right PWM: ' + str(int(i[2:])))
				pass
		pass


	def talker(self):
		slowDown = 0
		delay = 0
		while not rospy.is_shutdown():
			resived = self.serial.read(size=20)
			theMessages = self.findMessages(resived)
			self.messagesHandler(theMessages)
			
			if slowDown > 2:
				self.serial.write("RELR")
				self.serial.write("RERR")
				slowDown = 0
			else:
				slowDown = slowDown + 1
			
#			if delay > 200:
#				rospy.loginfo('left: '+str(self.leftcounter*0.0005))
#				rospy.loginfo('right: '+str(self.rightcounter*0.0005))
#				delay = 0
#			else:
#				delay = delay + 1
#				rospy.loginfo('left: '+str(self.leftcounter*0.0005))
#				rospy.loginfo('right: '+str(self.rightcounter*0.0005))
			rospy.sleep(0.02)


if __name__ == '__main__':
	try:
		Microcontroller_connector = Microcontroller_connector()
	except rospy.ROSInterruptException:
		pass

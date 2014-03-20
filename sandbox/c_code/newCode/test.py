#!/usr/bin/env python
#import sys
import serial
#import time

def talker():
	pass

if __name__ == '__main__':
	ser = serial.Serial(
	'/dev/ttyUSB0',
	baudrate=57600,
	parity=serial.PARITY_NONE,
	stopbits=serial.STOPBITS_ONE,
	bytesize=serial.EIGHTBITS)
	if ser.isOpen():
		print "Connection open"
		talker()
	else:
		print 'connection failed'
	ser.close()
	pass


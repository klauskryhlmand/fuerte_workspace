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
import cv2
import numpy as np
import math



class Visualization (object):

	def __init__(self):
		rospy.init_node('Visualization')
		rospy.loginfo(rospy.get_name() + ": I heard %s")
		laserMsg = rospy.get_param("laser_scan_topic", '/frobo/cordinats')
		self.lenght = rospy.get_param("lenght",300.0)
		self.width = rospy.get_param("width",300.0)
		rospy.Subscriber(laserMsg, sensor_msgs.msg.PointCloud , self.cordinatCallback)
		
		rospy.spin()
		pass

	def cordinatCallback(self,data):
		rospy.loginfo('cordinats callback')
		blank_image = np.zeros((self.width,self.lenght,3), np.uint8)
#		for i in xrange(len(data.points)):
#			rospy.loginfo('x: ' + str(data.points[i].x) + ' y: ' + str(data.points[i].y))
		self.putCordinatesInImages(data.points,blank_image)
		cv2.imshow("blank_image",blank_image)
		cv2.waitKey(1)
		pass

	def polarToCartesian(self,r,angle):
		return [r * math.cos(angle), r * math.sin(angle)]
		pass

	def testCordinatInImagesArea(self, cordinat):
		if cordinat.y > -(self.width/200.0/2.0) and cordinat.y < (self.width/200.0/2.0):
			if cordinat.x > -0.025 and cordinat.x < (self.lenght/200.0 - 0.5):
				return True
		pass

	def putCordinatesInImages(self,cordinats,img):
		for i in cordinats:
#			img[i.y*200+20,i.x*200+self.width/2] = [255,255,255]
			if self.testCordinatInImagesArea(i):
#				rospy.loginfo('y: ' + str(i.y) + ' x: ' + str(i.x))
				img[i.y*200 + self.width/2,i.x*200 + 100] = [255,255,255]
#				img[i.y*200 + self.width/2,0] = [255,255,255]
			
#			if x1 < self.lenght and y1 < self.width:
#				img[x1,y1] = [255,255,255]
#		for i in xrange(100):
#			for j in xrange(int(self.width)):
##				print 'x: ' + str(i+90) + ' y: ' + str(self.width-j)
#				img[j,i] = [0,0,0]
		gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
		
		
#		kernel = np.ones((3,3),np.uint8)
#		
#		dilation = cv2.dilate(img,kernel,iterations = 3)
#		erosion = cv2.erode(dilation,kernel,iterations = 4)
#		erosion = cv2.dilate(erosion,kernel,iterations = 1)
#		
#		cv2.imshow('dilation',dilation)
#		cv2.imshow('erosion',erosion)
#		
		edges = cv2.Canny(gray,50,250,apertureSize = 3)
#		edges = cv2.Canny(erosion,50,150,apertureSize = 3)
		
		

#		gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
#		edges = cv2.Canny(gray,150,200,apertureSize = 3)
		lines = cv2.HoughLines(edges,3,np.pi/180,75)
#		lines = cv2.HoughLines(gray,10,np.pi/180,90)
#		lines1 = cv2.HoughLines(gray,1,np.pi/180,13)
#		rospy.loginfo('size of lines: ' + str(len(lines)) + ' size of lines1: ' + str(len(lines1)))

		cv2.line(img,(0,int(self.width/2)),(int(self.lenght),int(self.width/2)),(0,0,255),2)
		for rho,theta in lines[0]:
			a = np.cos(theta)
			b = np.sin(theta)
#			if a < np.pi/8 and a > (np.pi/180)*5:
			if a < np.pi/8 or a > 0.1:
				x0 = a*rho
				y0 = b*rho
				x1 = int(x0 + 1000*(-b))   # Here i have used int() instead of rounding the decimal value, so 3.8 --> 3
				y1 = int(y0 + 1000*(a))    # But if you want to round the number, then use np.around() function, then 3.8 --> 4.0
				x2 = int(x0 - 1000*(-b))   # But we need integers, so use int() function after that, ie int(np.around(x))
				y2 = int(y0 - 1000*(a))
				cv2.line(img,(x1,y1),(x2,y2),(0,255,0),2)
		pass


if __name__ == '__main__':
	lolus = Visualization()
	pass
	
	

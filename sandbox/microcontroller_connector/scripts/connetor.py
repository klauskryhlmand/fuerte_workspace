#!/usr/bin/env python

import rospy
from std_msgs.msg import Int16, Float64
   
import serial
import threading
import re
import time
import signal

class Controller:
    """Microcontroller_connector"""
    def __init__(self):
        rospy.init_node('Microcontroller_connector')
        rospy.loginfo('Microcontroller connector started')

        self.serial = serial.Serial('/dev/ttyUSB0', baudrate=57600, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, bytesize=serial.EIGHTBITS)
        if self.serial.isOpen():
            print "Connection open"

        self.done = False
        signal.signal(signal.SIGINT, self.sigint_proc)

        # create listener
        thread = threading.Thread(target=self.nonBlockingListener, args=())
        thread.start()
        
#        # create subscriber
#        rospy.Subscriber(set_pos_topic, Int16, self.set_pos)
#        # create publisher
#        self.pos_pub = rospy.Publisher(get_pos_topic, Float64)
        # create timer
        rospy.spin()

    def sigint_proc(self, signum, frame):
        rospy.logerr('Rexroth caught SIGNAL, exiting!')
        self.done = True


    def listenSimpleBlocked(self):
        done = False
        while(not done):
            data = self.serial.read(self.serial.inWaiting())
            if len(data)>0:
                print data
                done = True
    
    def nonBlockingListener(self):
        data = ''
        printlist = ''
        catch = False
        while(not self.done):
            c = self.serial.read(1)
            if re.match(r'T', c):
                print c
                if len(printlist)>0:
                    if re.search(r'S-0-0051', printlist):
                        catch = True
                    elif catch:
                        if re.search(r'\+', printlist):
                            printlist = printlist[1:]
                        self.current_pos = float(printlist)
                        self.pos_pub.publish(self.current_pos)
                        catch = False
                    else:
                        print ">>\t" + printlist
                printlist=''
            else:
                printlist += c
            
            
            if (re.search(r':>', printlist)):
                #print ">>\t" + printlist
                printlist=''
            time.sleep(0.01)
                
if __name__ == '__main__':
    try:
        controller = Controller()
    except rospy.ROSInterruptException:
        pass

#!/usr/bin/env python
import time
import serial
import glob
import os
import datetime
 
def my_callback(channel):

    ser.write((bytes(str("p"), 'utf-8')))

    if GPIO.input(channel) == GPIO.HIGH:
        print('\n▼  at ' + str(datetime.datetime.now()))
    else:
        print('\n ▲ at ' + str(datetime.datetime.now()))

#/home/pi/vast_ai/dream_machine/out_imgs
def wait_new_file(folder):
    """
    Wait for new files to appear in the folder.
    """
    # wait for new files to appear in the FTP folder
    files_old = glob.glob("{}/*".format(folder))
    files_new = []
    # filecount = len(files_old)
    print("Waiting for new files to appear in the FTP folder...")
    while True:
        files_new = glob.glob("{}/*".format(folder))
        filecount = len(files_new)
        if filecount > len(files_old):
            break
        time.sleep(0.01)

    # get new file by unique intersection
    new_file = str(list(set(files_new) - set(files_old))[0])
    print("New file found: " + str(new_file))
    while True:
        if os.path.isfile(str(new_file)): break
        time.sleep(0.01)
    return new_file

import RPi.GPIO as GPIO 
from time import sleep 
GPIO.setwarnings(False) 

ser = serial.Serial(
        port='/dev/ttyS0', #Replace ttyS0 with ttyAM0 for Pi1,Pi2,Pi0
        baudrate = 9600,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=1
)


try:
	GPIO.setmode(GPIO.BCM) 
	GPIO.setup(26, GPIO.IN, pull_up_down=GPIO.PUD_UP) 
	GPIO.add_event_detect(26, GPIO.BOTH, callback=my_callback)

	while True:
		wait_new_file("/home/pi/vast_ai/dream_machine/out_imgs")
		ser.write((bytes(str("0"), 'utf-8')))
		time.sleep(1)


finally:
    GPIO.cleanup()
 
print("Goodbye!")







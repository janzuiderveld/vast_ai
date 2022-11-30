#!/usr/bin/env python
import time
import serial


import RPi.GPIO as GPIO 
from time import sleep 
GPIO.setwarnings(False) 
GPIO.setmode(GPIO.BCM) 
GPIO.setup(26, GPIO.IN, pull_up_down=GPIO.PUD_UP) 

ser = serial.Serial(
        port='/dev/ttyS0', #Replace ttyS0 with ttyAM0 for Pi1,Pi2,Pi0
        baudrate = 9600,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=1
)

while True: 
    if 1:
        button_v = (GPIO.input(26))  
        print (button_v)
        sleep(0.01)
        ser.write((bytes(str(button_v), 'utf-8')))




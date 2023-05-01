import time
import RPi.GPIO as GPIO

relay_ch = 21

relay_dict = {"1":21,
              "2":22,
              "3":23,
              "4":24,
              "5":25,
              "6":26,
              "7":27,
              }

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.setup(relay_ch, GPIO.OUT)

print("ye")
GPIO.output(relay_ch, GPIO.LOW)
time.sleep(1)
print("ye")
GPIO.output(relay_ch, GPIO.HIGH)

GPIO.cleanup()

try:
    while True:
        relay_ch = input("Enter relay number (1-7): ")

        time.sleep(1)
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(relay_ch, GPIO.OUT)

        GPIO.output(relay_ch, GPIO.LOW)
        time.sleep(1)
        GPIO.output(relay_ch, GPIO.HIGH)
        

        GPIO.cleanup()

except:
    GPIO.cleanup()


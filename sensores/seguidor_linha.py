from RPi import GPIO
from time import sleep

GPIO.setmode(GPIO.BCM)

sensor_left = 17
sensor_right = 27

GPIO.setup(sensor, GPIO.IN)

try:
	while True:
        if not GPIO.input(sensor_left):
            print("Robot is straying off to the right, move left captain!")
        elif not GPIO.input(sensor_right):
            print("Robot is straying off to the left, move right captain!")
        else:
            print("Following the line!")
		sleep(0.2)
except:
	GPIO.cleanup()

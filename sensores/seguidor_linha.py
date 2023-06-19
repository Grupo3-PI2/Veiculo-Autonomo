from RPi import GPIO
from time import sleep

GPIO.setmode(GPIO.BCM)

sensor = 17

GPIO.setup(sensor, GPIO.IN)

try:
	while True:
		if not GPIO.input(sensor):
			print("Saiu da linha preta!")
		else:
			print("Following the line!: está no pretp")
		sleep(0.2)
except:
	GPIO.cleanup()

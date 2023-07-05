import RPi.GPIO as GPIO
from time import sleep

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
Ena, In1, In2 = 22, 13, 19
GPIO.setup(Ena, GPIO.OUT)
GPIO.setup(In1, GPIO.OUT)
GPIO.setup(In2, GPIO.OUT)
pwm = GPIO.PWM(Ena, 100)
pwm.start(0)

speed = 50;

while True:
    GPIO.output(In1,GPIO.LOW)
    GPIO.output(In2,GPIO.HIGH)
    if speed > 100 :
        speed = 100
    pwm.ChangeDutyCycle(speed)

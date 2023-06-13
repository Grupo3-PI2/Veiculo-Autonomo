import RPi.GPIO as GPIO  #importando as bibliotecas de GPIO 
import time  # bliblioteca com recursos de manejar o tempo, pausar os script e etc
 
#GPIO Mode (BOARD / BCM) Aqui escolhemos se vamos usar a pinagem BOARD ou BCM
GPIO.setmode(GPIO.BCM)  
 
#set GPIO Pins
SD1_GPIO_TRIGGER = 23
SD1_GPIO_ECHO = 24
SD2_GPIO_TRIGGER = 25
SD2_GPIO_ECHO = 26

#set GPIO direction (IN / OUT)
GPIO.setup(SD1_GPIO_TRIGGER, GPIO.OUT)#trigger do sd1
GPIO.setup(SD1_GPIO_ECHO, GPIO.IN)#echo do sd1

GPIO.setup(SD2_GPIO_TRIGGER, GPIO.OUT)#triger do sd2
GPIO.setup(SD2_GPIO_ECHO, GPIO.IN)#echo sd2

 
def distance(GPIO_TRIGGER,GPIO_ECHO):
    # set Trigger to HIGH
    GPIO.output(GPIO_TRIGGER, True)
 
    # set Trigger after 0.01ms to LOW
    time.sleep(0.00001)
    GPIO.output(GPIO_TRIGGER, False)
 
    StartTime = time.time()
    StopTime = time.time()
 
    # save StartTime
    while GPIO.input(GPIO_ECHO) == 0:
        StartTime = time.time()
 
    # save time of arrival
    while GPIO.input(GPIO_ECHO) == 1:
        StopTime = time.time()
 
    # time difference between start and arrival
    TimeElapsed = StopTime - StartTime
    # multiply with the sonic speed (34300 cm/s)
    # and divide by 2, because there and back
    distance = (TimeElapsed * 34300) / 2
 
    return distance
 
if __name__ == '__main__':
    try:
        while True:
            dist = distance(SD1_GPIO_TRIGGER,SD1_GPIO_ECHO)
            print ("Measured Distance de SD1 = %.1f cm" % dist)
            time.sleep(0.001)
            dist = distance(SD2_GPIO_TRIGGER,SD2_GPIO_ECHO)
            print ("Measured Distance de SD2 = %.1f cm" % dist)
            time.sleep(0.001)

        # Reset by pressing CTRL + C
    except KeyboardInterrupt:
        print("Measurement stopped by User")
        GPIO.cleanup()

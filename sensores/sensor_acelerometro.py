#Para rodar esse código tem que atender essas dependencias na Rasp 
#> sudo apt install python3-smbus
#> pip install mpu6050-raspberrypi
from mpu6050 import mpu6050 #declara a biblioteca com as funções prontas
import time
mpu = mpu6050(0x68) #seleciona o registrador padrão do I2C

while True:
    print("Temp : "+str(mpu.get_temp()))
    print()

    accel_data = mpu.get_accel_data() #aceleração é dada em m/s. tem que pegar espaços de tempo pra calcular a velocidade
    print("Acc X : "+str(accel_data['x']))
    print("Acc Y : "+str(accel_data['y']))
    print("Acc Z : "+str(accel_data['z']))
    print()

    gyro_data = mpu.get_gyro_data()
    print("Gyro X : "+str(gyro_data['x']))
    print("Gyro Y : "+str(gyro_data['y']))
    print("Gyro Z : "+str(gyro_data['z']))
    print()
    print("-------------------------------")
    time.sleep(1)


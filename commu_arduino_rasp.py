import serial
import time

s = serial.Serial('/dev/ttyUSB0', 9600) # altere o nome, se necessário

if not s.isOpen(): # caso a porta não esteja aberta
    s.open()

time.sleep(5) # o Arduino é redefinido após a ativação da conexão serial

try:
    while True:
        response = s.readline().decode('utf-8')  # decodifica a sequência de bytes para uma string
        print(response)
except KeyboardInterrupt:
    s.close()

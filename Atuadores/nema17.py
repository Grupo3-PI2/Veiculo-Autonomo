
from time import sleep
import RPi.GPIO as GPIO
#mport serial
from time import sleep

#definição dos pinos e principais variáveis
PPR = 0   
passo = 0   
temp = 1000 #TEMPO DOS PASSOS 


#ENA = 
MS1 = 17
MS2 = 27
MS3 = 22
#RST = 9
#SLP = 8
STP = 13
DIR = 6
PPR = 500 #LIMITE PARA NÃO FORÇAR A ESTRUTURA
print('antes da pinagem')
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(DIR, GPIO.OUT)

#GPIO.setup(ENA, GPIO.OUT)
GPIO.setup(MS1, GPIO.OUT)
GPIO.setup(MS2, GPIO.OUT)
GPIO.setup(MS3, GPIO.OUT)
GPIO.setup(STP, GPIO.OUT)

#GPIO.output(SLP, GPIO.LOW)
GPIO.output(DIR, GPIO.HIGH)
#GPIO.output(RST, GPIO.HIGH)

#ATIVANDO RESOLUÇÕES DE PASSO
GPIO.output(MS1, GPIO.HIGH)
GPIO.output(MS2, GPIO.HIGH)
GPIO.output(MS3, GPIO.HIGH)

print('pinagem definida')
#DESLIGA O DRIVER
def OFF_D():
  GPIO.output(ENA, GPIO.LOW)
  sleep(1/100)
  return

#RESETA O DRIVER
def rst(): 
  GPIO.output(RST, GPIO.LOW)
  sleep(1/100)
  GPIO.output(RST, GPIO.HIGH)
  sleep(1/100)
  print("DRIVER RESETADO")
  return

#RESOLUÇÃO DE PASSO 
def Resolucao():
  PPR = 500
  print("Modo Sixteenth - STEP (1 / 16)")
  GPIO.output(MS1, GPIO.HIGH)
  sleep(1/100)
  GPIO.output(MS2, GPIO.HIGH)
  sleep(1/100)
  GPIO.output(MS3, GPIO.HIGH)
  sleep(1/100)
  return

#IR PARA DIREITA COM O AUMENTO DO PASSO
def VD():
  passo = 0
  #GPIO.output(ENA, GPIO.HIGH)
  sleep(1/100)
  #GPIO.output(DIR, GPIO.HIGH)
  sleep(1/100)
  PPR = 500

  while (PPR > passo): #Enquanto PPR for maior que passo Avança o passo
    GPIO.output(STP, GPIO.LOW)
    sleep(1) 
    GPIO.output(STP, GPIO.HIGH)
    sleep(1)
    passo = passo+1
  return

#IR PARA ESUERDA COM O AUMENTO DO PASSO
def VE():
  passo = 0
  print('VE')
  #GPIO.output(ENA, GPIO.HIGH)
  #sleep(1/100)
  #GPIO.output(DIR, GPIO.LOW)
  #sleep(1/100)

  while (PPR > passo): #Enquanto PPR for maior que passo Avança o passo
    GPIO.output(STP, GPIO.LOW)
    sleep(0.01) 
    GPIO.output(STP, GPIO.HIGH)
    sleep(0.01)
    passo = passo+1
    print('adioione um passo')
  return

i = 0
#print('funções definidas')
#while(i < 5):
 #   VE()
  #  print('entrou')
   # sleep(1)
    #VD()
    #i = i+1


GPIO.output(ENA, GPIO.HIGH)

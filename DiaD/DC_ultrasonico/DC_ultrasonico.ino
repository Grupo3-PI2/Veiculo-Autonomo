// ULTRASSONICO + DC

#include "Ultrasonic.h"
#include <MPU6050_tockn.h>
#include <Wire.h>
/*---- DECLARAÇÃO ULTRASSÔNICO 1 E 2---- */
#define echoPin  11 //PINO DIGITAL UTILIZADO PELO HC-SR04 ECHO(RECEBE)
#define trigPin  10 //PINO DIGITAL UTILIZADO PELO HC-SR04 TRIG(ENVIA)
#define echoPin2  13 //PINO DIGITAL UTILIZADO PELO HC-SR04 ECHO(RECEBE)
#define trigPin2  12 //PINO DIGITAL UTILIZADO PELO HC-SR04 TRIG(ENVIA)

Ultrasonic ultrasonic(trigPin, echoPin); //INICIALIZANDO OS PINOS DO ARDUINO
Ultrasonic ultrasonic2(trigPin2, echoPin2); //INICIALIZANDO OS PINOS DO ARDUINO
MPU6050 mpu6050(Wire); // declaraçãod e objeto Acelerômetro
String read_msg; // Leitura do monitor serial
bool stateDC = true;// estado do motor DC (andando ou parado)[talvez não seja preciso]

int distancia; //VARIÁVEL DO TIPO INTEIRO
int distancia_final; //VARIÁVEL DO TIPO INTEIRO
int distancia_2; //VARIÁVEL DO TIPO INTEIRO


String result; //VARIÁVEL DO TIPO STRING
String result_2; //VARIÁVEL DO TIPO STRING

int v = 0; // VARIÁVEL DA VELOCIDADE DO CARRINHO

// Iremos fazer uma classe para facilitar o uso da ponte H L298N na manipulação dos motores na função Setup e Loop.
class DCMotor {
    int spd = 255, pin1, pin2;

  public:

    void Pinout(int in1, int in2) { // Pinout é o método para a declaração dos pinos que vão controlar o objeto motor
      pin1 = in1;
      pin2 = in2;
      pinMode(pin1, OUTPUT);
      pinMode(pin2, OUTPUT);
    }
    void Speed(int in1) { // Speed é o método que irá ser responsável por salvar a velocidade de atuação do motor
      spd = in1;
    }
    void Forward() { // Forward é o método para fazer o motor girar para frente
      analogWrite(pin1, spd);
      digitalWrite(pin2, LOW);
    }
    void Backward() { // Backward é o método para fazer o motor girar para trás
      digitalWrite(pin1, LOW);
      analogWrite(pin2, spd);
    }
    void Stop() { // Stop é o metodo para fazer o motor ficar parado.
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, LOW);
    }
};
DCMotor Motor2; // Criação de dois objetos motores, já que usaremos dois motores, e eles já estão prontos para receber os comandos já configurados acima.
void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  Motor2.Pinout(5, 6);
  pinMode(echoPin, INPUT); //DEFINE O PINO COMO ENTRADA (RECEBE)
  pinMode(trigPin, OUTPUT); //DEFINE O PINO COMO SAIDA (ENVIA)
  Wire.begin();
  mpu6050.begin();// inicia o acelerômetro
  mpu6050.calcGyroOffsets(true);//calibra o acelerômetro

}

void hcsr04() {
  digitalWrite(trigPin, LOW); //SETA O PINO 6 COM UM PULSO BAIXO "LOW"
  delayMicroseconds(2); //INTERVALO DE 2 MICROSSEGUNDOS
  digitalWrite(trigPin, HIGH); //SETA O PINO 6 COM PULSO ALTO "HIGH"
  delayMicroseconds(10); //INTERVALO DE 10 MICROSSEGUNDOS
  digitalWrite(trigPin, LOW); //SETA O PINO 6 COM PULSO BAIXO "LOW" NOVAMENTE
  //FUNÇÃO RANGING, FAZ A CONVERSÃO DO TEMPO DE
  //RESPOSTA DO ECHO EM CENTIMETROS, E ARMAZENA
  //NA VARIAVEL "distancia"
  distancia = (ultrasonic.Ranging(CM)); //VARIÁVEL GLOBAL RECEBE O VALOR DA DISTÂNCIA MEDIDA
  result = String(distancia); //VARIÁVEL GLOBAL DO TIPO STRING RECEBE A DISTÂNCIA(CONVERTIDO DE INTEIRO PARA STRING)
  delay(100); //INTERVALO DE 500 MILISSEGUNDOS
}

void hcsr04_2() {
  digitalWrite(trigPin2, LOW); //SETA O PINO 6 COM UM PULSO BAIXO "LOW"
  delayMicroseconds(2); //INTERVALO DE 2 MICROSSEGUNDOS
  digitalWrite(trigPin2, HIGH); //SETA O PINO 6 COM PULSO ALTO "HIGH"
  delayMicroseconds(10); //INTERVALO DE 10 MICROSSEGUNDOS
  digitalWrite(trigPin2, LOW); //SETA O PINO 6 COM PULSO BAIXO "LOW" NOVAMENTE
  //FUNÇÃO RANGING, FAZ A CONVERSÃO DO TEMPO DE
  //RESPOSTA DO ECHO EM CENTIMETROS, E ARMAZENA
  //NA VARIAVEL "distancia"
  distancia_2 = (ultrasonic.Ranging(CM)); //VARIÁVEL GLOBAL RECEBE O VALOR DA DISTÂNCIA MEDIDA
  result_2 = String(distancia_2); //VARIÁVEL GLOBAL DO TIPO STRING RECEBE A DISTÂNCIA(CONVERTIDO DE INTEIRO PARA STRING)
  delay(100); //INTERVALO DE 500 MILISSEGUNDOS
}



void loop() {
  hcsr04();
  hcsr04_2();

  if (distancia_2 > distancia) {
    distancia_final = distancia;
  }
  else {
    distancia_final = distancia_2;
  }

  if (Serial.available()) {// recebe a msg via serial e julga de acordo
    read_msg = Serial.readString();
    if (read_msg[0] == 'G')//Aciona o motor DC(podemos trocar pela função Motor2.Forward
      stateDC = true;
    else if (read_msg[0] == 'S')//Desliga o motor DC(podemos trocar pela função Motor2.Stop
      stateDC = false;
    else if (read_msg[0] == 'U') { //atualiza e printa os dados do MPU
      mpu6050.update();
      printMpuData();
    }
    else //Aponta erro de mensagem estranha
      Serial.println(F("Erro de mensagem não padronizada recebida"));


  Serial.print("Distancia 1:"); //IMPRIME O TEXTO NO MONITOR SERIAL
  Serial.print(result);
  ////IMPRIME NO MONITOR SERIAL A DISTÂNCIA MEDIDA
  Serial.println("cm"); //IMPRIME O TEXTO NO MONITOR SERIAL

  Serial.print("Distancia 2:"); //IMPRIME O TEXTO NO MONITOR SERIAL
  Serial.print(result_2);
  ////IMPRIME NO MONITOR SERIAL A DISTÂNCIA MEDIDA
  Serial.println("cm"); //IMPRIME O TEXTO NO MONITOR SERIAL

  if (distancia < 70 || distancia_2 < 70 ) {
    Motor2.Stop();
    delay(1000);
  }
  else if (distancia < 90 || distancia_2 < 90 ) {
    Motor2.Speed(80);// A velocidade do motor pode variar de 64 a 255, onde 255 é a velocidade máxima e 64 o mínimo.
    Motor2.Backward();
    delay(10);
  }
  else if (distancia < 110 || distancia_2 < 110) {
    Motor2.Speed(100);// A velocidade do motor pode variar de 64 a 255, onde 255 é a velocidade máxima e 64 o mínimo.
    Motor2.Backward();
    delay(10);
  }

  else if (distancia < 130 || distancia_2 < 130) {
    Motor2.Speed(130);// A velocidade do motor pode variar de 64 a 255, onde 255 é a velocidade máxima e 64 o mínimo.
    Motor2.Backward();
    delay(10);
  }
  else {
    Motor2.Speed(160);// A velocidade do motor pode variar de 64 a 255, onde 255 é a velocidade máxima e 64 o mínimo.
    Motor2.Backward();
    delay(10);
  }

}
}

void printMpuData() {
  Serial.println(F("============INIT DATA MPU====================="));
  //    Serial.print("temp : ");Serial.println(mpu6050.getTemp());
  Serial.print(F("accX : ")); Serial.println(mpu6050.getAccX());
  Serial.print(F("accY : ")); Serial.println(mpu6050.getAccY());
  Serial.print(F("accZ : ")); Serial.println(mpu6050.getAccZ());
  //    Serial.print("gyroX : ");Serial.print(mpu6050.getGyroX());
  //    Serial.print("gyroY : ");Serial.print(mpu6050.getGyroY());
  //    Serial.print("gyroZ : ");Serial.println(mpu6050.getGyroZ());
  //    Serial.print("accAngleX : ");Serial.print(mpu6050.getAccAngleX());
  //    Serial.print("\taccAngleY : ");Serial.println(mpu6050.getAccAngleY());
  //    Serial.print("gyroAngleX : ");Serial.print(mpu6050.getGyroAngleX());
  //    Serial.print("\tgyroAngleY : ");Serial.print(mpu6050.getGyroAngleY());
  //    Serial.print("\tgyroAngleZ : ");Serial.println(mpu6050.getGyroAngleZ());
  Serial.print(F("angleX : ")); Serial.println(mpu6050.getAngleX());
  Serial.print(F("angleY : ")); Serial.println(mpu6050.getAngleY());
  Serial.print(F("angleZ : ")); Serial.println(mpu6050.getAngleZ());
  Serial.println(F("============+END DATA MPU=====================\n"));
}

// distancia > 80 -> Vou para 80v
//  80 < distancia > 100 -> Vou para 100v
// distanci > 100 --> Vou para 155

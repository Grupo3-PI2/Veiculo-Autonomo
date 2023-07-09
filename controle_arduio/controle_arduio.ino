//#include <HCSR04.h>
#include <Wire.h>
#include "Ultrasonic.h"

#define ENA 7 /* Enable ativa os drivers do motor */
#define MS1 6 /* Seleciona a resolução do passo */
#define MS2 4 /* Seleciona a resolução do passo   */
#define MS3 3 /* Seleciona a resolução do passo */
//#define RST 9 /* Desativa os drivers de saída   */
//#define SLP 8 /* Inicia um descanso para algumas funções do drive */
#define DIR 2 /* Direção do passo  */
#define STP 5 /* Avanço do passo */
#define TRIGGER_SL   11
#define ECHO_SL      10
#define TRIGGER_SR   13
#define ECHO_SR      12
#define linha_L      9
#define linha_R      8
#define motPin1 14
#define motPin2 15

//#define max_passo 500 /* pulsos por resolução */
#define temp 1000

bool sen_linha_L = 0;
bool sen_linha_R = 0;
int dist = 0;
int distancia = 0;

/*---- Variáveis de controle motor de passo---- */
int passo = 0;    /* passos */

String result; //VARIÁVEL DO TIPO STRING
String result_2; //VARIÁVEL DO TIPO STRING

void print_speed_dist();
void esquerda();
void direita();
// Inicializa o sensor usando os pinos TRIGGER and ECHO.
//UltraSonicDistanceSensor sen_dist_L(TRIGGER_SL, ECHO_SL), sen_dist_R(TRIGGER_SR, ECHO_SR);
Ultrasonic sen_distL(11, 10);
Ultrasonic sen_distR(13, 12);

void mpu_begin();
void motoPassoBegin();
void getMpuData();
void pegar_distancias();

//Endereco I2C do MPU6050
const int MPU = 0x68;
//Variaveis para armazenar valores dos sensores
int AcX, AcY, AcZ, GyX, GyY, GyZ;

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
  //  motoPassoBegin();
  //Setamos os pinos dos seguidores de linha como entrada
  pinMode(linha_L, INPUT);
  pinMode(linha_R, INPUT);
//  pinMode(TRIGGER_SL, OUTPUT);
//  pinMode(ECHO_SL, INPUT);
//  sen_distL.Ultrasonic(TRIGGER_SL,ECHO_SL);
//  pinMode(TRIGGER_SR, OUTPUT);
//  pinMode(ECHO_SR, INPUT);
//  sen_distR.Ultrasonic(TRIGGER_SR,ECHO_SR);
  
  ///////////pinagem do motor de passo
  motoPassoBegin();

  //begin do motor DC
  Motor2.Pinout(motPin1, motPin2);
  Serial.begin(9600);
  while (!Serial)
    ;
  Serial.print(F("Comunicacao serial estabelecida \nInicializando componentes"));
  ///MPU begin
//  mpu_begin();
  // sensor de distância não precisa de inicialização

  Serial.println(F("..INIT.."));
}

void loop() {
  // put your main code here, to run repeatedly:

  //////////////////////////////////////////////////
  // MPU
  //  getMpuData();
  ////////////////////////////////////////////////////////////
  // sensores de Distancia
  pegar_distancias();

  // Seguidor de linha
  sen_linha_L = digitalRead(linha_L);
  sen_linha_R = digitalRead(linha_R);
  Serial.print(F("Linha esquerda")); Serial.println(sen_linha_L);
  Serial.print(F("Linha direita")); Serial.println(sen_linha_R);
  if (sen_linha_L) {
    esquerda();
  }
  else if (sen_linha_R) {
    direita();
  }
  else {
    Serial.println(F("Estou seguindo a linha"));
  }
  if (distancia < 70 || dist < 70 ) {
    Motor2.Stop();
    delay(10);
    print_speed_dist();
  }
  else if (distancia < 90 || dist < 90 ) {
    Motor2.Speed(80);// A velocidade do motor pode variar de 64 a 255, onde 255 é a velocidade máxima e 64 o mínimo.
    Motor2.Backward();
    print_speed_dist();
    delay(10);
  }
  else if (distancia < 110 || dist < 110) {
    Motor2.Speed(100);// A velocidade do motor pode variar de 64 a 255, onde 255 é a velocidade máxima e 64 o mínimo.
    Motor2.Backward();
    print_speed_dist();
    delay(10);
  }

  else if (distancia < 130 || dist < 130) {
    Motor2.Speed(130);// A velocidade do motor pode variar de 64 a 255, onde 255 é a velocidade máxima e 64 o mínimo.
    Motor2.Backward();
    print_speed_dist();
    delay(10);
  }
  else {
    Motor2.Speed(160);// A velocidade do motor pode variar de 64 a 255, onde 255 é a velocidade máxima e 64 o mínimo.
    Motor2.Backward();
    print_speed_dist();
    delay(10);
  }
}

void print_speed_dist() {
  Serial.println(dist);
  Serial.println(distancia);
  Serial.println(v);

}

//  delay(500);



////Valores distâncias////////////////////////////////////////////////////////
void pegar_distancias() {
  //Cria variavel do tipo int

  //Variável recebe o valor da função da biblioteca
  //  distancia = sen_dist_L.measureDistanceCm();
  hcsr04_L();
  hcsr04_R();
  Serial.print(F("L_dist: "));
  Serial.println(distancia);
  //  dist = sen_dist_R.measureDistanceCm();
  Serial.print(F("R_dist: "));
  Serial.println(dist);
  if (dist >= distancia)
    dist = distancia;

}
void mpu_begin() {
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);

  //Inicializa o MPU-6050
  Wire.write(0);
  Wire.endTransmission(true);
}

void motoPassoBegin() {
  pinMode(ENA, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  pinMode(STP, OUTPUT);
  pinMode(DIR, OUTPUT);
  //  digitalWrite(SLP, LOW);   /* Sleep desabilitado */
  digitalWrite(DIR, HIGH);  /* Sentido Horário habilitado */
  //  digitalWrite(RST, HIGH);  /* RST habilitado */
}
void getMpuData() {
  // MPU
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  //Solicita os dados do sensor
  Wire.requestFrom(MPU, 14, true);
  //Armazena o valor dos sensores nas variaveis correspondentes
  AcX = Wire.read() << 8 | Wire.read(); //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  //  Tmp = Wire.read() << 8 | Wire.read(); //0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX = Wire.read() << 8 | Wire.read(); //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  Serial.print(F("AcX: ")); Serial.println(AcX);
  Serial.print(F("AcY: ")); Serial.println(AcY);
  Serial.print(F("AcZ: ")); Serial.println(AcZ);
  //  Serial.print(F("Tmp: ")); Serial.println(Tmp);
  Serial.print(F("GyX: ")); Serial.println(GyX);
  Serial.print(F("GyY: ")); Serial.println(GyY);
  Serial.print(F("GyZ: ")); Serial.println(GyZ);
}


////////////////////////////////



void direita() {
  // max_passo = 500; // limete para não quebrar o volante
  // passo = 0;
  passo++;
  if (passo >= 500) {
    passo = 500;
  }
  else {
    Serial.println("Modo Sixteenth - STEP (1 / 16)");
    digitalWrite(MS1, HIGH);
    digitalWrite(MS2, HIGH);
    digitalWrite(MS3, HIGH);
    Serial.println("Sentido - Horario");
    digitalWrite(DIR, HIGH);
    digitalWrite(STP, LOW);
    delayMicroseconds(temp); /* Tempo em Microseconds */
    digitalWrite(STP, HIGH);
    delayMicroseconds(temp);
    ena2();
    delay(20);
  }
}

void esquerda() {
  // max_passo = 500; // limete para não quebrar o volante
  // passo = 0;
  passo--;
  if (passo <= -500) {
    passo = -500;
  }
  else {
    Serial.println("Modo Sixteenth - STEP (1 / 16)");
    digitalWrite(MS1, HIGH);
    digitalWrite(MS2, HIGH);
    digitalWrite(MS3, HIGH);
    Serial.println("Sentido - Horario");
    digitalWrite(DIR, LOW);
    digitalWrite(STP, LOW);
    delayMicroseconds(temp); /* Tempo em Microseconds */
    digitalWrite(STP, HIGH);
    delayMicroseconds(temp);
    ena2();
    delay(20);
  }
}

void ena2() {
  digitalWrite(ENA, HIGH);  /* Ativa o A4988 */
  delay(10);
}
void hcsr04_L() {
  digitalWrite(TRIGGER_SL, LOW); //SETA O PINO 6 COM UM PULSO BAIXO "LOW"
  delayMicroseconds(2); //INTERVALO DE 2 MICROSSEGUNDOS
  digitalWrite(TRIGGER_SL, HIGH); //SETA O PINO 6 COM PULSO ALTO "HIGH"
  delayMicroseconds(10); //INTERVALO DE 10 MICROSSEGUNDOS
  digitalWrite(TRIGGER_SL, LOW); //SETA O PINO 6 COM PULSO BAIXO "LOW" NOVAMENTE
  //FUNÇÃO RANGING, FAZ A CONVERSÃO DO TEMPO DE
  //RESPOSTA DO ECHO EM CENTIMETROS, E ARMAZENA
  //NA VARIAVEL "distancia"
  distancia = (sen_distL.Ranging(CM)); //VARIÁVEL GLOBAL RECEBE O VALOR DA DISTÂNCIA MEDIDA
  result = String(distancia); //VARIÁVEL GLOBAL DO TIPO STRING RECEBE A DISTÂNCIA(CONVERTIDO DE INTEIRO PARA STRING)
  delay(5); //INTERVALO DE 500 MILISSEGUNDOS
}

void hcsr04_R() {
  digitalWrite(TRIGGER_SR, LOW); //SETA O PINO 6 COM UM PULSO BAIXO "LOW"
  delayMicroseconds(2); //INTERVALO DE 2 MICROSSEGUNDOS
  digitalWrite(TRIGGER_SR, HIGH); //SETA O PINO 6 COM PULSO ALTO "HIGH"
  delayMicroseconds(10); //INTERVALO DE 10 MICROSSEGUNDOS
  digitalWrite(TRIGGER_SR, LOW); //SETA O PINO 6 COM PULSO BAIXO "LOW" NOVAMENTE
  //FUNÇÃO RANGING, FAZ A CONVERSÃO DO TEMPO DE
  //RESPOSTA DO ECHO EM CENTIMETROS, E ARMAZENA
  //NA VARIAVEL "distancia"
  dist = (sen_distR.Ranging(CM)); //VARIÁVEL GLOBAL RECEBE O VALOR DA DISTÂNCIA MEDIDA
  result_2 = String(dist); //VARIÁVEL GLOBAL DO TIPO STRING RECEBE A DISTÂNCIA(CONVERTIDO DE INTEIRO PARA STRING)
  delay(5); //INTERVALO DE 500 MILISSEGUNDOS
}
//void rst()  {      /* Reseta os drivers */
//  digitalWrite(RST, LOW);
//  delay(10);
//  digitalWrite(RST, HIGH);
//  delay(10);
//  Serial.println("Reset OK");
//}
///////////////////////////////////////////////

// distancia > 80 -> Vou para 80v
//  80 < distancia > 100 -> Vou para 100v
// distanci > 100 --> Vou para 155

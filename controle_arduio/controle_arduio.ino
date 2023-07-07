#include <HCSR04.h>
#include <Wire.h>

#define ENA 13 /* Enable ativa os drivers do motor */
#define MS1 12 /* Seleciona a resolução do passo */
#define MS2 11 /* Seleciona a resolução do passo   */
#define MS3 10 /* Seleciona a resolução do passo */
#define RST 9 /* Desativa os drivers de saída   */
#define SLP 8 /* Inicia um descanso para algumas funções do drive */
#define DIR 7 /* Direção do passo  */
#define STP 6 /* Avanço do passo */
#define TRIGGER_SL   5
#define ECHO_SL      4
#define TRIGGER_SR   3
#define ECHO_SR      2
#define linha_L      1
#define linha_R      0
//#define max_passo 500 /* pulsos por resolução */
#define temp 1000

bool sen_linha_L = 0;
bool sen_linha_R = 0;

/*---- Variáveis de controle motor de passo---- */
int passo = 0;    /* passos */



// Inicializa o sensor usando os pinos TRIGGER and ECHO.
UltraSonicDistanceSensor sen_dist_L(TRIGGER_SL, ECHO_SL), sen_dist_R(TRIGGER_SR, ECHO_SR);

void mpu_begin();
void motoPassoBegin();
void getMpuData();
void pegar_distancias();

//Endereco I2C do MPU6050
const int MPU = 0x68;
//Variaveis para armazenar valores dos sensores
int AcX, AcY, AcZ, GyX, GyY, GyZ;

void setup() {
  //  motoPassoBegin();
  //Setamos os pinos dos seguidores de linha como entrada
  pinMode(linha_L, INPUT);
  pinMode(linha_R, INPUT);

  Serial.begin(9600);
  while (!Serial)
    ;
  Serial.print(F("Comunicacao serial estabelecida \nInicializando componentes"));
  ///MPU begin
  mpu_begin();
  // sensor de distância não precisa de inicialização

  Serial.println(F("..INIT.."));
}

void loop() {
  // put your main code here, to run repeatedly:

  //////////////////////////////////////////////////
  // MPU
  getMpuData();
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


  //  delay(500);


}
////Valores distâncias////////////////////////////////////////////////////////
void pegar_distancias() {
  //Cria variavel do tipo int
  int distancia = 0;
  //Variável recebe o valor da função da biblioteca
  distancia = sen_dist_L.measureDistanceCm();
  Serial.print(F("L_dist: "));
  Serial.println(distancia);
  //  distancia = sen_dist_R.measureDistanceCm();
  //  Serial.print(F("R_dist: "));
  //  Serial.println(distancia);
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
  digitalWrite(SLP, LOW);   /* Sleep desabilitado */
  digitalWrite(DIR, HIGH);  /* Sentido Horário habilitado */
  digitalWrite(RST, HIGH);  /* RST habilitado */
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

void rst()  {      /* Reseta os drivers */
  digitalWrite(RST, LOW);
  delay(10);
  digitalWrite(RST, HIGH);
  delay(10);
  Serial.println("Reset OK");
}

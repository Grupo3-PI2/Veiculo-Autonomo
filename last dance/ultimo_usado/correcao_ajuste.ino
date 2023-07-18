#include "Ultrasonic.h"  //INCLUSÃO DA BIBLIOTECA NECESSÁRIA PARA FUNCIONAMENTO DO CÓDIGO

const int echoPin = 13;   //PINO DIGITAL UTILIZADO PELO HC-SR04 ECHO(RECEBE)
const int trigPin = 12;   //PINO DIGITAL UTILIZADO PELO HC-SR04 TRIG(ENVIA)
const int echoPin2 = 16;  //PINO DIGITAL UTILIZADO PELO HC-SR04 ECHO(RECEBE)
const int trigPin2 = 17;  //PINO DIGITAL UTILIZADO PELO HC-SR04 TRIG(ENVIA)

Ultrasonic ultrasonic(trigPin, echoPin);   //INICIALIZANDO OS PINOS DO ARDUINO
Ultrasonic ultrasonic2(trigPin, echoPin);  //INICIALIZANDO OS PINOS DO ARDUINO

int distancia;   //VARIÁVEL DO TIPO INTEIRO
int distancia2;  //VARIÁVEL DO TIPO INTEIRO

// Iremos fazer uma classe para facilitar o uso da ponte H L298N na manipulação dos motores na função Setup e Loop.
class DCMotor {
  int spd = 255, pin1, pin2;

public:

  void Pinout(int in1, int in2) {  // Pinout é o método para a declaração dos pinos que vão controlar o objeto motor
    pin1 = in1;
    pin2 = in2;
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
  }
  void Speed(int in1) {  // Speed é o método que irá ser responsável por salvar a velocidade de atuação do motor
    spd = in1;
  }
  void Forward() {  // Forward é o método para fazer o motor girar para frente
    analogWrite(pin1, spd);
    digitalWrite(pin2, LOW);
  }
  void Backward() {  // Backward é o método para fazer o motor girar para trás
    digitalWrite(pin1, LOW);
    analogWrite(pin2, spd);
  }
  void Stop() {  // Stop é o metodo para fazer o motor ficar parado.
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
  }
};
DCMotor Motor2;  // Criação de dois objetos motores, já que usaremos dois motores, e eles já estão prontos para receber os comandos já configurados acima.


#define ENA 7 /* Enable ativa os drivers do motor */
#define MS1 6 /* Seleciona a resolução do passo */
#define MS2 4 /* Seleciona a resolução do passo   */
#define MS3 3 /* Seleciona a resolução do passo */
#define RST 8 /* Desativa os drivers de saída   */
#define SLP 9 /* Inicia um descanso para algumas funções do drive */
#define DIR 2 /* Direção do passo  */
#define STP 5 /* Avanço do passo */

/*---- DECLARAÇÃO SEGUIDOR DE LINHA---- */
#define linha_L 15
#define linha_R 14

/*---- Variáveis de controle ---- */
int PPR = 0;     /* pulsos por resolução */
int passo = 0;   /* passos */
int temp = 1000; /* tempo entre os passos */

bool sen_linha_L = 0;
bool sen_linha_R = 0;
bool end_curva = true;

void setup() {
  pinMode(echoPin, INPUT);    //DEFINE O PINO COMO ENTRADA (RECEBE)
  pinMode(trigPin, OUTPUT);   //DEFINE O PINO COMO SAIDA (ENVIA)
  pinMode(echoPin2, INPUT);   //DEFINE O PINO COMO ENTRADA (RECEBE)
  pinMode(trigPin2, OUTPUT);  //DEFINE O PINO COMO SAIDA (ENVIA)
  Motor2.Pinout(10, 11);
  pinMode(ENA, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  pinMode(STP, OUTPUT);
  pinMode(DIR, OUTPUT);
  digitalWrite(SLP, LOW);  /* Sleep desabilitado */
  digitalWrite(DIR, HIGH); /* Sentido Horário habilitado */
  digitalWrite(RST, HIGH); /* RST habilitado */
  //Serial.begin(9600);
  // Serial.println("Hello");
}

void loop() {
  hcsr04();  // FAZ A CHAMADA DO MÉTODO "hcsr04()"
  // if (distancia > 30 && distancia2 > 30) {
  //   move();
  //   //Serial.print('ANDANDO PARA FRENTE');

  // } else {
  //   stop();
  //   //Serial.print('PARADO');
  // }
  sixteenth();

  sen_linha_L = digitalRead(linha_L);
  sen_linha_R = digitalRead(linha_R);

  if (distancia > 30 && distancia2 > 30) {

  while (sen_linha_L && !sen_linha_R) {
    // Serial.print(F("estou virrando pra esuerda\n"));
    end_curva = false;
    movemin();

    if(passo <= PPR){
      volta_esquerda();
    }
    sen_linha_L = digitalRead(linha_L);
    sen_linha_R = digitalRead(linha_R);

    // Serial.println("Esquerda");
    //erial.println(passo);
    //delay(10);
    // Serial.println("Direita");
    // Serial.println(sen_linha_R);
  }
  if (!end_curva) {

    while(passo > 0){
      volta_direita();

      sen_linha_L = digitalRead(linha_L);
      sen_linha_R = digitalRead(linha_R);

      if(sen_linha_L){
        break;
      }
    }
    // ena2();
    // // delay(20);
    // ena();
    end_curva = true;
  }
  while (!sen_linha_L && sen_linha_R) {
    // Serial.print(F("estou virrando pra direita\n"));
    end_curva = false;
    movemin();

    if(passo >= -PPR){
      volta_direita();
    }
    sen_linha_L = digitalRead(linha_L);
    sen_linha_R = digitalRead(linha_R);
    //Serial.println(passo);
  }
  if (!end_curva) {

     while(passo < 0){
      volta_esquerda();

      sen_linha_L = digitalRead(linha_L);
      sen_linha_R = digitalRead(linha_R);

      if(sen_linha_R){
        break;
      }
    }

    // ena2();
    // delay(20);
    // ena();
    end_curva = true;
  }
  if (!sen_linha_L && !sen_linha_R) {
    movemax();
  }
  }
  else {
    stop();
  }
  //rst();                    /* Inicia o void rst */
}

void movemin() {
  Motor2.Speed(80);  // A velocidade do motor pode variar de 64 a 255, onde 255 é a velocidade máxima e 64 o mínimo.
  Motor2.Backward();
}

void movemax() {
  Motor2.Speed(110);  // A velocidade do motor pode variar de 64 a 255, onde 255 é a velocidade máxima e 64 o mínimo.
  Motor2.Backward();
}

void stop() {
  Motor2.Stop();
  delay(100);
}

//MÉTODO RESPONSÁVEL POR CALCULAR A DISTÂNCIA
void hcsr04() {
  digitalWrite(trigPin, LOW);   //SETA O PINO 6 COM UM PULSO BAIXO "LOW"
  delayMicroseconds(2);         //INTERVALO DE 2 MICROSSEGUNDOS
  digitalWrite(trigPin, HIGH);  //SETA O PINO 6 COM PULSO ALTO "HIGH"
  delayMicroseconds(10);        //INTERVALO DE 10 MICROSSEGUNDOS
  digitalWrite(trigPin, LOW);   //SETA O PINO 6 COM PULSO BAIXO "LOW" NOVAMENTE

  distancia = 120;  //VARIÁVEL GLOBAL RECEBE O VALOR DA DISTÂNCIA MEDIDA

  delay(10);  //INTERVALO DE 10 MILISSEGUNDOS

  digitalWrite(trigPin2, LOW);   //SETA O PINO 6 COM UM PULSO BAIXO "LOW"
  delayMicroseconds(2);          //INTERVALO DE 2 MICROSSEGUNDOS
  digitalWrite(trigPin2, HIGH);  //SETA O PINO 6 COM PULSO ALTO "HIGH"
  delayMicroseconds(10);         //INTERVALO DE 10 MICROSSEGUNDOS
  digitalWrite(trigPin2, LOW);   //SETA O PINO 6 COM PULSO BAIXO "LOW" NOVAMENTE

  distancia2 = 120;  //VARIÁVEL GLOBAL RECEBE O VALOR DA DISTÂNCIA MEDIDA
  delay(10);                               //INTERVALO DE 10 MILISSEGUNDOS
}

void sixteenth() { /* Em sixteenth - 3200 passos */
  PPR = 5000;       // limete para não quebrar o volante
  //Serial.println("Modo Sixteenth - STEP (1 / 16)");
  digitalWrite(MS1, HIGH);
  digitalWrite(MS2, HIGH);
  digitalWrite(MS3, HIGH);
}


void ena2() {
  digitalWrite(ENA, HIGH); /* Ativa o A4988 */
  delay(10);
}
void ena() {
  digitalWrite(ENA, LOW); /* Desativa o A4988 */
  delay(10);
}

void rst() { /* Reseta os drivers */
  digitalWrite(RST, LOW);
  delay(10);
  digitalWrite(RST, HIGH);
  delay(10);
  // Serial.println("Reset OK");
}
void Sleepoff() { /* Desativa o modo Sleep */
  //Serial.println("Sleep off");
  digitalWrite(SLP, LOW);
  delay(10);
}
void Sleepon() { /* Ativa o modo Sleep */
                 // Serial.println("Sleep on");
  digitalWrite(SLP, HIGH);
  delay(10);
}




void AH() { /* Sentido Anti-Horário */
  // passo = 0;
  digitalWrite(DIR, LOW);
}

void HR() { /* Sentido Horário */
  // passo = 0;
  digitalWrite(DIR, HIGH);
}

void volta_esquerda() {
  AH();              /* Inicia o sentido horário */
  if (PPR > passo) { /* Enquanto PPR for maior que passo */
                     /* Avança o passo */
    //Serial.println("MEDINDO O PASSO (ESUERDA) - "); Serial.println(passo);

    digitalWrite(STP, LOW);
    delayMicroseconds(temp); /* Tempo em Microseconds */
    digitalWrite(STP, HIGH);
    delayMicroseconds(temp);
    passo += 10; /* Aumenta em 1 o valor de passo */
  }
  else{
    delayMicroseconds(20000);
    delayMicroseconds(20000);
  }
  // if(passo > PPR){
  //   passo = 0; /* valor de passso muda pra 0 */
  // }
  // passo = 0; /* valor de passso muda pra 0 */
  // ena2();
  // delay(20);
  // ena();
}

void volta_direita() {
  HR();              /* Inicia o sentido anti horário */
  if (-PPR < passo) { /* Enquanto PPR for maior que passo */
                     /* Avança o passo */
    digitalWrite(STP, LOW);
    delayMicroseconds(temp); /* Tempo em Microseconds */
    digitalWrite(STP, HIGH);
    delayMicroseconds(temp);
    passo -= 10; /* Aumenta em 1 o valor de passo */
  }
  else{
    delayMicroseconds(20000);
    delayMicroseconds(20000);
  }
  // if (passo > PPR) {
  //   passo = 0; /* valor de passso muda pra 0 */
  // }
  // ena2();
  // delay(20);
  // ena();
}
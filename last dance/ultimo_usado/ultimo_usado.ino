#include "Ultrasonic.h"  //INCLUSÃO DA BIBLIOTECA NECESSÁRIA PARA FUNCIONAMENTO DO CÓDIGO
#include <MPU6050_tockn.h>
#include <Wire.h>
const int echoPin = 13;   //PINO DIGITAL UTILIZADO PELO HC-SR04 ECHO(RECEBE)
const int trigPin = 12;   //PINO DIGITAL UTILIZADO PELO HC-SR04 TRIG(ENVIA)
const int echoPin2 = 16;  //PINO DIGITAL UTILIZADO PELO HC-SR04 ECHO(RECEBE)
const int trigPin2 = 17;  //PINO DIGITAL UTILIZADO PELO HC-SR04 TRIG(ENVIA)

Ultrasonic ultrasonic(trigPin, echoPin);   //INICIALIZANDO OS PINOS DO ARDUINO
Ultrasonic ultrasonic2(trigPin, echoPin);  //INICIALIZANDO OS PINOS DO ARDUINO

int distancia;   //VARIÁVEL DO TIPO INTEIRO
int distancia2;  //VARIÁVEL DO TIPO INTEIRO

MPU6050 mpu6050(Wire); // declaraçãod e objeto Acelerômetro
String read_msg; // Leitura do monitor serial
bool stateDC = false;// estado do motor DC (andando ou parado)[talvez não seja preciso]

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
bool end_curval = true;
bool end_curvar = true;

unsigned long time1, time2, tdiff, tdiffl,tdiffr; // variáveis de tempo auxiliar(ajustar motor passo)


void setup() {
  pinMode(echoPin, INPUT);    //DEFINE O PINO COMO ENTRADA (RECEBE)
  pinMode(trigPin, OUTPUT);   //DEFINE O PINO COMO SAIDA (ENVIA)
  pinMode(echoPin2, INPUT);   //DEFINE O PINO COMO ENTRADA (RECEBE)
  pinMode(trigPin2, OUTPUT);  //DEFINE O PINO COMO SAIDA (ENVIA)
  Motor2.Pinout(11, 10);
  pinMode(ENA, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  pinMode(STP, OUTPUT);
  pinMode(DIR, OUTPUT);
  digitalWrite(SLP, LOW);  /* Sleep desabilitado */
  digitalWrite(DIR, HIGH); /* Sentido Horário habilitado */
  digitalWrite(RST, HIGH); /* RST habilitado */
  //  Serial.begin(9600);
  tdiffl = 0;
   tdiffr = 0;
  Serial.begin(9600);
  while (!Serial)
    ;
  Wire.begin();
  mpu6050.begin();// inicia o acelerômetro
  mpu6050.calcGyroOffsets(true);//calibra o acelerômetro
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

  getLineStatus(); // pega estado do seguidor de linha
  serialCom();// comunicação serial com a Rasp

  lineControl();// controle de motor de passo e velocidade de acordo com estado
  // da linha pra fazer curvas

  //rst();                    /* Inicia o void rst */

}
void serialCom() {
  if (Serial.available()) {
    read_msg = Serial.readString();
    if (read_msg[0] == 'G' || read_msg[0] == 'S')
      Serial.println(stateDC);
    else if (read_msg[0] == 'U') {
      mpu6050.update();
      printMpuData();
      Serial.println(stateDC);
    }
    else
      Serial.println(F("ERROR"));
    //    Serial.print(F(":"));
    //    Serial.println(stateDC);
  }
}
void lineControl() {
  if (distancia > 30 && distancia2 > 30) {
    time1 = millis();
    while (sen_linha_L && !sen_linha_R) {
      // Serial.print(F("estou virrando pra esuerda\n"));
      end_curvar = false;
      movemin();
      if (millis() < (time1 + 1100 - tdiffr)) {
        volta_esquerda();
        time2 = millis();
      }
      //      Serial.print(F("passo na volta pra esquerda:"));
      //      Serial.println(passo);
      getLineStatus();
      //      time2 = millis();
    }
    tdiff = time2 - time1;
    if (!end_curvar) {
      unsigned long int aux = millis();
      while (millis() < (aux + tdiff)) {
        volta_direita();
        getLineStatus();
        
        //        Serial.print(F("passo no aajuste:"));
        //        Serial.println(passo);
      }
      passo = 0; /* valor de passso muda pra 0 */
      ena2();
      delay(20);
      ena();
      end_curvar = true;
    }
    while (!sen_linha_L && sen_linha_R) {
      // Serial.print(F("estou virrando pra direita\n"));
      end_curval = false;
      movemin();
      if (millis() < (time1 + 1100 - tdiffl)) {
        volta_direita();
        time2 = millis();
      }
      getLineStatus();
      //      time2 = millis();
    }
    tdiff = time2 - time1;
    if (!end_curval) {
      unsigned long int aux = millis();
      while (millis() < (aux + tdiff)) {
        volta_esquerda();
        //        Serial.print(F("passo no aajuste:"));
        //        Serial.println(passo);
      }
      passo = 0; /* valor de passso muda pra 0 */
      ena2();
      delay(20);
      ena();
      end_curval = true;
    }
    if (!sen_linha_L && !sen_linha_R) {
      movemax();
    }
  }
  else {
    stop();
  }
}
void getLineStatus() {
  sen_linha_L = digitalRead(linha_L);
  sen_linha_R = digitalRead(linha_R);
}
void movemin() {
  Motor2.Speed(108);  // A velocidade do motor pode variar de 64 a 255, onde 255 é a velocidade máxima e 64 o mínimo.
  Motor2.Backward();
  stateDC = true;
}

void movemax() {
  Motor2.Speed(120);  // A velocidade do motor pode variar de 64 a 255, onde 255 é a velocidade máxima e 64 o mínimo.
  Motor2.Backward();
  stateDC = true;
}

void stop() {
  Motor2.Stop();
  stateDC = false;
  delay(100);
}

//MÉTODO RESPONSÁVEL POR CALCULAR A DISTÂNCIA
void hcsr04() {
  digitalWrite(trigPin, LOW);   //SETA O PINO 6 COM UM PULSO BAIXO "LOW"
  delayMicroseconds(2);         //INTERVALO DE 2 MICROSSEGUNDOS
  digitalWrite(trigPin, HIGH);  //SETA O PINO 6 COM PULSO ALTO "HIGH"
  delayMicroseconds(10);        //INTERVALO DE 10 MICROSSEGUNDOS
  digitalWrite(trigPin, LOW);   //SETA O PINO 6 COM PULSO BAIXO "LOW" NOVAMENTE

  distancia = (ultrasonic.Ranging(CM));  //VARIÁVEL GLOBAL RECEBE O VALOR DA DISTÂNCIA MEDIDA

  delay(10);  //INTERVALO DE 10 MILISSEGUNDOS

  digitalWrite(trigPin2, LOW);   //SETA O PINO 6 COM UM PULSO BAIXO "LOW"
  delayMicroseconds(2);          //INTERVALO DE 2 MICROSSEGUNDOS
  digitalWrite(trigPin2, HIGH);  //SETA O PINO 6 COM PULSO ALTO "HIGH"
  delayMicroseconds(10);         //INTERVALO DE 10 MICROSSEGUNDOS
  digitalWrite(trigPin2, LOW);   //SETA O PINO 6 COM PULSO BAIXO "LOW" NOVAMENTE

  distancia2 = (ultrasonic2.Ranging(CM));  //VARIÁVEL GLOBAL RECEBE O VALOR DA DISTÂNCIA MEDIDA
  delay(10);                               //INTERVALO DE 10 MILISSEGUNDOS
}

void sixteenth() { /* Em sixteenth - 3200 passos */
  PPR = 400;       // limete para não quebrar o volante
  //Serial.println("Modo Sixteenth - STEP (1 / 16)");
  digitalWrite(MS1, HIGH);
  digitalWrite(MS2, HIGH);
  digitalWrite(MS3, HIGH);
}

void direita() {
  // max_passo = 500; // limete para não quebrar o volante
  // passo = 0;
  // Serial.print(F("PASSO DIREITA: ")); Serial.println(passo);

  passo += 5;
  if (passo >= 500) {
    passo = 500;
  } else {
    // Serial.println("Modo Sixteenth - STEP (1 / 16)");
    //digitalWrite(MS1, HIGH);
    //digitalWrite(MS2, HIGH);
    // digitalWrite(MS3, HIGH);
    //Serial.println("Sentido - Horario");
    digitalWrite(DIR, HIGH);
    digitalWrite(STP, LOW);
    delayMicroseconds(temp); /* Tempo em Microseconds */
    digitalWrite(STP, HIGH);
    delayMicroseconds(temp);
    ena2();
    delay(50);
    ena();
    //delay(20)
  }
}

void esquerda() {
  // max_passo = 500; // limete para não quebrar o volante
  // passo = 0;
  //Serial.print(F("PASSO ESUERDA: ")); Serial.println(passo);

  passo -= 5;
  if (passo <= -500) {
    passo = -500;
  } else {
    //Serial.println("Modo Sixteenth - STEP (1 / 16)");
    // digitalWrite(MS1, HIGH);
    //  digitalWrite(MS2, HIGH);
    // digitalWrite(MS3, HIGH);
    // Serial.println("Sentido - Horario");
    digitalWrite(DIR, LOW);
    digitalWrite(STP, LOW);
    delayMicroseconds(temp); /* Tempo em Microseconds */
    digitalWrite(STP, HIGH);
    delayMicroseconds(temp);
    ena2();
    delay(50);
    ena();
    //delay(20);
  }
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
  passo = 0;
  digitalWrite(DIR, LOW);
}

void HR() { /* Sentido Horário */
  passo = 0;
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
  if (PPR > passo) { /* Enquanto PPR for maior que passo */
    /* Avança o passo */
    digitalWrite(STP, LOW);
    delayMicroseconds(temp); /* Tempo em Microseconds */
    digitalWrite(STP, HIGH);
    delayMicroseconds(temp);
    passo -= 10; /* Aumenta em 1 o valor de passo */
  }
  // if (passo > PPR) {
  //   passo = 0; /* valor de passso muda pra 0 */
  // }
  // ena2();
  // delay(20);
  // ena();
}

void printMpuData() {
  Serial.print(mpu6050.getAccX());
  Serial.print(F(":"));
  Serial.print(mpu6050.getAccY());
  Serial.print(F(":"));
  Serial.print(mpu6050.getAccZ());
  Serial.print(F(":"));
}
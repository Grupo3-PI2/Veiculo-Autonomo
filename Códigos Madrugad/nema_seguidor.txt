/*---- DECLARAÇÃO NEMA 17 ---- */

#define ENA 7 /* Enable ativa os drivers do motor */
#define MS1 6 /* Seleciona a resolução do passo */
#define MS2 4 /* Seleciona a resolução do passo   */
#define MS3 3 /* Seleciona a resolução do passo */
#define RST 8 /* Desativa os drivers de saída   */
#define SLP 9 /* Inicia um descanso para algumas funções do drive */
#define DIR 2 /* Direção do passo  */
#define STP 5 /* Avanço do passo */

/*---- DECLARAÇÃO SEGUIDOR DE LINHA---- */
#define linha_L      11
#define linha_R      12

/*---- Variáveis de controle ---- */
int PPR = 0;      /* pulsos por resolução */
int passo = 0;    /* passos */
int temp = 1000;  /* tempo entre os passos */

bool sen_linha_L = 0;  
bool sen_linha_R = 0;

void setup()
{
  pinMode(ENA, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  pinMode(STP, OUTPUT);
  pinMode(DIR, OUTPUT);
  digitalWrite(SLP, LOW);   /* Sleep desabilitado */
  digitalWrite(DIR, HIGH);  /* Sentido Horário habilitado */
  digitalWrite(RST, HIGH);  /* RST habilitado */
//  Serial.begin(9600);
}

void loop()
  {
  sixteenth();

  sen_linha_L = digitalRead(linha_L);
  sen_linha_R = digitalRead(linha_R);
  //Serial.print(F("Linha esquerda: ")); Serial.println(sen_linha_L);
  //Serial.print(F("Linha direita: ")); Serial.println(sen_linha_R);
 if (sen_linha_L) {
    volta_esquerda();
  }
  else if (sen_linha_R) {
    volta_direita();
  }
  else {
   // Serial.println(F("Estou seguindo a linha"));
  }
    rst();                    /* Inicia o void rst */
  
}

void sixteenth()
{                      /* Em sixteenth - 3200 passos */
  PPR = 200; // limete para não quebrar o volante
  //Serial.println("Modo Sixteenth - STEP (1 / 16)");
  digitalWrite(MS1, HIGH);
  digitalWrite(MS2, HIGH);
  digitalWrite(MS3, HIGH);
}

void direita() {
  // max_passo = 500; // limete para não quebrar o volante
  // passo = 0;
 // Serial.print(F("PASSO DIREITA: ")); Serial.println(passo);

  passo+=5;
  if (passo >= 500) {
    passo = 500;
  }
  else {
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

  passo-=5;
  if (passo <= -500) {
    passo = -500;
  }
  else {
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
  digitalWrite(ENA, HIGH);  /* Ativa o A4988 */
  delay(10);
}
void ena() {
  digitalWrite(ENA, LOW);   /* Desativa o A4988 */
  delay(10);
}

void rst()  {      /* Reseta os drivers */
  digitalWrite(RST, LOW);
  delay(10);
  digitalWrite(RST, HIGH);
  delay(10);
 // Serial.println("Reset OK");
}
void Sleepoff()  {   /* Desativa o modo Sleep */
  //Serial.println("Sleep off");
  digitalWrite(SLP, LOW);
  delay(10);
}
void Sleepon()  {   /* Ativa o modo Sleep */
 // Serial.println("Sleep on");
  digitalWrite(SLP, HIGH);
  delay(10);
}




void AH()  {               /* Sentido Anti-Horário */
  passo = 0;
  digitalWrite(DIR, LOW);
}

void HR() {                /* Sentido Horário */
  passo = 0;     
  digitalWrite(DIR, HIGH);
}

void volta_esquerda() {
  AH();  /* Inicia o sentido horário */
  while (PPR > passo)
  {      /* Enquanto PPR for maior que passo */
     /* Avança o passo */
    //Serial.println("MEDINDO O PASSO (ESUERDA) - "); Serial.println(passo);

    digitalWrite(STP, LOW);
    delayMicroseconds(temp); /* Tempo em Microseconds */
    digitalWrite(STP, HIGH);
    delayMicroseconds(temp);
    passo++; /* Aumenta em 1 o valor de passo */
  }
  passo = 0; /* valor de passso muda pra 0 */
  ena2();    
  delay(20);
  ena();
}

void volta_direita() {
  HR();  /* Inicia o sentido anti horário */
  while (PPR > passo)
  {      /* Enquanto PPR for maior que passo */
     /* Avança o passo */

    digitalWrite(STP, LOW);
    delayMicroseconds(temp); /* Tempo em Microseconds */
    digitalWrite(STP, HIGH);
    delayMicroseconds(temp);
    passo++; /* Aumenta em 1 o valor de passo */
  }
  passo = 0; /* valor de passso muda pra 0 */
  ena2();    
  delay(20);
  ena();
}
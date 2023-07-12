#define ENA 7 /* Enable ativa os drivers do motor */
#define MS1 6 /* Seleciona a resolução do passo */
#define MS2 4 /* Seleciona a resolução do passo   */
#define MS3 3 /* Seleciona a resolução do passo */
#define RST 8 /* Desativa os drivers de saída   */
#define SLP 9 /* Inicia um descanso para algumas funções do drive */
#define DIR 2 /* Direção do passo  */
#define STP 5 /* Avanço do passo */

/*---- Variáveis de controle ---- */
int PPR = 0;      /* pulsos por resolução */
int passo = 0;    /* passos */
int temp = 1000;  /* tempo enktre os passos */
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
  Serial.begin(9600);
}

void loop()
  {
    sixteenth();             /* Inicia o void sixteenth */
    volta1();
	  volta2();
    rst();                    /* Inicia o void rst */
  
}

void sixteenth()
{                      /* Em sixteenth - 3200 passos */
  PPR = 500; // limete para não quebrar o volante
  Serial.println("Modo Sixteenth - STEP (1 / 16)");
  digitalWrite(MS1, HIGH);
  digitalWrite(MS2, HIGH);
  digitalWrite(MS3, HIGH);
}
void volta1() {
  HR();  /* Inicia o sentido horário */
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
  delay(1000);
  ena();
  AH();      /* Inicia o Sentido Anti-horário */
  while (PPR > passo) 
  {      /* Enquanto PPR for maior que passo */
         /* Avança o passo */
    digitalWrite(STP, LOW);
    delayMicroseconds(temp);
    digitalWrite(STP, HIGH);
    delayMicroseconds(temp);
    passo++;
  }
  passo = 0;
  ena2();
  delay(1000);
  ena();
}

void volta2() {
  AH();  /* Inicia o sentido horário */
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
  delay(1000);
  ena();
  HR();      /* Inicia o Sentido Anti-horário */
  while (PPR > passo) 
  {      /* Enquanto PPR for maior que passo */
         /* Avança o passo */
    digitalWrite(STP, LOW);
    delayMicroseconds(temp);
    digitalWrite(STP, HIGH);
    delayMicroseconds(temp);
    passo++;
  }
  passo = 0;
  ena2();
  delay(1000);
  ena();
}

void ena2() {
  digitalWrite(ENA, HIGH);  /* Ativa o A4988 */
  delay(10);
}
void ena() {
  digitalWrite(ENA, LOW);   /* Desativa o A4988 */
  delay(10);
}
void HR() {                /* Sentido Horário */
  passo = 0;     
  Serial.println("Sentido - Horario");
  digitalWrite(DIR, HIGH);
}
void AH()  {               /* Sentido Anti-Horário */
  passo = 0;
  Serial.println("Sentido - Anti-horario");
  digitalWrite(DIR, LOW);
}
void rst()  {      /* Reseta os drivers */
  digitalWrite(RST, LOW);
  delay(10);
  digitalWrite(RST, HIGH);
  delay(10);
  Serial.println("Reset OK");
}
void Sleepoff()  {   /* Desativa o modo Sleep */
  Serial.println("Sleep off");
  digitalWrite(SLP, LOW);
  delay(10);
}
void Sleepon()  {   /* Ativa o modo Sleep */
  Serial.println("Sleep on");
  digitalWrite(SLP, HIGH);
  delay(10);
}

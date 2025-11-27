// Motores
#define MOTOR_DIR_FRENTE 5  
#define MOTOR_DIR_TRAS   6  
#define MOTOR_ESQ_TRAS   9
#define MOTOR_ESQ_FRENTE 10

// Sensores
#define PINO_DIR_SENSOR 2
#define PINO_ESQ_SENSOR 3
#define PINO_TRIG 12
#define PINO_ECHO 11
#define PINO_BOTAO 4

// Configurações
#define PRETO HIGH
#define BRANCO LOW
#define DISTANCIA_ATAQUE 30 

// Velocidades
#define VELOCIDADE_ATAQUE 255 
#define VELOCIDADE_GIRO   110 
#define VELOCIDADE_RE     220  

bool lutaIniciada = false;
unsigned long inicioBusca = 0;

void setup() {
  pinMode(MOTOR_DIR_FRENTE, OUTPUT);
  pinMode(MOTOR_DIR_TRAS, OUTPUT);
  pinMode(MOTOR_ESQ_TRAS, OUTPUT);
  pinMode(MOTOR_ESQ_FRENTE, OUTPUT);

  pinMode(PINO_ESQ_SENSOR, INPUT);
  pinMode(PINO_DIR_SENSOR, INPUT);
  pinMode(PINO_TRIG, OUTPUT);
  pinMode(PINO_ECHO, INPUT);
  pinMode(PINO_BOTAO, INPUT_PULLUP);

  Serial.begin(9600);
  pararMotores(); 
}

void loop() {
  if (lutaIniciada == false) {
    if (digitalRead(PINO_BOTAO) == LOW) {
      pararMotores();
      delay(5000);
      lutaIniciada = true; 
      inicioBusca = millis(); 
    } else {
      pararMotores(); 
      return; 
    }
  }

  int linhaEsq = digitalRead(PINO_ESQ_SENSOR);
  int linhaDir = digitalRead(PINO_DIR_SENSOR);

  if (linhaEsq == BRANCO || linhaDir == BRANCO) {
    pararMotores(); 
    irParaTras(VELOCIDADE_RE);   
    delay(600);     
    girarDireita(VELOCIDADE_RE); 
    delay(300);
    inicioBusca = 0;
    return;         
  }

  int distancia = lerDistancia();

  if (distancia > 0 && distancia < DISTANCIA_ATAQUE) {
    irParaFrente(VELOCIDADE_ATAQUE);
    inicioBusca = 0; 
  } else {
    executarEstrategiaBusca();
  }
}

void executarEstrategiaBusca() {
  if (inicioBusca == 0) inicioBusca = millis();
  unsigned long tempo = millis() - inicioBusca;

  if (tempo < 1000) {
    girarEsquerda(VELOCIDADE_GIRO);
  } 
  
  else if (tempo < 2000) { 
    pararMotores();
  }
  
  else if (tempo < 3500) { 
    girarDireita(VELOCIDADE_GIRO);
  }
  
  else if (tempo < 5000) { 
    girarEsquerda(VELOCIDADE_GIRO);
  }
  
  else if (tempo < 6500) { 
    girarEsquerda(VELOCIDADE_GIRO);
  }
  
  else if (tempo < 8000) { 
    girarDireita(VELOCIDADE_GIRO);
  }
  
  else {
    inicioBusca = millis();
  }
}

void irParaFrente(int vel) {
  analogWrite(MOTOR_DIR_FRENTE, vel); digitalWrite(MOTOR_DIR_TRAS, LOW);
  analogWrite(MOTOR_ESQ_FRENTE, vel); digitalWrite(MOTOR_ESQ_TRAS, LOW);
}

void irParaTras(int vel) {
  digitalWrite(MOTOR_DIR_FRENTE, LOW); analogWrite(MOTOR_DIR_TRAS, vel);
  digitalWrite(MOTOR_ESQ_FRENTE, LOW); analogWrite(MOTOR_ESQ_TRAS, vel);
}

void girarDireita(int vel) {
  analogWrite(MOTOR_ESQ_FRENTE, vel); digitalWrite(MOTOR_ESQ_TRAS, LOW);
  digitalWrite(MOTOR_DIR_FRENTE, LOW); analogWrite(MOTOR_DIR_TRAS, vel);
}

void girarEsquerda(int vel) {
  digitalWrite(MOTOR_ESQ_FRENTE, LOW); analogWrite(MOTOR_ESQ_TRAS, vel);
  analogWrite(MOTOR_DIR_FRENTE, vel); digitalWrite(MOTOR_DIR_TRAS, LOW);
}

void pararMotores() {
  digitalWrite(MOTOR_DIR_FRENTE, LOW); digitalWrite(MOTOR_DIR_TRAS, LOW);
  digitalWrite(MOTOR_ESQ_TRAS, LOW); digitalWrite(MOTOR_ESQ_FRENTE, LOW);
}

int lerDistancia() {
  digitalWrite(PINO_TRIG, LOW); delayMicroseconds(2);
  digitalWrite(PINO_TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(PINO_TRIG, LOW);
  long duracao = pulseIn(PINO_ECHO, HIGH);
  return duracao * 0.034 / 2;
}
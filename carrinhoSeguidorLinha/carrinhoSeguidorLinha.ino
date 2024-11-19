#include <Ultrasonic.h>
#include <Wire.h>

#define motorA_VM 5
#define motorB_VM 6
#define motorA_DM 7
#define motorB_DM 8
#define pinS0 9
#define pinS1 10
#define pinS2 13
#define pinS3 12
#define pinOut 11

//int motorA_VM = 4;  //Controle de velocidade do motor A
//int motorB_VM = 5;  //Controle de velocidade do motor B
//int motorA_DM = 6;  //Controle de direção do motor A
//int motorB_DM = 7;  //Controle de direção do motor B

int sensorDireito = 8; 
int sensorCentro = 7;
int sensorEsquerdo = 6;

const int echoPin= 3;
const int trigPin= 2;
const int pinoLedVerm= 5;
Ultrasonic ultrasonic(2, 3);


//Leitura de sensores
int leituraDireito;
int leituraCentro;
int leituraEsquerdo;

//Velocidade Motores
int veloc0 = 0;  //Velocidade para parar os motores
int velocA1 = 100;
int velocB1 = 100;
int velocA2 = 40;
int velocB2 = 40;

// variaveis de controle
long microsec = 0;
float distanciaCM = 0;

unsigned int valorVermelho = 0;
unsigned int valorVerde = 0;
unsigned int valorAzul = 0;
unsigned int valorBranco = 0;

void detectaCor();

void setup() {
  Serial.begin(9600); //Inicializa o monitor serial


  delay(5000); // delay de 5 segundos para inicializar o programa

//Declarando os motores como saída
  pinMode(motorA_VM, OUTPUT);
  pinMode(motorA_DM, OUTPUT);
  pinMode(motorB_VM, OUTPUT);
  pinMode(motorB_DM, OUTPUT);

  
//Declarando os sensores como saída
  pinMode(sensorDireito, INPUT);
  pinMode(sensorCentro, INPUT);
  pinMode(sensorEsquerdo, INPUT);
  //Declarando Led
  pinMode(pinoLedVerm, OUTPUT);

//Declarando sensor RGB
  pinMode(pinS0, OUTPUT);
  pinMode(pinS1, OUTPUT);
  pinMode(pinS2, OUTPUT);
  pinMode(pinS3, OUTPUT);
  pinMode(pinOut, INPUT);

  Serial.begin(9600);
  digitalWrite(pinS0, HIGH);
  digitalWrite(pinS1, LOW);



}
void loop() {
  
  leitura();
  seguidor();
  ledDistancia();

  delay(2000);

 
  //Serial.println(sensorDireito);//printar a leitura do sensor no monitor serial
  
  //Lendo o valor do sensor
  microsec = ultrasonic.timing();

  //Convertendo a distância em CM
  distanciaCM = ultrasonic.convert(microsec, Ultrasonic::CM);


  Serial.print(distanciaCM);// mostrar a distancia na porta serial
  Serial.println(" cm");// colocar unidade de medida
  
 
}
//leitura do sensor infravermelho
void leitura() {
  leituraDireito = digitalRead(sensorDireito);
  leituraCentro = digitalRead(sensorCentro);
  leituraEsquerdo = digitalRead(sensorEsquerdo);

}
//Condicionais para a movimentação do carrinho
void seguidor() {

  if ((leituraDireito == 1) && (leituraCentro == 0) && (leituraEsquerdo == 1)) {
    frente();
    Serial.println(sensorCentro);

  }
  if ((leituraDireito == 0) && (leituraCentro == 1) && (leituraEsquerdo == 1)) {
    direita();
    Serial.println(sensorDireito);
  }
  if ((leituraDireito == 1) && (leituraCentro == 1) && (leituraEsquerdo == 0)) {
    esquerda();
    Serial.println(sensorEsquerdo);
  }
  }
  //Funções para a movimentação do carrinho
  void frente() {
    digitalWrite(motorA_DM, HIGH);
    digitalWrite(motorB_DM, HIGH);
    analogWrite(motorA_VM, velocA1);
    analogWrite(motorB_VM, velocB1);
  }
  void direita() {
    digitalWrite(motorA_DM, HIGH);
    digitalWrite(motorB_DM, LOW);
    analogWrite(motorA_VM, velocA2);
    analogWrite(motorB_VM, veloc0);
  }
  void esquerda() {
    digitalWrite(motorA_DM, LOW);
    digitalWrite(motorB_DM, HIGH);
    analogWrite(motorA_VM, veloc0);
    analogWrite(motorB_VM, velocB2);
  }
  void parar() {
    digitalWrite(motorA_DM, LOW);
    digitalWrite(motorB_DM, LOW);
    analogWrite(motorA_VM, veloc0);
    analogWrite(motorB_VM, veloc0);
  }
  void ledDistancia() {
    digitalWrite(led,LOW);
  //Condicional para acionamento do LED
  if (distanciaCM <= 6) { // se a distancia for menor ou igual a 8 cm o carrinho para
    digitalWrite(led, HIGH);
    parar();
    
  }
void sensorRGB() {
     //Detecta a cor
  detectaCor();

  //Mostra valores no serial monitor
  Serial.print("Vermelho :");
  Serial.print(valorVermelho);

  Serial.print(" Verde : ");
  Serial.print(valorVerde);

  Serial.print(" Azul : ");
  Serial.print(valorAzul);

  Serial.print(" Branco : ");
  Serial.print(valorBranco);
  Serial.println();

  //Verifica se a cor vermelha foi detectada
  if ((valorVermelho < valorAzul) &&
      (valorVermelho < valorVerde) &&
      (valorBranco < 100)) {
    Serial.println("Vermelho");
    digitalWrite(pinoLedVerm, HIGH); //Acende o led vermelho

  } else if ((valorAzul < valorVermelho) &&  //Verifica se a cor azul foi detectada
             (valorAzul < valorVerde) &&
             (valorBranco < 100)) {
    Serial.println("Azul");
    digitalWrite(pinoLedVerm, LOW);

  } else if ((valorVerde < valorVermelho) &&  //Verifica se a cor verde foi detectada
             (valorVerde < valorAzul) &&
             (valorBranco < 100)) {
    Serial.println("Verde");
    digitalWrite(pinoLedVerm, LOW);
  }
  Serial.println();

  //Delay para apagar os leds e reiniciar o processo
  delay(50);
  digitalWrite(pinoLedVerm, LOW);
}
void detectaCor() {
  //Vermelho
  digitalWrite(pinS2, LOW);
  digitalWrite(pinS3, LOW);
  valorVermelho = pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH);
  
  //Sem filtro
  digitalWrite(pinS2, HIGH);
  valorBranco = pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH);

  //Azul
  digitalWrite(pinS2, LOW);
  digitalWrite(pinS3, HIGH);
  valorAzul = pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH);

  //Verde
  digitalWrite(pinS2, HIGH);
  valorVerde = pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH);
}
}

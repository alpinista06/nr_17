///////////
//Defines//
///////////
#define limiar_sensores 800
#define velocidade 40

//#define teste_sensores
#define main
#define Mostrar_valores

/////////////////
//Dados globais//
////////////////
int sensor_1 = A0;
int sensor_2 = A1;
int sensor_3 = A2;
uint8_t array_sensores = 0;
int A = 5;
int Kp = 20;
int Ki = 30;
int resposta_p;
int resposta_i;
int Kpi = 50;
int Resposta_PI;
int respPI;
int Vplus, Vless;
int IN0A, IN1A, IN0B, IN1B;
int ENA = 9; // motor esquerdo
int ENB = 10; //motor direito
int erro_array;
int amostra_atual;
int amostra_anterior = 0;
int erro_atual;
int erro_anterior = 0;
//declaracao dos motores
int motor1_a = 4;
int motor1_b = 6;
int motor2_a = 5;
int motor2_b = 7;

int leitura1;
int leitura2;
int leitura3;



void frente() {

  analogWrite(ENA, abs(velocidade) );
  analogWrite(ENB, abs(velocidade) );

  digitalWrite(motor1_a , HIGH);
  digitalWrite(motor1_b , LOW);
  digitalWrite(motor2_a , LOW);
  digitalWrite(motor2_b , HIGH);
}

int giro_a_esquerda(int respPI)
{
  Vplus = velocidade + respPI;
  Vless = velocidade - respPI;
  analogWrite(ENA, abs(Vplus) );
  analogWrite(ENB, abs(Vless) );

  digitalWrite(motor1_a , HIGH);
  digitalWrite(motor1_b , LOW);
  digitalWrite(motor2_a , LOW);
  digitalWrite(motor2_b , LOW);
  // delay(2000);
}

int giro_a_direita(int respPI)
{
  Vplus = velocidade + respPI;
  Vless = velocidade - respPI;
  analogWrite(ENA, abs(Vless) );
  analogWrite(ENB, abs(Vplus) );

  digitalWrite(motor1_a , LOW);
  digitalWrite(motor1_b , LOW);
  digitalWrite(motor2_a , LOW);
  digitalWrite(motor2_b , HIGH);
  // delay(2000);
}

int motores_parados()
{
  digitalWrite(motor1_a , HIGH);
  digitalWrite(motor1_b , HIGH);
  digitalWrite(motor2_a , HIGH);
  digitalWrite(motor2_b , HIGH);
  //  delay(1000);
}

void setup() {

  Serial.begin(9600);

}
void loop() {
  leitura1 = analogRead(sensor_1);
  leitura2 = analogRead(sensor_2);
  leitura3 = analogRead(sensor_3);


  //teste sensores//
#ifdef teste_sensores
  Serial.print(leitura1);
  Serial.print("\t");
  Serial.print(leitura2);
  Serial.print("\t");
  Serial.println(leitura3);
  Serial.print("\t");
  Serial.println();
#endif

#ifdef main
  //Bloco de montagem das estrutura do array_sensores//
  if (leitura1 > limiar_sensores && leitura2 > limiar_sensores && leitura3 > limiar_sensores)
  {
    array_sensores = 0b111;
  } else if (leitura1 < limiar_sensores && leitura2 > limiar_sensores && leitura3 < limiar_sensores)
  {
    array_sensores = 0b010;
  } else if (leitura1 < limiar_sensores && leitura2 < limiar_sensores && leitura3 > limiar_sensores)
  {
    array_sensores = 0b001;
  } else if (leitura1 < limiar_sensores && leitura2 > limiar_sensores && leitura3 < limiar_sensores)
  {
    array_sensores = 0b010;
  } else if (leitura1 < limiar_sensores && leitura2 > limiar_sensores && leitura3 > limiar_sensores)
  {
    array_sensores = 0b011;
  } else if (leitura1 > limiar_sensores && leitura2 < limiar_sensores && leitura3 < limiar_sensores)
  {
    array_sensores = 0b100;
  } else if (leitura1 > limiar_sensores && leitura2 > limiar_sensores && leitura3 < limiar_sensores)
  {
    array_sensores = 0b110;
  }

  //Calculo dos erros//
  if (array_sensores == 0b100) {
    erro_array = 2;

  } else if (array_sensores == 0b010) {
    erro_array = 0;

  } else if (array_sensores == 0b001) {
    erro_array = -2;

  } else if (array_sensores == 0b110) {
    erro_array = 1;

  } else if (array_sensores == 0b011) {
    erro_array = -1;

  } else if (array_sensores == 0b111) {
    erro_array = 0;

  } else if (array_sensores == 0b000) {
    erro_array = 0;

  }
#ifdef Mostrar_valores
  Serial.print("Valor do erro:");
  Serial.print("  ");
  Serial.print(erro_array);
  Serial.print("  ");
#endif
  //Compensador PI//
  erro_atual = erro_array;
  resposta_p = erro_atual * Kp;
  resposta_i = resposta_i + erro_atual * Ki * A;
  Resposta_PI = resposta_p + resposta_i;
#ifdef Mostrar_valores
  Serial.print("\t");
  Serial.print("Resposta PI:");
  Serial.print("  ");
  Serial.print(Resposta_PI);
  Serial.print("  ");
#endif
  //main//
  if (erro_array == 0) {
    frente();
  }
  if (erro_array == 1) {
    giro_a_esquerda(Resposta_PI);
  }
  if (erro_array == 2) {
    giro_a_esquerda(Resposta_PI);
  }
  if (erro_array == -1) {
    giro_a_direita(Resposta_PI);
  }
  if (erro_array == -2) {
    giro_a_direita(Resposta_PI);
  }
#ifdef Mostrar_valores
  Serial.print("\t");
  Serial.print("V+:");
  Serial.print("  ");
  Serial.print(Vplus);
  Serial.print("  ");
  Serial.print("\t");
  Serial.print("V-:");
  Serial.print("  ");
  Serial.println(Vless);
#endif

#endif

}

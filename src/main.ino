///////////
//Defines//
///////////
#define LIMIAR_SENSORES 800

//#define TESTE_SENSORES
#define PRINCIPAL
//#define MOSTRAR_VALORES

/////////////////
//Dados globais//
////////////////
//Sensores//
int sensor_1 = A0;
int sensor_2 = A1;
int sensor_3 = A2;
uint8_t array_sensores = 0;
int leitura1;
int leitura2;
int leitura3;
//Compensador PID//
int A = 0.5;
int Kp = 20;
int Ki = 35;
int Kd = 15;
int resposta_p;
int resposta_i;
int resposta_d;
int Resposta_PID;
int erro_array;
int amostra_atual;
int amostra_anterior;
int erro_atual;
int erro_anterior;
//Velocidade//
int velocidade = 0;
int respPI;
int Vplus, Vless;
int IN0A, IN1A, IN0B, IN1B;
int ENA = 9; // motor esquerdo
int ENB = 10; //motor direito
//declaracao dos motores//
int motor1_a = 4;
int motor1_b = 6;
int motor2_a = 5;
int motor2_b = 7;

//Bloco das funções//

void frente() {

  velocidade = 80;
  analogWrite(ENA, abs(velocidade) );
  analogWrite(ENB, abs(velocidade) );

  digitalWrite(motor1_a , HIGH);
  digitalWrite(motor1_b , LOW);
  digitalWrite(motor2_a , LOW);
  digitalWrite(motor2_b , HIGH);
}

//curva compensada à esquerda//
void curva_a_esquerda(int respPI) {

  velocidade = 80;
  Vplus = velocidade + respPI;
  Vless = velocidade - respPI;
  analogWrite(ENA, Vplus );
  analogWrite(ENB, Vless );

  digitalWrite(motor1_a , HIGH);
  digitalWrite(motor1_b , LOW);
  digitalWrite(motor2_a , LOW);
  digitalWrite(motor2_b , HIGH);
}
//curva compensada à direita//
void curva_a_direita(int respPI) {

  velocidade = 80;
  Vplus = velocidade + respPI;
  Vless = velocidade - respPI;
  analogWrite(ENA, Vless );
  analogWrite(ENB, Vplus );

  digitalWrite(motor1_a , HIGH);
  digitalWrite(motor1_b , LOW);
  digitalWrite(motor2_a , LOW);
  digitalWrite(motor2_b , HIGH);
}

//Giro a esquerda desligando um dos motores//
void giro_a_esquerda(int respPI)
{
  velocidade = 135;
  Vplus = velocidade + respPI;
  Vless = velocidade - respPI;
  analogWrite(ENA, Vplus );
  analogWrite(ENB, Vless );

  digitalWrite(motor1_a , HIGH);
  digitalWrite(motor1_b , LOW);
  digitalWrite(motor2_a , LOW);
  digitalWrite(motor2_b , LOW);
  // delay(2000);
}
//Giro a direita desligando um dos motores//
void giro_a_direita(int respPI)
{
  velocidade = 135;
  Vplus = velocidade + respPI;
  Vless = velocidade - respPI;
  analogWrite(ENA, Vless );
  analogWrite(ENB, Vplus );

  digitalWrite(motor1_a , LOW);
  digitalWrite(motor1_b , LOW);
  digitalWrite(motor2_a , LOW);
  digitalWrite(motor2_b , HIGH);
  // delay(2000);
}

void motores_parados()
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
#ifdef TESTE_SENSORES
  Serial.print(leitura1);
  Serial.print("\t");
  Serial.print(leitura2);
  Serial.print("\t");
  Serial.println(leitura3);
  Serial.print("\t");
  Serial.println();
#endif

#ifdef PRINCIPAL
  //Bloco de montagem das estrutura do array_sensores//
  if (leitura1 > LIMIAR_SENSORES && leitura2 > LIMIAR_SENSORES && leitura3 > LIMIAR_SENSORES)
  {
    array_sensores = 0b111;
  } else if (leitura1 < LIMIAR_SENSORES && leitura2 > LIMIAR_SENSORES && leitura3 < LIMIAR_SENSORES)
  {
    array_sensores = 0b010;
  } else if (leitura1 < LIMIAR_SENSORES && leitura2 < LIMIAR_SENSORES && leitura3 > LIMIAR_SENSORES)
  {
    array_sensores = 0b001;
  } else if (leitura1 < LIMIAR_SENSORES && leitura2 > LIMIAR_SENSORES && leitura3 < LIMIAR_SENSORES)
  {
    array_sensores = 0b010;
  } else if (leitura1 < LIMIAR_SENSORES && leitura2 > LIMIAR_SENSORES && leitura3 > LIMIAR_SENSORES)
  {
    array_sensores = 0b011;
  } else if (leitura1 > LIMIAR_SENSORES && leitura2 < LIMIAR_SENSORES && leitura3 < LIMIAR_SENSORES)
  {
    array_sensores = 0b100;
  } else if (leitura1 > LIMIAR_SENSORES && leitura2 > LIMIAR_SENSORES && leitura3 < LIMIAR_SENSORES)
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
    erro_array = 3;

  } else if (array_sensores == 0b000) {
    erro_array = 0;

  }
#ifdef MOSTRAR_VALORES
  Serial.print("Valor do erro:");
  Serial.print("  ");
  Serial.print(erro_array);
  Serial.print("  ");
#endif
  //Compensador PID//
  erro_atual = erro_array;
  resposta_p = erro_atual * Kp;
  resposta_i = resposta_i + erro_atual * Ki * A;
  resposta_d = Kd * (erro_atual - erro_anterior);
  Resposta_PID = resposta_p + resposta_i + resposta_d;
  erro_anterior = erro_atual;
#ifdef MOSTRAR_VALORES
  Serial.print("\t");
  Serial.print("Resposta PID:");
  Serial.print("  ");
  Serial.print(Resposta_PID);
  Serial.print("  ");
#endif
  //PRINCIPAL//
  if (erro_array == 0) {
    frente();
  }
  if (erro_array == 1) {
    curva_a_esquerda(Resposta_PID);
  }
  if (erro_array == 2) {
    curva_a_esquerda(Resposta_PID);
  }
  if (erro_array == -1) {
    curva_a_direita(Resposta_PID);
  }
  if (erro_array == -2) {
    curva_a_direita(Resposta_PID);
  }
  if (erro_array == 3) {
    motores_parados();
    delay(5000);
  }
#ifdef MOSTRAR_VALORES
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

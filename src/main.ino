//Defines//

///////////

#define LIMIAR_SENSORES 300
#define CPD 115
#define CP 15



//#define TESTE_SENSORES

#define PRINCIPAL

//#define MOSTRAR_VALORES



/////////////////

//Dados globais//

////////////////

//Sensores//
int sensor_0 = A0;

int sensor_1 = A1;


int sensor_2 = A2;

int sensor_3 = A3;

int sensor_4 = A4;

uint8_t array_sensores = 0;

int leitura0;
int leitura1;

int leitura2;

int leitura3;

int leitura4;

//Compensador PID//

int Kp = CP;

int Kd = CPD;

int resposta_p = 0;

int resposta_d = 0;

int Resposta_PID = 0;

int erro_array = 0;

int amostra_atual = 0;

int amostra_anterior = 0;

int erro_atual = 0;

int erro_anterior = 0;

//Velocidade//

int velocidade_f;
int velocidade_1;
int velocidade_2;
int velocidade_3;
int velocidade_4;
int velocidade_5;

int respPI;

int Vplus, Vless;

int subindo;

int IN0A, IN1A, IN0B, IN1B;

int ENA = 9; // motor esquerdo

int ENB = 10; //motor direito

//declaracao dos motores//

int motor1_a = 4;

int motor1_b = 6;

int motor2_a = 5;

int motor2_b = 7;
//ligacao do robo//
int robo_ligado = 0;
int linha_final;



//////////////////////////////////////////////////////////////////
/////////////////Bloco das funções no plano//////////////////////
/////////////////////////////////////////////////////////////////
void frente(int velocidade) {

  analogWrite(ENA, velocidade );

  analogWrite(ENB, velocidade );



  digitalWrite(motor1_a , HIGH);

  digitalWrite(motor1_b , LOW);

  digitalWrite(motor2_a , LOW);

  digitalWrite(motor2_b , HIGH);

  Serial.print("\t");
  Serial.print(velocidade);

}

//Giro a esquerda desligando um dos motores//

void giro_a_esquerda(int velocidade, int respPI)
{

  Vplus = velocidade + respPI;
  if (Vplus > 255) {
    Vplus = 255;
  } else if (Vplus < 0) {
    Vplus = 0;
  }

  Vless = velocidade - respPI;
  if (Vless > 255) {
    Vless = 255;
  } else if (Vless < 0) {
    Vless = 0;
  }

  analogWrite(ENA, Vplus );

  analogWrite(ENB, Vless );



  digitalWrite(motor1_a , HIGH);

  digitalWrite(motor1_b , LOW);

  digitalWrite(motor2_a , LOW);

  digitalWrite(motor2_b , LOW);

  Serial.print("giro a esquerda");
  Serial.print("\t");
  Serial.print("motor ENA");
  Serial.print("\t");
  Serial.print(Vplus);
  Serial.print("\t");
  Serial.print("motor ENB");
  Serial.print("\t");
  Serial.print(Vless);
}

void curva_esquerda(int velocidade, int respPI)
{

  Vplus = velocidade + respPI;
  if (Vplus > 255) {
    Vplus = 255;
  } else if (Vplus < 0) {
    Vplus = 0;
  }

  Vless = velocidade - respPI;
  if (Vless > 255) {
    Vless = 255;
  } else if (Vless < 0) {
    Vless = 0;
  }

  analogWrite(ENA, Vplus );

  analogWrite(ENB, Vless );

  digitalWrite(motor1_a , HIGH);

  digitalWrite(motor1_b , LOW);

  digitalWrite(motor2_a , LOW);

  digitalWrite(motor2_b , HIGH);

  Serial.print("curva a esquerda");
  Serial.print("\t");
  Serial.print("motor ENA");
  Serial.print("\t");
  Serial.print(Vplus);
  Serial.print("\t");
  Serial.print("motor ENB");
  Serial.print("\t");
  Serial.print(Vless);

}

//Giro a direita desligando um dos motores//

void giro_a_direita(int velocidade, int respPI)

{

  Vplus = velocidade + respPI;
  if (Vplus > 255) {
    Vplus = 255;
  } else if (Vplus < 0) {
    Vplus = 0;
  }

  Vless = velocidade - respPI;
  if (Vless > 255) {
    Vless = 255;
  } else if (Vless < 0) {
    Vless = 0;
  }

  analogWrite(ENA, Vplus ); //neste caso e o menor pois o erro e negativo

  analogWrite(ENB, Vless ); //neste caso e o maior pois o erro e negativo



  digitalWrite(motor1_a , LOW);

  digitalWrite(motor1_b , LOW);

  digitalWrite(motor2_a , LOW);

  digitalWrite(motor2_b , HIGH);

  Serial.print("giro a direita");
  Serial.print("\t");
  Serial.print("motor ENB");
  Serial.print("\t");
  Serial.print(Vless);
  Serial.print("\t");
  Serial.print("motor ENA");
  Serial.print("\t");
  Serial.print(Vplus);

}

void curva_direita(int velocidade, int respPI)
{

  Vplus = velocidade + respPI;
  if (Vplus > 255) {
    Vplus = 255;
  } else if (Vplus < 0) {
    Vplus = 0;
  }

  Vless = velocidade - respPI;
  if (Vless > 255) {
    Vless = 255;
  } else if (Vless < 0) {
    Vless = 0;

  }

  analogWrite(ENA, Vplus ); //neste caso e o menor pois o erro e negativo

  analogWrite(ENB, Vless ); //neste caso e o maior pois o erro e negativo


  digitalWrite(motor1_a , HIGH);

  digitalWrite(motor1_b , LOW);

  digitalWrite(motor2_a , LOW);

  digitalWrite(motor2_b , HIGH);

  Serial.print("curva a direita");
  Serial.print("\t");
  Serial.print("motor ENB");
  Serial.print("\t");
  Serial.print(Vless);
  Serial.print("\t");
  Serial.print("motor ENA");
  Serial.print("\t");
  Serial.print(Vplus);
}


void motores_parados()

{


  digitalWrite(motor1_a , HIGH);

  digitalWrite(motor1_b , HIGH);

  digitalWrite(motor2_a , HIGH);

  digitalWrite(motor2_b , HIGH);
  Serial.print("\t");
  Serial.print("desliguei");
  delay(5000);

}

void setup() {

  Serial.begin(9600);
  pinMode(13, OUTPUT);


}



void loop() {


  velocidade_f = 255;
  velocidade_1 = 255;
  velocidade_2 = 200;
  velocidade_3 = 140;
  velocidade_4 = 80;
  velocidade_5 = 40;

  leitura0 = analogRead(sensor_0);

  leitura1 = analogRead(sensor_1);

  leitura2 = analogRead(sensor_2);

  leitura3 = analogRead(sensor_3);

  leitura4 = analogRead(sensor_4);


  while ( robo_ligado == 0) {
    delay (1000);
    robo_ligado++;
  }

  //teste sensores//

#ifdef TESTE_SENSORES

  Serial.print(leitura0);

  Serial.print("\t");

  Serial.print(leitura1);

  Serial.print("\t");

  Serial.print(leitura2);

  Serial.print("\t");

  Serial.print(leitura3);

  Serial.print("\t");

  Serial.print(leitura4);

  Serial.print("\t");

  Serial.println();
#endif


  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef PRINCIPAL
  if (millis() >= 13250  && millis() < 15750) { //USAR UM SEGUNDO A MENOS QUE O DESEJADO
    Kp = 15;
    Kd = 35;
    velocidade_f = 33;
    velocidade_1 = 28;
    velocidade_2 = 25;
    velocidade_3 = 25;
    velocidade_4 = 25;
    velocidade_5 = 25;
    digitalWrite(13, HIGH);
  } else {
    Kp = CP;
    Kd = CPD;
  velocidade_f = 255;
  velocidade_1 = 240;
  velocidade_2 = 200;
  velocidade_3 = 130;
  velocidade_4 = 80;
  velocidade_5 = 40;
    digitalWrite(13, LOW);
  }
  //Bloco de montagem das estrutura do array_sensores//
  //if ((leitura0 < LIMIAR_SENSORES && leitura1 < LIMIAR_SENSORES && leitura2 < LIMIAR_SENSORES && leitura3 < LIMIAR_SENSORES && leitura4 < LIMIAR_SENSORES) && millis() >= 22000)
  if (millis() >= 19200)
  {
    /* linha_final = millis();
      while(1){

       if((millis() - linha_final) > 9000)
    */
    frente(velocidade_f);
    digitalWrite(13, HIGH);

    {
      motores_parados();

    }


  } else if (leitura0 > LIMIAR_SENSORES && leitura1 > LIMIAR_SENSORES && leitura2 < LIMIAR_SENSORES && leitura3 > LIMIAR_SENSORES && leitura4 > LIMIAR_SENSORES)
  {

    array_sensores = 0b00100;

  } else if (leitura0 > LIMIAR_SENSORES && leitura1 > LIMIAR_SENSORES && leitura2 > LIMIAR_SENSORES && leitura3 < LIMIAR_SENSORES && leitura4 > LIMIAR_SENSORES)
  {

    array_sensores = 0b00010;

  } else if (leitura0 > LIMIAR_SENSORES && leitura1 > LIMIAR_SENSORES && leitura2 < LIMIAR_SENSORES && leitura3 < LIMIAR_SENSORES && leitura4 > LIMIAR_SENSORES)
  {

    array_sensores = 0b00110;

  } else if (leitura0 > LIMIAR_SENSORES && leitura1 < LIMIAR_SENSORES && leitura2 > LIMIAR_SENSORES && leitura3 > LIMIAR_SENSORES && leitura4 > LIMIAR_SENSORES)
  {

    array_sensores = 0b01000;

  } else if (leitura0 > LIMIAR_SENSORES && leitura1 < LIMIAR_SENSORES && leitura2 < LIMIAR_SENSORES && leitura3 > LIMIAR_SENSORES && leitura4 > LIMIAR_SENSORES)
  {

    array_sensores = 0b01100;

  } else if (leitura0 < LIMIAR_SENSORES && leitura1 < LIMIAR_SENSORES && leitura2 > LIMIAR_SENSORES && leitura3 > LIMIAR_SENSORES && leitura4 > LIMIAR_SENSORES)
  {

    array_sensores = 0b11000;

  } else if (leitura0 < LIMIAR_SENSORES && leitura1 < LIMIAR_SENSORES && leitura2 < LIMIAR_SENSORES && leitura3 > LIMIAR_SENSORES && leitura4 > LIMIAR_SENSORES)
  {

    array_sensores = 0b11100;

  } else if (leitura0 < LIMIAR_SENSORES && leitura1 > LIMIAR_SENSORES && leitura2 > LIMIAR_SENSORES && leitura3 > LIMIAR_SENSORES && leitura4 > LIMIAR_SENSORES)
  {

    array_sensores = 0b10000;

  } else if (leitura0 > LIMIAR_SENSORES && leitura1 > LIMIAR_SENSORES && leitura2 < LIMIAR_SENSORES && leitura3 < LIMIAR_SENSORES && leitura4 < LIMIAR_SENSORES)
  {

    array_sensores = 0b00111;

  } else if (leitura0 > LIMIAR_SENSORES && leitura1 > LIMIAR_SENSORES && leitura2 > LIMIAR_SENSORES && leitura3 < LIMIAR_SENSORES && leitura4 < LIMIAR_SENSORES)
  {

    array_sensores = 0b00011;

  } else if (leitura0 > LIMIAR_SENSORES && leitura1 > LIMIAR_SENSORES && leitura2 > LIMIAR_SENSORES && leitura3 > LIMIAR_SENSORES && leitura4 < LIMIAR_SENSORES)
  {

    array_sensores = 0b00001;

  } else if (leitura0 < LIMIAR_SENSORES && leitura1 < LIMIAR_SENSORES && leitura2 < LIMIAR_SENSORES && leitura3 < LIMIAR_SENSORES && leitura4 > LIMIAR_SENSORES)
  {

    array_sensores = 0b11110;

  } else if (leitura0 > LIMIAR_SENSORES && leitura1 < LIMIAR_SENSORES && leitura2 < LIMIAR_SENSORES && leitura3 < LIMIAR_SENSORES && leitura4 < LIMIAR_SENSORES)
  {

    array_sensores = 0b01111;

  }

  //Calculo dos erros//

  if (array_sensores == 0b01000) {

    erro_array = 2;

  } else if (array_sensores == 0b00100) {

    erro_array = 0;


  } else if (array_sensores == 0b11110) {

    erro_array = 6;


  } else if (array_sensores == 0b00010) {

    erro_array = -2;

  } else if (array_sensores == 0b00011 ) {

    erro_array = -3;


  } else if (array_sensores == 0b00110) {

    erro_array = -1;


  } else if (array_sensores == 0b01100) {

    erro_array = 1;


  } else if (array_sensores == 0b10000) {

    erro_array = 4;


  } else if (array_sensores == 0b11000) {

    erro_array = 3;


  } else if (array_sensores == 0b00001) {

    erro_array = -4;


  } else if (array_sensores == 0b11100) {

    erro_array = 5;


  } else if (array_sensores == 0b00111) {

    erro_array = -5;


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

  resposta_d = Kd * (erro_atual - erro_anterior);

  Resposta_PID = resposta_p + resposta_d;

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

    frente(velocidade_f);

  }

  if (erro_array == 1) {

    //giro_a_esquerda(velocidade, Resposta_PID); // erro positivo virar à esqerda
    curva_esquerda(velocidade_1, Resposta_PID);
  }

  if (erro_array == 2) {

    //giro_a_esquerda(velocidade, Resposta_PID);
    curva_esquerda(velocidade_2, Resposta_PID);

  } if (erro_array == 3) {

    //giro_a_esquerda(velocidade, Resposta_PID);
    curva_esquerda(velocidade_3, Resposta_PID);

  } if (erro_array == 4) {

    //giro_a_esquerda(velocidade, Resposta_PID);
    curva_esquerda(velocidade_4, Resposta_PID);

  } if (erro_array == 5) {

    giro_a_esquerda(velocidade_5, Resposta_PID);
    //curva_esquerda(velocidade, Resposta_PID);

  } if (erro_array == 6) {

    giro_a_esquerda(velocidade_5, Resposta_PID);
    //curva_esquerda(velocidade, Resposta_PID);

  }

  if (erro_array == -1) {

    //giro_a_direita(velocidade, Resposta_PID); // erro negativo virar à esquerda
    curva_direita(velocidade_1, Resposta_PID);
  }

  if (erro_array == -2) {

    //giro_a_direita(velocidade, Resposta_PID);
    curva_direita(velocidade_2, Resposta_PID);

  } if (erro_array == -3) {

    //giro_a_direita(velocidade, Resposta_PID);
    curva_direita(velocidade_3, Resposta_PID);

  } if (erro_array == -4) {

    //giro_a_direita(velocidade, Resposta_PID);
    curva_direita(velocidade_4, Resposta_PID);

  } if (erro_array == -5) {

    giro_a_direita(velocidade_5, Resposta_PID);
    //curva_direita(velocidade, Resposta_PID);

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

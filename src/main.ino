////////////////////
// Sesnr Inercial //
////////////////////
/*Esquema de montagem:
Arduino - Dispositivo
A4      - SDA do GY-521
A5      - SCL do GY-521
5V      - VCC do GY-521
GND     - GND do GY-521*/

//Bibliotecas do sensor inercial
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

#define QPMM 10 // Qnt de pontos media movel - Sera feita a media de 10 pontos
#define PSDMP 42 //Packet Size DMP - tam do pacote interno da mpu-6050

//Uso do Millis com um timer
#define mpu_interval 10 //Each 10ms
unsigned long previousMPUMillis = 0;

//Variaveis Inercial
MPU6050 mpu(0x68);

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
int numbPackets;

/**
* Inicializa a comunicação I2C
* Inicializa o sensor
* Configura os dados de calibração
*/
void mpu6050_init() {
  //Sensor Inercial
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  Wire.setClock(200000); //NOTE: Ajustar de acordo com arduino utilizado
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
  #endif
  if (mpu.testConnection()) {
    mpu.initialize(); //Initializes the IMU
    uint8_t ret = mpu.dmpInitialize(); //Initializes the DMP
    delay(50);
    if (ret == 0) {
      mpu.setDMPEnabled(true);
      mpu.setXAccelOffset(-1099);
      mpu.setYAccelOffset(-14);
      mpu.setZAccelOffset(454);
      mpu.setXGyroOffset(83);
      mpu.setYGyroOffset(-48);
      mpu.setZGyroOffset(21);
      //DEBUG_PRINT__("Sensor Inercial configurado com sucesso.\n");
    } else {
      //TODO: adicionar uma forma melhor de aviso. outro led?
      //DEBUG_PRINT__("Erro na inicializacao do sensor Inercial!\n");
    }
  } else {
    //DEBUG_PRINT__("Erro na conexao do sensor Inercial.\n");
  }
}

/**
* Essa função é chamada pelo update a cada mpu_interval milisegundos
*/
void mpu6050_data_update() {
  numbPackets = floor(mpu.getFIFOCount() / PSDMP);
  ////DEBUG_PRINT__(numbPackets); //DEBUG_PRINT__(" - ");
  if (numbPackets >= 24) {
    mpu.resetFIFO();
    //DEBUG_PRINT__("FIFO sensor 1 overflow!\n"); //TODO: mostrar isso de alguma forma. outro led?
  } else {
    while (numbPackets > 0) {
      mpu.getFIFOBytes(fifoBuffer, PSDMP);
      numbPackets--;
    }
    // display Euler angles in degrees
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetEuler(euler, &q);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
  }
}

/**
* Colocar dentro do loop, NÂO PODE TER DELAYS
*/
void mpu6050_update(){
  if (millis() - previousMPUMillis >= mpu_interval) {
    previousMPUMillis = millis();
    mpu6050_data_update(); //Realiza leitura e envia pacote(ou mostra) dados
  }
}


/**
* Verifica se esta em subida, plano ou descida
* @param  eixo   0 1 ou 2 indicando o index dos eixos x,y e z. Default: 1
* @param  limiar Quantos graus ele considera rampa ou plano. Default: 15º
* @return 1 se em subida, 0 se no plano e -1 se na descida
*/
int8_t mpu6050_verify(uint8_t eixo=1, uint8_t limiar=15){
  if(abs(euler[eixo] * 180/M_PI) < limiar){
    //digitalWrite(13, 0); //E estabiliza
    return 0;
  } else if(euler[eixo] * 180/M_PI < -limiar){
    //digitalWrite(13, 1); //E sobe
    return 1;
  } else if(euler[eixo] * 180/M_PI > limiar){
    //digitalWrite(13, 1); //E desce
    return -1;
  } else {
    //Caso não for contemplado acima
    return 0;
  }
}

/**
* Exemplo de Uso:
* //Bibliotecas
* //Defines
* //Variaveis
* void setup(){
*      mpu6050_init();
* }
* void loop(){
*      mpu6050_update();
*      if(mpu6050_verify(1,15) == 0){
*          Serial.println("Estou no plano");
*      } else {
*          Serial.println("Não Estou no plano");
*      }
* }
*/
///////////
//Defines//
///////////
#define LIMIAR_SENSORES 750

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
int A = 0.7;
int Kp = 25;
int Ki = 35;
int Kd = 20;
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
int velocidade;
int respPI;
int Vel_up = 255;   // Velocidade de subida
int Vel_med = 110;   // Velocidade no plano
int Vel_down = 80;  // Velocidade de descida
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
void frente(int velocidade) {


  velocidade = 90;
  analogWrite(ENA, abs(velocidade) );
  analogWrite(ENB, abs(velocidade) );

  digitalWrite(motor1_a , HIGH);
  digitalWrite(motor1_b , LOW);
  digitalWrite(motor2_a , LOW);
  digitalWrite(motor2_b , HIGH);
}

//curva compensada à esquerda//
void curva_a_esquerda(int velocidade, int respPI) {

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
void curva_a_direita(int velocidade, int respPI) {

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
void giro_a_esquerda(int velocidade, int respPI)
{
  velocidade = 130;
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
void giro_a_direita(int velocidade, int respPI)
{
  velocidade = 130;
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
  pinMode(13, OUTPUT);
  mpu6050_init();
}

void loop() {
  leitura1 = analogRead(sensor_1);
  leitura2 = analogRead(sensor_2);
  leitura3 = analogRead(sensor_3);

  mpu6050_update(); //Lê o sensor inercial

  //Acelerômetro//
  if(mpu6050_verify(1,15) == 0){
    Serial.println("Estou no plano");
    digitalWrite(13, 0); //plano
    //velocidade = Vel_med;
  } else if(mpu6050_verify(1,15) == 1){
    Serial.println("Estou subindo");
    digitalWrite(13, 1); //E sobe
    velocidade = Vel_up;
    frente(velocidade);
  } else
  {
    Serial.println("Estou descendo");
    digitalWrite(13, 1); //E desce
    velocidade = Vel_down;
  }

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
    frente(velocidade);
  }
  if (erro_array == 1) {
    giro_a_esquerda(velocidade, Resposta_PID); // erro positivo virar à esqerda
  }
  if (erro_array == 2) {
    giro_a_esquerda(velocidade, Resposta_PID);
  }
  if (erro_array == -1) {
    giro_a_direita(velocidade, Resposta_PID); // erro negativo virar à esquerda
  }
  if (erro_array == -2) {
    giro_a_direita(velocidade, Resposta_PID);
  }
  if (erro_array == 3) {
    motores_parados();
    //delay(5000);
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

/**
  UNIVERSIDADE FEDERAL DE UBERLÂNDIA - UFU
  Autor: TODO: Mario Divino Borges Filho
  Data:  TODO: 05/07/2018
  Git:  TODO: https://github.com/alpinista06

  Descrição:  TODO: Biblioteca de configuraçao e ultilização de sensor inercial
  expecificamente para a media de ângulos
*/
#ifndef _INCR_H_
#define _INCR_H_ //Previne quando multiplas inclusões

////////////////////
// Sesnr Inercial //
////////////////////
/*Esquema de montagem:
Arduino - Dispositivo
A4      - SDA do GY-521
A5      - SCL do GY-521
5V      - VCC do GY-521
GND     - GND do GY-521*/

//////////////////////////////////////
//Dependencias (Outras Bibliotecas) //
//////////////////////////////////////

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

///////////////
//Definições //
///////////////

#define QPMM 10 // Qnt de pontos media movel - Sera feita a media de 10 pontos
#define PSDMP 42 //Packet Size DMP - tam do pacote interno da mpu-6050

//Uso do Millis com um timer
#define mpu_interval 10 //Each 10ms


//////////////////////
//Variáveis globais //
//////////////////////

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

//////////////////////////
//Funções da Biblioteca //
//////////////////////////
/**
* Inicializa a comunicação I2C
* Inicializa o sensor
* Configura os dados de calibração
*/
void mpu6050_init();

/**
* Essa função é chamada pelo update a cada mpu_interval milisegundos
*/
void mpu6050_data_update();

/**
* Colocar dentro do loop, NÂO PODE TER DELAYS
*/
void mpu6050_update();

/**
* Verifica se esta em subida, plano ou descida
* @param  eixo   0 1 ou 2 indicando o index dos eixos x,y e z. Default: 1
* @param  limiar Quantos graus ele considera rampa ou plano. Default: 15º
* @return 1 se em subida, 0 se no plano e -1 se na descida
*/
int8_t mpu6050_verify(uint8_t eixo=1, uint8_t limiar=15);


#endif /*_INRC_H_*/

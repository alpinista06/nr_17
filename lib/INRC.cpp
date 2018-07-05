#include "INRC.h"

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

void mpu6050_update(){
  if (millis() - previousMPUMillis >= mpu_interval) {
    previousMPUMillis = millis();
    mpu6050_data_update(); //Realiza leitura e envia pacote(ou mostra) dados
  }
}

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

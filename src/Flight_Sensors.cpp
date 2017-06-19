/*
Author: John Dean
Date: 1-6-2017
*/

#include "Flight_Sensors.h"
#include <math.h>


/*Sensors*/
Flight_Sensors::Flight_Sensors()
  : bmp1(Adafruit_BMP280(BMP1_CS)),
    bmp2(Adafruit_BMP280(BMP2_CS)),
    bno(Adafruit_BNO055(55)),
    mma(MMA65XX_Sensor(MMA_CS)),
    gps(&GPS_SERIAL) {}

bool Flight_Sensors::initialize(){
  bool sucessful = 0;

  pinMode(ESENSE_1, INPUT);
  pinMode(ESENSE_2, INPUT);
  pinMode(ESENSE_3, INPUT);
  pinMode(ESENSE_4, INPUT);

  pinMode(ISO_SENSE_11, INPUT);
  pinMode(ISO_SENSE_12, INPUT);
  pinMode(ISO_SENSE_13, INPUT);
  pinMode(ISO_SENSE_21, INPUT);
  pinMode(ISO_SENSE_22, INPUT);
  pinMode(ISO_SENSE_23, INPUT);

  pinMode(VSENSE, INPUT);
  analogReadResolution(12);
  sucessful |= bmp1.begin()<<1;
  sucessful |= bmp2.begin()<<2;
  sucessful |= mma.begin()<<3;
  sucessful |= gps.begin()<<4;
  sucessful |= bno.begin()<<5;
  if(sucessful&&(0b11111)) return true;
  else {
    if(~sucessful&(1<<1)) Serial.println("bmp1 init error");
    if(~sucessful&(1<<2)) Serial.println("bmp2 init error");
    if(~sucessful&(1<<3)) Serial.println("mma init error");
    if(~sucessful&(1<<4)) Serial.println("gps init error");
    if(~sucessful&(1<<5)) Serial.println("bno init error");
    return false;
  }
}

bool Flight_Sensors::update() {
  return gps.update();
}

byte Flight_Sensors::readESense(){
  byte esense_array = 0;
  if(digitalReadFast(ESENSE_1)){esense_array|=(1<<7);}
  if(digitalReadFast(ESENSE_2)){esense_array|=(1<<6);}
  if(digitalReadFast(ESENSE_3)){esense_array|=(1<<5);}
  if(digitalReadFast(ESENSE_4)){esense_array|=(1<<4);}
  return esense_array;
}

byte Flight_Sensors::readIsoSense(){
  byte iso_sense_array = 0;
  if(digitalReadFast(ISO_SENSE_11)){iso_sense_array|=(1<<7);}
  if(digitalReadFast(ISO_SENSE_12)){iso_sense_array|=(1<<6);}
  if(digitalReadFast(ISO_SENSE_13)){iso_sense_array|=(1<<5);}
  if(digitalReadFast(ISO_SENSE_21)){iso_sense_array|=(1<<4);}
  if(digitalReadFast(ISO_SENSE_22)){iso_sense_array|=(1<<3);}
  if(digitalReadFast(ISO_SENSE_23)){iso_sense_array|=(1<<2);}
  return iso_sense_array;
}

Bno_Data Flight_Sensors::readBNO(){
  elapsedMicros timer = 0;
  Bno_Data bno_data;
  imu::Vector<3> vec;
  vec = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
  bno_data.lin_a.x = vec[0];
  bno_data.lin_a.y = vec[1];
  bno_data.lin_a.z = vec[2];
  //Serial.print("1: ");
  //Serial.println(timer);
  vec = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
  //Serial.print("2: ");
  //Serial.println(timer);
  bno_data.gyro.x = vec[0];
  bno_data.gyro.y = vec[1];
  bno_data.gyro.z = vec[2];
  vec = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  bno_data.rot_a.x = vec[0];
  bno_data.rot_a.y = vec[1];
  bno_data.rot_a.z = vec[2];
  //Serial.print("3: ");
  //Serial.println(timer);
  vec = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  bno_data.euler.x = vec[0];
  bno_data.euler.y = vec[1];
  bno_data.euler.z = vec[2];
  imu::Quaternion quat = bno.getQuat();
  bno_data.quat.w = quat.w();
  bno_data.quat.x = quat.x();
  bno_data.quat.y = quat.y();
  bno_data.quat.z = quat.z();
  Serial.println(timer);

  return bno_data;
}

Bmp_Data Flight_Sensors::readBMP(){
  //void SpiFlushRxFifo(void);
  Bmp_Data bmp_data;
  bmp_data.pressure1 = bmp1.readPressure();
  bmp_data.pressure2 = bmp2.readPressure();
  return bmp_data;
}

Mma_Data Flight_Sensors::readMMA() {

  Mma_Data ret;
  sensors_event_t evt;
  mma.getEvent(&evt);
  ret.x = evt.acceleration.x;
  ret.y = evt.acceleration.y;

  return ret;
}

Gps_Data Flight_Sensors::readGPS() {
  return gps.readPosition();
}

float Flight_Sensors::readVbat(){
  int read = analogRead(VSENSE);
  float vbat = ((float)read)/4095.0 * 3.3 / (VBAT_RATIO);
  return vbat;
}

/*
Author: John Dean
Date: 1-6-2017
*/

#include "Flight_Sensors.h"


/*Sensors*/
Flight_Sensors::Flight_Sensors()
  : bmp1(Adafruit_BMP280(BMP1_CS)),
    bmp2(Adafruit_BMP280(BMP2_CS)),
    bno(Adafruit_BNO055(BNO_ADR)),
    mma(MMA65XX_Sensor(MMA_CS)),
    gps(&GPS_SERIAL) {}

bool Flight_Sensors::initialize(){
  bool sucessful = true;

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

  sucessful &= bmp1.begin();
  sucessful &= bmp2.begin();
  sucessful &= mma.begin();
  sucessful &= gps.begin();

  return sucessful;
}

void Flight_Sensors::update() {
  gps.update();
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

Bmp_Data* Flight_Sensors::readPressure(){
  Bmp_Data* bmp_data = new Bmp_Data;
  bmp_data->pressure1 = bmp1.readPressure();
  bmp_data->pressure2 = bmp2.readPressure();
  return bmp_data;
}

Mma_Data* Flight_Sensors::readAcceleration() {

  Mma_Data* ret = new Mma_Data;
  sensors_event_t evt;

  mma.getEvent(&evt);
  ret->x = evt.acceleration.x;
  ret->y = evt.acceleration.y;

  return ret;
}

Gps_Data* Flight_Sensors::readPosition() {
  return gps.readPosition();
}

float Flight_Sensors::readVbat(){
  int read = analogRead(VSENSE);
  float vbat = ((float)read)/1023.0 * 3.3 / (VBAT_RATIO);
  return vbat;
}

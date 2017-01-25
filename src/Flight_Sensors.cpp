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
    mma(MMA65XX_Sensor(MMA_CS)){}

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

  //code to initialize all sensors goes here

  return sucessful;
}

bool* Flight_Sensors::readESense(){
  bool* esense_array = new bool[4];
  esense_array[0] = digitalReadFast(ESENSE_1);
  esense_array[1] = digitalReadFast(ESENSE_2);
  esense_array[2] = digitalReadFast(ESENSE_3);
  esense_array[3] = digitalReadFast(ESENSE_4);
  return esense_array;
}

bool* Flight_Sensors::readIsoSense(){
  bool* iso_sense_array = new bool[6];
  iso_sense_array[0] = digitalReadFast(ISO_SENSE_11);
  iso_sense_array[1] = digitalReadFast(ISO_SENSE_12);
  iso_sense_array[2] = digitalReadFast(ISO_SENSE_13);
  iso_sense_array[3] = digitalReadFast(ISO_SENSE_21);
  iso_sense_array[4] = digitalReadFast(ISO_SENSE_22);
  iso_sense_array[5] = digitalReadFast(ISO_SENSE_23);
  return iso_sense_array;
}

Bmp_Data* Flight_Sensors::readPressure(){
  Bmp_Data* bmp_data = new Bmp_Data;

  //code to actaually read sensors goes here

  return bmp_data;
}

float* Flight_Sensors::readAcceleration() {
  
  float* ret = new float[2];
  sensors_event_t evt;

  mma.getEvent(&evt);
  ret[0] = evt.acceleration.x;
  ret[1] = evt.acceleration.y;

  return ret;
}

float Flight_Sensors::readVbat(){
  int read = analogRead(VSENSE);
  float vbat = ((float)read)/1023.0 * 3.3 / (VBAT_RATIO);
  return vbat;
}

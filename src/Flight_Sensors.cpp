/*
Author: John Dean
Date: 1-6-2017
*/

#include "Flight_Sensors.h"


/*Sensors*/
Flight_Sensors::Flight_Sensors(){
  this->bmp1 = Adafruit_BMP280(BMP1_CS);
  this->bmp2 = Adafruit_BMP280(BMP2_CS);
  this->bno  = Adafruit_BNO055(BNO_ADR);
  //this->mma  = MMA6525(MMA_CS)
  this->adc  = new ADC();

}

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

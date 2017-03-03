/*

Author: John Dean
Date: 1-6-2017
*/

#include "Flight_Data.h"

 /*Flight_Data::Flight_Data() {

 }*/

bool Flight_Data::initialize(){
  bool success = true;
  /*global_time = 0;
  success &= sd.begin(SD_CS, SPI_FULL_SPEED);
  sd.ls(LS_SIZE|LS_R);
  sd.mkdir("Flights");
  sd.chdir("Flights");
  String filepath;
  filepath += "F000.csv";
  while (sd.exists(filepath.c_str())) {
    if (filepath[3] != '9') {
      filepath[3]++;
    } else if (filepath[2] != '9'){
      filepath[2]++;
      filepath[3] = '0';
    } else if (filepath[1] != '9'){
      filepath[1]++;
      filepath[2] = 0;
      filepath[3] = 0;
    }
  }
  Serial.print("Creating new data file, ");
  Serial.println(filepath);*/
  // success &= data_file.contigu
  // success &= data_file.open(filepath.c_str(),O_RDWR | O_CREAT | O_AT_END);
  // data_file.println("lets try and write some shit");
  // for(int i = 0; i < 1000; i++){
  //   elapsedMicros timer = 0;
  //   data_file.print(global_time);
  //   data_file.print(",");
  //   data_file.print("DAAAAAATTTTTTTAAAAAAAAATTTTTAAAAAA");
  //   data_file.sync();
  //   Serial.println(timer);
  //   delay(10);
  // }

  return success;
}
void Flight_Data::updateESense(byte esense_array){
  this->esense_array = esense_array;
  //dataLog(FESENSE);
}

void Flight_Data::updateIsoSense(byte iso_sense_array){
  this->iso_sense_array = iso_sense_array;
}

byte Flight_Data::getESense(){
  return this->esense_array;
}
byte Flight_Data::getIsoSense(){
  return this->iso_sense_array;
}

void Flight_Data::updateBMP(Bmp_Data* bmp_data){
  this->bmp_data.pressure1 = bmp_data->pressure1;
  this->bmp_data.pressure2 = bmp_data->pressure2;

  delete bmp_data;
}

void Flight_Data::writeBuffers(){
  // int i = millis();
  // test_buffer.push(i);
}

void Flight_Data::printBuffers(){
  // int* buffer = test_buffer.getFullArray();
  // for(int i = 0; i < ARRAYLENGTH; i++){
  //   Serial.print(buffer[i]);
  //   Serial.print(',');
  // }
  // Serial.println(' ');
  // delete buffer;
}

/* ------ SD card ------ */



void Flight_Data::printDirectory(File dir, int numTabs) {
  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}

File Flight_Data::newDataFile(File dir){
  int n = 0;
  while(true){
    n++;
    File entry = dir.openNextFile();
    if(! entry){
      // no more files
      String filepath;
      filepath += dir.name();
      filepath += "/f";
      if(n<10) filepath += "00";      //pad with 0's
      else if(n<100) filepath += "0";
      filepath += n;
      filepath += ".csv";
      File newfile = sd.open(filepath.c_str(),FILE_WRITE);
      if (newfile){
        Serial.print("success  ");
      } else {
        Serial.print("fail  ");
      }
      //Serial.println(filepath);
      return newfile;
    }
    entry.close();
  }
}

/* ----- Circular_Storage_Buffer ------ */

template<class t_type> t_type Circular_Array<t_type>::getOld(uint32_t get_time){
   int16_t ind = static_cast<float>(sample_freq) * get_time / store_prescale / 1000;
   ind = head - ind;
   if(ind < 0) ind += array_length;
   return data_array[ind];
}

template<class t_type> void Circular_Array<t_type>::push(t_type data){
  write_count++;
  if(write_count >= store_prescale){
    write_count = 0;
    head++;
    checkHead();
  }
  data_array[head] = data;
}

template<class t_type> t_type Circular_Array<t_type>::getLast(){
  return data_array[head];
}

template<class t_type> t_type* Circular_Array<t_type>::getFullArray(){
  t_type* full_array = new t_type[array_length];
  for(int i = 0; i <= head; i++){
    full_array[i] = data_array[head-i];
  }
  for(int i = head+1; i < array_length; i++){
    full_array[i] = data_array[(array_length+head)-i];
  }
  return full_array;
}

template<class t_type> void Circular_Array<t_type>::checkHead(){
  if(head >= array_length){
    head = 0;
  }
}

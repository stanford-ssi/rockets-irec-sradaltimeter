#ifndef _HERMES_H
#define _HERMES_H

#include <Arduino.h>
//This file, which is shared between Skybass and SRADIO, defines the protocol by which data is transmitted between them.
//its called hermes?

typedef struct __attribute__((__packed__)) hermes_data_t
{
  virtual size_t getSize();
} hermes_data_t;

typedef uint8_t packet_type_t;

class Hermes
{
public:
  Hermes(HardwareSerial serial);
  bool receiveSkybassData(hermes_data_t &data_struct);
  void sendSkybassData(hermes_data_t &data_struct);

private:
  void send(hermes_data_t &data_struct);
  void parse(hermes_data_t &data_struct);
  HardwareSerial serialPort;
  const uint32_t baud = 115200;
};

#endif
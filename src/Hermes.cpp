#include <Arduino.h>
#include "Hermes.h"
//the right way to do this is to register callbacks for the message handler, but it dosen't matter and for not im just gonna jank.

uint8_t RADIO_START_SEQUENCE[] = {204, 105, 119, 82};
uint8_t RADIO_END_SEQUENCE[] = {162, 98, 128, 161};

Hermes::Hermes(HardwareSerial serial)
{
    serialPort = serial;
    serialPort.begin(Hermes::baud);
}

void Hermes::send(hermes_data_t &data_struct)
{
    uint8_t data[data_struct.getSize()];
    memcpy(data, &data_struct, data_struct.getSize());
    serialPort.write(data, data_struct.getSize());
}

void Hermes::sendSkybassData(hermes_data_t &data_struct)
{
    serialPort.write(RADIO_START_SEQUENCE, 4);
    send(data_struct);
    serialPort.write(RADIO_END_SEQUENCE, 4);
}

bool Hermes::receiveSkybassData(hermes_data_t &data_struct)
{
    uint8_t last_bytes[4] = {0};
    while (serialPort.available() > 0)
    {
        uint8_t byte = serialPort.read();
        last_bytes[0] = last_bytes[1];
        last_bytes[1] = last_bytes[2];
        last_bytes[2] = last_bytes[3];
        last_bytes[3] = byte;
        if (*(uint32_t *)RADIO_START_SEQUENCE == *(uint32_t *)last_bytes)
        {
            delay(100); //let the message come in completely
            parse(data_struct);
            last_bytes[0] = serialPort.read();
            last_bytes[1] = serialPort.read();
            last_bytes[2] = serialPort.read();
            last_bytes[3] = serialPort.read();
            if (*(uint32_t *)RADIO_END_SEQUENCE != *(uint32_t *)last_bytes)
            {
                Serial.println("Skybass Packet Receive Error");
            }
            else
            {
                Serial.println("Received Skybass Packet");
                return true;
            }
        }
    }
    return false;
}

void Hermes::parse(hermes_data_t &data_struct)
{
    uint8_t data[data_struct.getSize()];
    for (uint8_t i = 0; i < data_struct.getSize(); i++)
        data[i] = serialPort.read();
    memcpy(&data_struct, &data, data_struct.getSize());
}
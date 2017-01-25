#include "MMA65XX_Sensor.h"
#include <SPI.h>

const MMA65XX_Sensor::Msg REGISTER_OFFSET = 8;
const MMA65XX_Sensor::Msg ACCEL_OFFSET0 = 14;
const double CONVERSION = 9.80665/18.2;

enum CommandMsg {
  CMD_REG_P_EVEN = 1 << 15,
  CMD_REG_P_ODD = 0 << 15,
  CMD_REG_P = 1 << 15,
  CMD_AX_X = 0 << 14,
  CMD_AX_Y = 1 << 14,
  CMD_AX = 1 << 14,
  CMD_REG_READ = 0 << 14,
  CMD_REG_WRITE = 1 << 14,
  CMD_A_REG_OP = 0 << 13,
  CMD_A_DATA_REQ = 1 << 13,
  CMD_A = 1 << 13,
  CMD_OC_OFFSET_CANCELLED = 0 << 12,
  CMD_OC_RAW_ACCEL = 1 << 12,
  CMD_OC = 1 << 12,
  CMD_NEEDED_BIT = 1 << 3,
  CMD_SD_SIGNED_DATA = 0 << 2,
  CMD_SD_UNSIGNED_DATA = 1 << 2,
  CMD_SD = 1 << 2,
  CMD_ARM_PCM = 0 << 1,
  CMD_ARM_ARMING = 1 << 1,
  CMD_ARM = 1 << 1,
  CMD_P_ODD = 0 << 0,
  CMD_P_EVEN = 1 << 0,
  CMD_P = 1 << 0
};

enum ResponseMsg {
  RES_AX_X = 0 << 13,
  RES_AX_Y = 1 << 13,
  RES_AX = 1 << 13,
  RES_P_ODD = 0 << 12,
  RES_P_EVEN = 1 << 12,
  RES_P = 1 << 12,
  RES_STAT_INIT = 0x00 << 10,
  RES_STAT_NORMAL = 0x01 << 10,
  RES_STAT_ST_ACTIVE = 0x02 << 10,
  RESR_STAT_ERROR = 0x03 << 10,
  RES_STAT = 0x03 << 10,
  RES_ACCEL_DATA0 = 0x02 << ACCEL_OFFSET0,
  RES_ACCEL_DATA1 = 0x3ff
};

enum Registers {
  DEVSTAT = 0x14 << REGISTER_OFFSET,
  DEVCFG = 0x0b << REGISTER_OFFSET
};

enum RegisterBits {
  BIT_OC = 0x80,
  BIT_ENDINIT = 0x20,
  BIT_SD = 0x10,
  BIT_OFMON = 0x08
};

MMA65XX_Sensor::MMA65XX_Sensor(int _ss_pin)
  : ss_pin(_ss_pin) {
  pinMode(ss_pin, OUTPUT);
  SPI.begin();
  //Ensure that device exits "initialization" phase.
  //Also make it give us unsigned data, because that's a bit easier to convert.
  transfer(CMD_REG_P_EVEN | CMD_REG_WRITE | DEVCFG | BIT_ENDINIT | BIT_SD);
  transfer(CMD_REG_P_EVEN | CMD_REG_READ | DEVSTAT);
}

bool MMA65XX_Sensor::getEvent(sensors_event_t* evt) {
  Msg x, y;
  transfer(CMD_AX_Y | CMD_A_DATA_REQ | CMD_OC_OFFSET_CANCELLED |
	   CMD_SD_UNSIGNED_DATA | CMD_ARM_PCM | CMD_P_EVEN | CMD_NEEDED_BIT);
  y = transfer(CMD_AX_X | CMD_A_DATA_REQ | CMD_OC_OFFSET_CANCELLED |
	       CMD_SD_UNSIGNED_DATA | CMD_ARM_PCM | CMD_P_ODD | CMD_NEEDED_BIT);
  x = transfer(0);
  evt->type = SENSOR_TYPE_ACCELEROMETER;
  evt->acceleration.x = getAccelData(x);
  evt->acceleration.y = getAccelData(y);
  return true;
}

MMA65XX_Sensor::Msg MMA65XX_Sensor::transfer(Msg data) {
  Msg cmd = data;
  Msg res;
  digitalWrite(ss_pin, LOW);
  res = (SPI.transfer(byte(cmd >> 8))) << 8;
  res |= SPI.transfer(byte(cmd));
  digitalWrite(ss_pin, HIGH);
  return res;
}

double MMA65XX_Sensor::getAccelData(Msg msg) {
  int16_t res = (msg & RES_ACCEL_DATA1) << 2;
  res |= (msg & RES_ACCEL_DATA0) >> ACCEL_OFFSET0;
  return double(res - 2048)*CONVERSION;
}

void MMA65XX_Sensor::printBin(Msg num) {
  for (Msg bin = 2; bin > 1; bin <<= 1) {
    if (num < bin) Serial.print(0);
  }
  Serial.println(num, BIN);
}

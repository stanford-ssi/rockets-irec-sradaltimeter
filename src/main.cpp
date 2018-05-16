
#include "Altimeter.h"
#include <stdio.h>
#include "hw_test.h"

#include <WProgram.h>
#include <Wire.h>
#include <ADC.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_BMP280.h>
#include <ADC.h>
#include <MMA65XX_Sensor.h>
void testHW();

//global objects
Altimeter  altimeter;
ArduinoOutStream cout(Serial);

//actual main
int main(void)
{
  //gps_test();
  //testHW();
  altimeter.startup();
  while(true){
    altimeter.manageEvents();
  }
  return 0;
}

void testHW(){
  uint8_t address;
  int8_t status;
  Serial.begin(115200);
  delay(3000);
  Wire.begin();
  while(1) {
    Serial.println("Scanning...");
    for (address = 1; address <= 127; address++) {
      Wire.beginTransmission(address);
      status = Wire.endTransmission();

      if (status == 0) {
        Serial.print("I2C device found at: 0x");
        Serial.println(address, HEX);
      }
    }
    Serial.println("done");
    delay(2000);
  }
  Adafruit_BMP280  bmp1(Adafruit_BMP280(BMP1_CS));
  Adafruit_BMP280  bmp2(Adafruit_BMP280(BMP2_CS));
  Adafruit_BNO055  bno(Adafruit_BNO055(55,0x28));
  MMA65XX_Sensor  mma(MMA65XX_Sensor(MMA_CS));
  QuickGPS  gps(&GPS_SERIAL) ;
  bmp1.begin();
  bmp2.begin();

  Serial.printf("%f, %f, %f, %f\n",bmp1.readPressure(),bmp1.readTemperature(),bmp2.readPressure(),bmp2.readTemperature());
  Serial.print("Done....");
  while(1){
    Serial.printf(".");
    delay(100);
  };
}

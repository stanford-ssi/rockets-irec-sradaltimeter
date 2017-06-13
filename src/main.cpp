
#include "Altimeter.h"
#include <stdio.h>

/* global object */
Altimeter  altimeter;
ArduinoOutStream cout(Serial);


//actual main
int main(void)
{
  altimeter.startup();
  while(true){
    altimeter.manageEvents();
  }
  return 0;
}

/*
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
Adafruit_BNO055 bno = Adafruit_BNO055(55);
int main(void){
  Serial.begin(115200);
  if(bno.begin()){
    Serial.println("it worked");
    while(1){
      sensors_event_t event;
      bno.getEvent(&event);
      //Display the floating point data
      Serial.print("X: ");
      Serial.println(event.orientation.x, 4);
    }
  }
  else Serial.println("nope");
}
*/



/*
//GPS testing
int main(void){
  Serial.begin(115200);
  Serial1.begin(9800);
  while(1){
    if(Serial1.available()){
      Serial.print((char)Serial1.read());
    }
  }
}
*/

/*
//I2C Scanner
int main(void){
  Wire.begin();

  Serial.begin(9600);
  delay(500);
  Serial.println("\nI2C Scanner");
  while(1){
    byte error, address;
    int nDevices;

    Serial.println("Scanning...");

    nDevices = 0;
    for(address = 1; address < 127; address++ )
    {
      // The i2c_scanner uses the return value of
      // the Write.endTransmisstion to see if
      // a device did acknowledge to the address.
      Wire.beginTransmission(address);
      error = Wire.endTransmission();

      if (error == 0)
      {
        Serial.print("I2C device found at address 0x");
        if (address<16)
          Serial.print("0");
        Serial.print(address,HEX);
        Serial.println("  !");

        nDevices++;
      }
      else if (error==4)
      {
        Serial.print("Unknown error at address 0x");
        if (address<16)
          Serial.print("0");
        Serial.println(address,HEX);
      }
    }
    if (nDevices == 0)
      Serial.println("No I2C devices found\n");
    else
      Serial.println("done\n");

    delay(5000);           // wait 5 seconds for next scan
  }
  return 0;
}
*/

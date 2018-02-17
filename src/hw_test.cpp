#include "hw_test.h"

void bno_test(void){
  Adafruit_BNO055 bno = Adafruit_BNO055(55);
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
  else{
    while(1){Serial.println("nope");}
  }
}


//GPS testing
void gps_test(void){
  Serial.begin(115200);
  Serial1.begin(9800);
  while(1){
    if(Serial1.available()){
      Serial.print((char)Serial1.read());
    }
  }
}



//I2C Scanner

void i2c_scan(void){
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
}

void mma_test(){
  MMA65XX_Sensor mma = MMA65XX_Sensor(MMA_CS);
  mma.begin();
  while(1){
    Mma_Data ret;
    sensors_event_t evt;
    mma.getEvent(&evt);
    ret.x = evt.acceleration.x;
    ret.y = -evt.acceleration.y;
    Serial.print(ret.x );
    Serial.print(',');
    Serial.println(ret.y);
    delay(100);
  }
}

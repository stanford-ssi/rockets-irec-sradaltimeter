
#include "Altimeter.h"

/* global object */
Altimeter  altimeter;
ArduinoOutStream cout(Serial);

int main(void)
{
  /*
  Serial3.begin(9600);
  delay(1000);
  while(1){
    Serial3.println("test");
  }
  */

  altimeter.startup();
  while(true){
    altimeter.manageEvents();
  }

}

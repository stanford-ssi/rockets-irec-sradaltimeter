
#include "Altimeter.h"

/* global object */
Altimeter  altimeter;
ArduinoOutStream cout(Serial);

int main(void)
{
  altimeter.startup();
  while(true){
    altimeter.manageEvents();
  }
}

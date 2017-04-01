
#include "Altimeter.h"
#include <stdio.h>

/* global object */
Altimeter  altimeter;
ArduinoOutStream cout(Serial);

int main(void)
{
  altimeter.startup();
  while(true){
    altimeter.manageEvents();
  }
  return 0;
}

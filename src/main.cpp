
#include "Altimeter.h"
#include <stdio.h>
#include "hw_test.h"

//global objects
Altimeter  altimeter;
ArduinoOutStream cout(Serial);


//actual main
int main(void)
{
  //gps_test();
  altimeter.startup();
  while(true){
    altimeter.manageEvents();
  }
  return 0;
}

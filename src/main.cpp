#include "Altimeter.h"

/* global object */
Altimeter  altimeter;

int main(void)
{
  altimeter.startup();
  while(true){
    altimeter.manageEvents();
  }
}

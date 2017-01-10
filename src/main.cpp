#include <WProgram.h>
#include <stdint.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_BMP280.h>
#include "Flight_Configuration.h"
#include "Salt_Rev0.h"
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

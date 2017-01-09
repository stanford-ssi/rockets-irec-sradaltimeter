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

/* global variables */
uint16_t flight_state = STRTUP;
volatile uint8_t events;
int led_counter = 0;

/* global functions */
void startup();
void mainUpdate();

int main(void)
{
	startup();
	while(true){
		if(altimeter.flight_events.check(EVENT_MAIN)){
			mainUpdate();
		}

		if(altimeter.flight_events.check(EVENT_READ_BMP)){
			altimeter.flight_data.updatePressure(altimeter.flight_sensors.readPressure());
		}
	}
}

/* Global Function definitions */

/* function: startup
 * First function that runs, initializes everything.
 * Sets flightmode to IDLE upon completion
 */
void startup(){
	/* gpio */
	pinMode(LED_1, OUTPUT);
	pinMode(LED_2, OUTPUT);
	pinMode(LED_3, OUTPUT);
	pinMode(LED_4, OUTPUT);
	pinMode(BUZZER, OUTPUT);

	pinMode(TRIG_1, OUTPUT);
	pinMode(TRIG_2, OUTPUT);
	pinMode(TRIG_3, OUTPUT);
	pinMode(TRIG_4, OUTPUT);

	altimeter.flight_sensors.initialize();
	//
	Serial.begin(9600);
	flight_state = IDLE;
	altimeter.flight_events.initialize();
}

void mainUpdate(){
	switch(flight_state){
		case IDLE:
			digitalWrite(LED_1, led_counter == 0);
			digitalWrite(LED_2, led_counter == 1);
			digitalWrite(LED_3, led_counter == 2);
			digitalWrite(LED_4, led_counter == 3);
			led_counter++;
			if(led_counter > 3){led_counter = 0;}
			break;
		case ARMED:
			break;
		}
}

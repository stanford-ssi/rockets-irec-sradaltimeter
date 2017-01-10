#include "Altimeter.h"

void Altimeter::manageEvents(){
  if(flight_events.check(EVENT_MAIN)){
    mainUpdate();

  }
  if(flight_events.check(EVENT_READ_BMP)){
    flight_data.updatePressure(flight_sensors.readPressure());
  }

}

void Altimeter::startup(){
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, HIGH);
	/* gpio */
	pinMode(LED_1, OUTPUT);
	pinMode(LED_2, OUTPUT);
	pinMode(LED_3, OUTPUT);
	pinMode(LED_4, OUTPUT);

	pinMode(TRIG_1, OUTPUT);
	pinMode(TRIG_2, OUTPUT);
	pinMode(TRIG_3, OUTPUT);
	pinMode(TRIG_4, OUTPUT);
  Serial.begin(9600);
  SD.begin(SD_CS);
	flight_sensors.initialize();
	flight_events.initialize();

  digitalWrite(BUZZER, LOW);
  flight_state = IDLE;
}

void Altimeter::mainUpdate(){
  switch(flight_state){
    case IDLE:
      manageLEDs();
      manageBuzzer();
    case ARMED:
      break;
    }
}

void Altimeter::manageLEDs(){
  digitalWrite(LED_1, led_counter == 0);
  digitalWrite(LED_2, led_counter == 1);
  digitalWrite(LED_3, led_counter == 2);
  digitalWrite(LED_4, led_counter == 3);
  led_counter++;
  if(led_counter > 3){led_counter = 0;}
}
void Altimeter::manageBuzzer(){

  if(buzzer_freq_scaler == 0){
    digitalWrite(BUZZER, (buzzer_counter == 0 && flight_data.getESense()[0]));
    digitalWrite(BUZZER, (buzzer_counter == 2 && flight_data.getESense()[0]));
    digitalWrite(BUZZER, (buzzer_counter == 4 && flight_data.getESense()[0]));
    digitalWrite(BUZZER, (buzzer_counter == 6 && flight_data.getESense()[0]));
    buzzer_counter++;
    if(buzzer_counter > 10){led_counter = 0;}
  }
  buzzer_freq_scaler++;
  if(buzzer_freq_scaler > 8){led_counter = 0;}

}

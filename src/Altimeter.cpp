#include "Altimeter.h"

/*
This is the only function that runs in the while(1) loop in main.cpp. It simply
checks the events and executes them
*/
void Altimeter::manageEvents(){

  if(flight_events.check(EVENT_MAIN)){
    mainUpdate();

  }
  if(flight_events.check(EVENT_READ_BMP)){
    //flight_data.updateBMP(sitl.readBMP());
  }
}

/*
The is the only function called before the while loop in main.cpp. It sets up
and initializes everything.
*/
void Altimeter::startup(){
  pinMode(BUZZER, OUTPUT);
  analogWriteFrequency(BUZZER, BUZZ_TONE_MID);
  analogWrite(BUZZER, 128);
  Serial.begin(9600);
  delay(1000);
	/* gpio */
	pinMode(LED_1, OUTPUT);
	pinMode(LED_2, OUTPUT);
	pinMode(LED_3, OUTPUT);
	pinMode(LED_4, OUTPUT);

	pinMode(TRIG_1, OUTPUT);
	pinMode(TRIG_2, OUTPUT);
	pinMode(TRIG_3, OUTPUT);
	pinMode(TRIG_4, OUTPUT);
  SD.begin(SD_CS);

  #ifdef SITL
  sitl.initialize();
  #else
	flight_sensors.initialize();
  #endif

	flight_events.initialize();
  delay(1000);
  analogWrite(BUZZER, 256);
  flight_state = IDLE;
}

/*
This is the main update function. It contains the switch case statement to execut
differently depending on the vehicle state.
*/
void Altimeter::mainUpdate(){
  switch(flight_state){
    case IDLE:
      manageLEDs();
      manageBuzzer();
      flight_data.writeBuffers();
      flight_data.printBuffers();
      //flight_data.updateBMP(sitl.readBMP());
      //flight_data.updateESense(flight_sensors.readESense());
      break;
    case ARMED:
      break;
    }
}

/*
This function manages the LED's of the altimeter. will be expanded in the future,
maybe made into a full object. right now it simple incrementally blinks the 4 LEDS
*/
void Altimeter::manageLEDs(){
  digitalWrite(LED_1, led_counter == 0);
  digitalWrite(LED_2, led_counter == 1);
  digitalWrite(LED_3, led_counter == 2);
  digitalWrite(LED_4, led_counter == 3);
  led_counter++;
  if(led_counter > 3){led_counter = 0;}
}

/*
This function manages the buzzer. also will be expanded, currently just buzzes
four times, low or high depending on if an ematch is connected or not. Also may
be converted into an object at some point.
*/
void Altimeter::manageBuzzer(){
  if(buzzer_freq_scaler == 0){
    switch(buzzer_counter){
      case 0:
        buzzInidicate(flight_data.getESense()&(1<<7));
        break;
      case 2:
        buzzInidicate(flight_data.getESense()&(1<<6));
        break;
      case 4:
        buzzInidicate(flight_data.getESense()&(1<<5));
        break;
      case 6:
        buzzInidicate(flight_data.getESense()&(1<<4));
        break;
        default:
          buzzOff();
        break;
    }
    buzzer_counter++;
    if(buzzer_counter > 30){buzzer_counter = 0;}
  }
  buzzer_freq_scaler++;
  if(buzzer_freq_scaler >= UPDATE_FREQ_HZ/BEEP_FREQ_HZ){buzzer_freq_scaler = 0;}
}

/*
This function turns on the buzzer, either high or low depending on the
bool that it is passed.
*/
void Altimeter::buzzInidicate(bool buzz){
  if(buzz){
    analogWriteFrequency(BUZZER, BUZZ_TONE_HIGH);
  } else {
    analogWriteFrequency(BUZZER, BUZZ_TONE_MID);
  }
  analogWrite(BUZZER, 128);
}

/*
Really simple function that just turns off the buzzer. Mostly just made it it's
own function cause I like the name.
*/
void Altimeter::buzzOff(){
  analogWrite(BUZZER,256);
}

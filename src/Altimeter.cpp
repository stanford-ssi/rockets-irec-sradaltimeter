#include "Altimeter.h"
#include "Logger.h"

long kk = 0;
long qq = 0;

/*
This is the only function that runs in the while(1) loop in main.cpp. It simply
checks the events and executes them
*/
void Altimeter::manageEvents(){

  if(flight_events.check(EVENT_MAIN)){
    mainUpdate();
    logger.log();
  }
  if(flight_events.check(EVENT_READ_BMP)){
    Event_Data ev = {kk};
    logger.log_variable(LOG_EVENT, &ev);
    kk++;
    /*Bmp_Data* data = flight_sensors.readPressure();
    Serial.println(data->pressure1);
    Serial.println(data->pressure2);
    delete data;
    Serial.println("-");
      Mma_Data* data2 = flight_sensors.readAcceleration();
      Serial.println(data2->x);
      Serial.println(data2->y);
      delete data2;
      Serial.println("--");*/
    //flight_data.updateBMP(sitl.readBMP());
  }
  if(flight_events.check(EVENT_READ_MMA)){
    Mma_Data da = {qq/2., qq*2.};
    logger.log_variable(LOG_MMA, &da);
    qq++;
  }
}

/*
The is the only function called before the while loop in main.cpp. It sets up
and initializes everything.
*/
void Altimeter::startup(){
  pinMode(BUZZER, OUTPUT);
  analogWriteFrequency(BUZZER, BUZZ_TONE_MID);
  analogWrite(BUZZER, 256);
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
  flight_data.initialize();
  #ifdef SITL
    sitl.initialize();
  #else
	  flight_sensors.initialize();
  #endif

	flight_events.initialize();
  delay(1000);
  analogWrite(BUZZER, 256);
  flight_state = IDLE;

  /* Pessimistic approximation of the number of bytes:
   * (100 Hz)*(5 sensors)*(16 byte/sensor)*(3 hour) = 86400000 bytes.
   * Incidentally, that's the number of milliseconds in a day
   * without taking into account sidereal periods or leap seconds. */
  if (logger.initialize(86400000)) {
    Serial.println("Logging initialized successfully");
  }
  logger.init_variable(LOG_BMP, "bmp", sizeof(Bno_Data));
  logger.init_variable(LOG_MMA, "mma", sizeof(Mma_Data));
  logger.init_variable(LOG_BNO, "bno", sizeof(Bno_Data));
  logger.init_variable(LOG_EVENT, "event", sizeof(Event_Data));

  logger.finish_headers();
}

/*
This is the main update function. It contains the switch case statement to execut
differently depending on the vehicle state.
*/
void Altimeter::mainUpdate(){
  switch(flight_state){
    case IDLE:
      manageLEDs();
      //manageBuzzer();
      flight_data.writeBuffers();
      flight_data.printBuffers();
      flight_sensors.update();
      //flight_data.updateBMP(sitl.readBMP());
      flight_data.updateESense(flight_sensors.readESense());
      break;
    case ARMED:
      manageLEDs();
      break;
    case PWRD_FLGHT:
      break;
    case COAST:
      break;
    case DESCNT_DROG:
      break;
    case DESCNT_MAIN:
      break;
    case PRE_RCVRY:
      break;
    case RCVRED:
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

#include <SPI.h>
#include "Altimeter.h"
#include "Logger.h"


/*
  This is the only function that runs in the while(1) loop in main.cpp. It simply
  checks the events and executes them
*/
void Altimeter::manageEvents(){
  if(flight_events.check(EVENT_MAIN)){
    flight_events.processor_busy = true;
    mainUpdate();
    logger.log();
    manageLEDs();

    //transmit data over xbee
    /*
    String xbee_tx =
      String(flight_state) + ";" +
      //flight_data.getGPSdata().lon + "," ;
      //flight_data.getGPSdata().lat + ';' +
      //flight_data.getBMPdata().pressure1 + ',' +
      //flight_data.getBMPdata().pressure2 + ';' ;
      //flight_data.getMMAdata().x + ',' +
      //flight_data.getMMAdata().y + ';' ;
      //flight_data.getESense() + ';' +
      //flight_data.getIsoSense() + ';'
    */
    Serial.println("test message");
    Serial3.print(flight_state);
    Serial3.print("; ");
    Serial3.print(flight_data.getGPSdata().lon);
    Serial3.print(", ");
    Serial3.print(flight_data.getGPSdata().lat);
    Serial3.print("; ");
    Serial3.print(flight_data.getBMPdata().pressure2);
    Serial3.print("; ");
    Serial3.print(flight_data.getGlobaltime());
    Serial3.print("; ");
    Serial3.println("test message");

  }
  if(flight_events.check(EVENT_READ_BMP)){
    flight_events.processor_busy = true;
    Bmp_Data bmp_data = flight_sensors.readBMP();
    flight_data.updateBMP(bmp_data);
    logger.log_variable(LOG_BMP, &bmp_data);
  }

  if(flight_events.check(EVENT_READ_MMA)){
    flight_events.processor_busy = true;
    Mma_Data mma_data = flight_sensors.readMMA();
    flight_data.updateMMA(mma_data);
    logger.log_variable(LOG_MMA, &mma_data);
  }

  if(flight_events.check(EVENT_READ_GPS)){
    flight_events.processor_busy = true;
    Gps_Data gps_data = flight_sensors.readGPS();
    flight_data.updateGPS(gps_data);
    logger.log_variable(LOG_GPS, &gps_data);
  }

  if(flight_events.check(EVENT_BUZZER)){
    flight_events.processor_busy = true;
    //manageBuzzer();
  }
  flight_events.processor_busy = false;
}

/*
  The is the only function called before the while loop in main.cpp. It sets up
  and initializes everything.
*/
void Altimeter::startup(){
  pinMode(BUZZER, OUTPUT);
  analogWriteFrequency(BUZZER, BUZZ_TONE_MID);
  analogWrite(BUZZER, 128);
  Serial.begin(115200);   //usb Serial
  Serial3.begin(9600);  //xbee Serial

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
  flight_state = IDLE;

  /* Pessimistic approximation of the number of bytes:
   * (100 Hz)*(5 sensors)*(16 byte/sensor)*(3 hour) = 86400000 bytes.
   * Incidentally, that's the number of milliseconds in a day
   * without taking into account sidereal periods or leap seconds. */
  if (logger.initialize(86400000)) {
    Serial.println("Logging initialized successfully");
  }
  logger.init_variable(LOG_BMP, "bmp", sizeof(Bmp_Data));
  logger.init_variable(LOG_MMA, "mma", sizeof(Mma_Data));
  logger.init_variable(LOG_BNO, "bno", sizeof(Bno_Data));
  logger.init_variable(LOG_EVENT, "event", sizeof(Event_Data));
  logger.finish_headers();
  buzzOff();
}

/*
  This is the main update function. It contains the switch case statement to execut
  differently depending on the vehicle state.
*/
void Altimeter::mainUpdate(){
  switch(flight_state){
    case IDLE:
      break;
    case ARMED:
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
  if(buzzer_freq_scaler >= BUZZER_FREQ/BEEP_FREQ_HZ){buzzer_freq_scaler = 0;}
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

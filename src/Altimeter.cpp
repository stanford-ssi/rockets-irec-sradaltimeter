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

  if(flight_events.check(EVENT_FILTER)){
    flight_events.processor_busy = true;
    alt_filter.update(flight_data.getBMPdata(), flight_data.getBNOdata(), flight_data.getMMAdata());
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
  xbeeSerial.begin(115200);  //xbee Serial
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
  digitalWrite(LED_1, true);
  digitalWrite(LED_2, true);
  digitalWrite(LED_3, true);
  digitalWrite(LED_4, true);
  delay(1000);
  /* gpio */

  pinMode(TRIG_1, OUTPUT);
  pinMode(TRIG_2, OUTPUT);
  pinMode(TRIG_3, OUTPUT);
  pinMode(TRIG_4, OUTPUT);
  flight_data.initialize();
  flight_sensors.initialize();

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
  flight_events.initialize();

  xbeeSerial.println("Unit Initialized");
  //Serial.println("Unit Initialized");
  buzzOff();
}

/*
  This is the main update function. It contains the switch case statement to execut
  differently depending on the vehicle state.
*/
void Altimeter::mainUpdate(){
  flight_sensors.update();
  logger.log();
  manageLEDs();
  temp_counter++;
  if(temp_counter == 100){
    temp_counter = 0;
    transmitXbee();
  }
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


void Altimeter::transmitXbee(){

  long gtime = flight_data.getGlobaltime();
  Bmp_Data bmp = flight_data.getBMPdata();
  Mma_Data mma = flight_data.getMMAdata();
  Bno_Data bno = flight_data.getBNOdata();
  Gps_Data gps = flight_data.getGPSdata();
  xbee_buf[0] = TX_START;
  xbee_buf[1] = XBEE_BUF_LENGTH;
  uint8_t *msg_ptr = xbee_buf + 2;
  memcpy(msg_ptr, &gtime, sizeof(gtime));
  msg_ptr += sizeof(gtime);
  memcpy(msg_ptr, &bmp, sizeof(bmp));
  msg_ptr += sizeof(bmp);
  memcpy(msg_ptr, &mma, sizeof(mma));
  msg_ptr += sizeof(mma);
  memcpy(msg_ptr, &bno, sizeof(bno));
  msg_ptr += sizeof(bno);
  memcpy(msg_ptr, &gps, sizeof(gps));
  msg_ptr += sizeof(gps);
  uint8_t checksum = 0;
  for(int i = 0; i < XBEE_BUF_LENGTH; i++){
    checksum = xbee_buf[i] ^ checksum;
  }
  *msg_ptr = checksum;
  msg_ptr++;
  *msg_ptr = TX_END;
  xbeeSerial.write(xbee_buf, XBEE_BUF_LENGTH);
  Serial.write(xbee_buf, XBEE_BUF_LENGTH);

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

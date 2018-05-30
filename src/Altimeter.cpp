#include <SPI.h>
#include "Altimeter.h"
#include "Logger.h"
#include "Flight_Configuration.h"
#include "Flight_Data.h"
#include "utils.h"

/*
  This is the only function that runs in the while(1) loop in main.cpp. It simply
  checks the events and executes them
*/
void Altimeter::manageEvents(){
  #ifdef _DEBUG_
    elapsedMicros timer;
  #endif
  for(int i = 0; i < NUM_EVENTS; i++){
    event_t event = (event_t)(1<<i);
    if(flight_events.check(event)){
      flight_events.processor_busy = event;
      #ifdef _DEBUG_
        Serial.printf("%d,",flight_events.processor_busy);
        timer = 0;
      #endif
      eventHandle(event);
      #ifdef _DEBUG_
        Serial.println(timer);
      #endif
    }
  }
  flight_events.processor_busy = 0;
}

void Altimeter::eventHandle(event_t event){
  if(event == EVENT_MAIN){
    mainUpdate();
    return;
  }
  if(event == EVENT_READ_BNO){
    Bno_Data bno_data = flight_sensors.readBNO();
    flight_data.updateBNO(bno_data);
    logger.log_variable(LOG_BNO, &bno_data, flight_data.getGlobaltime());
    return;
  }
  if(event == EVENT_READ_BMP){
    Bmp_Data bmp_data = flight_sensors.readBMP();
    flight_data.updateBMP(bmp_data);
    logger.log_variable(LOG_BMP, &bmp_data, flight_data.getGlobaltime());
    return;
  }
  if(event == EVENT_READ_MMA){
    Mma_Data mma_data = flight_sensors.readMMA();
    flight_data.updateMMA(mma_data);
    logger.log_variable(LOG_MMA, &mma_data, flight_data.getGlobaltime());
    return;
  }
  if(event == EVENT_READ_GPS){
    Gps_Data gps_data = flight_sensors.readGPS();
    flight_data.updateGPS(gps_data);
    logger.log_variable(LOG_GPS, &gps_data, flight_data.getGlobaltime());
    return;
  }
  if(event == EVENT_BUZZER){
    manageBuzzer();
    return;
  }
  if(event == EVENT_FILTER){
    //alt_filter.update(flight_data.getBMPdata(), flight_data.getBNOdata(), flight_data.getMMAdata());
    return;
  }
}

/*
  The is the only function called before the while loop in main.cpp. It sets up
  and initializes everything.
*/
void Altimeter::startup(){
  pinMode(BUZZER, OUTPUT);
  buzzOff();
  buzzInidicate(false);
  Serial.begin(115200);   //usb Serial
  xbeeSerial.begin(19200);  //xbee Serial
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
  digitalWrite(LED_1, true);
  digitalWrite(LED_2, true);
  digitalWrite(LED_3, true);
  digitalWrite(LED_4, true);
  delay(500);
  buzzOff();

  /* gpio */
  pinMode(TRIG_1, OUTPUT);
  pinMode(TRIG_2, OUTPUT);
  pinMode(TRIG_3, OUTPUT);
  pinMode(TRIG_4, OUTPUT);
  flight_data.initialize();
  if(flight_sensors.initialize()){
    Serial.println("Sensors initialized successfully");
  } else {
    Serial.println("Sensors not initialized successfully");
  }

  if (logger.initialize(1024 * 1024 * 100)) { // 100MB
    Serial.println("Logging initialized successfully");
  }
  logger.init_variable(LOG_BMP, "bmp", sizeof(Bmp_Data));
  logger.init_variable(LOG_MMA, "mma", sizeof(Mma_Data));
  logger.init_variable(LOG_BNO, "bno", sizeof(Bno_Data));
  logger.init_variable(LOG_GPS, "gps", sizeof(Gps_Data));
  logger.init_variable(LOG_EVENT, "event", sizeof(Event_Data));
  logger.init_variable(LOG_VBAT, "vbat", sizeof(float));
  logger.finish_headers();
  flight_events.initialize();

  xbeeSerial.println("Unit Initialized");
  Serial.println("Unit Initialized");
  buzzInidicate(true);
  delay(100);
  buzzOff();

  flight_state = IDLE;
}

/*
  This is the main update function. It contains the switch case statement to execut
  differently depending on the vehicle state.
*/
void Altimeter::mainUpdate(){

  // Cutdown!
  while (xbeeSerial.available() > 0) {
      char in = xbeeSerial.read();
      if (in == 'a') {
          while (true) {
              xbeeSerial.println(":skeleton: bye bye");
              Serial.println(":skeleton: bye bye");
              digitalWrite(TRIG_1, true);
              digitalWrite(TRIG_2, true);
              digitalWrite(TRIG_3, true);
              digitalWrite(TRIG_4, true);
          }
      }
  }

  flight_sensors.update();
  float vbat = flight_sensors.readVbat();
  logger.log_variable(LOG_VBAT, &vbat, flight_data.getGlobaltime());
  logger.log();
  manageLEDs();


  transmit_counter++;
  if((transmit_counter == 10)||(flight_state == ARMED)||(flight_state == PWRD_FLGHT)){
    transmit_counter = 0;
    setXbeeBuffer();
  }

  /*
  Bno_Data bno = flight_data.getBNOdata();
  Serial.print(", x: ");
  Serial.print(bno.euler.x);
  Serial.print(", y: ");
  Serial.print(bno.euler.y);
  Serial.print(", z: ");
  Serial.println(bno.euler.z);
  */

  Gps_Data g = flight_data.getGPSdata();
  if(g.lock) digitalWrite(LED_2, true);
  else digitalWrite(LED_2, false);

  if(flight_state == ARMED) digitalWrite(LED_1, true);
  else digitalWrite(LED_1, false);


  switch(flight_state){
    case IDLE:
      if(checkOnRail()){
        flight_state = ARMED;
        flight_data.launchpad_alt = flight_data.getBMPalt();
      }
      break;
    case ARMED:
      if(!checkOnRail())flight_state = IDLE;
      //if(checkLiftoff())flight_state = PWRD_FLGHT;
      break;
    case PWRD_FLGHT:
      //if(checkLowAlt())flight_state = RCVRED;
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


bool Altimeter::setXbeeBuffer(){
  if(xbee_buf_head != XBEE_BUF_LENGTH){
    return false;
  }
  long gtime = flight_data.getGlobaltime();
  Bmp_Data bmp = flight_data.getBMPdata();
  Mma_Data mma = flight_data.getMMAdata();
  Bno_Data bno = flight_data.getBNOdata();
  Gps_Data gps = flight_data.getGPSdata();
  float vbat = flight_sensors.readVbat();
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
  memcpy(msg_ptr, &vbat, sizeof(vbat));
  msg_ptr += sizeof(vbat);
  //for(int i = 2; i < XBEE_BUF_LENGTH-2; i++){
  //  xbee_buf[i] = 0;
  //}
  uint8_t checksum = 0;
  for(int i = 0; i < XBEE_BUF_LENGTH-2; i++){
    checksum = xbee_buf[i] ^ checksum;
  }
  *msg_ptr = checksum;
  //Serial.println(msg_ptr - xbee_buf);
  msg_ptr++;
  *msg_ptr = TX_END;
  xbee_buf_head = 0;
}

void Altimeter::transmitXbee(){
  int avail = xbeeSerial.availableForWrite();
  for(int i = 0; i < avail; i++){
    if(xbee_buf_head == XBEE_BUF_LENGTH){
      break;
    }
    xbeeSerial.write(xbee_buf[xbee_buf_head]);
    xbee_buf_head++;
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
      buzzInidicate(false);
      break;
    case 2:
      buzzInidicate(false);
      break;
    case 6:
      buzzInidicate(flight_state == ARMED);
      break;
    case 8:
      buzzInidicate(flight_data.getGPSdata().lock);
      break;
    default:
      buzzOff();
      break;
    }
    buzzer_counter++;
    if(buzzer_counter > 100)buzzer_counter = 0;
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
  analogWrite(BUZZER,BUZZER_OFF);
}

/***** Transition checking functions ******/

bool Altimeter::checkOnRail(){
  int len = flight_data.bno_array.array_length;
  Bno_Data data[len];
  Average<float> y_angle(len);
  flight_data.bno_array.getArray(data,(int)len);
  for(int i = 0; i<len;i++){
    y_angle.push(data[i].euler.y);
  }
  float std = y_angle.stddev();
  float avg = y_angle.mean();
  //Serial.print("std: ");
  //Serial.print(std);
  //Serial.print(", mean: ");
  //Serial.println(avg);
  if(flight_state == IDLE){
    return ((std < LAUNCH_ANGLE_STD_MAX)&&(avg > LAUNCH_ANGLE_AVG_MIN));
  }
  if(flight_state == ARMED){
    return (avg > LAUNCH_ANGLE_AVG_MIN);
  }
  return false;
}

bool Altimeter::checkLiftoff(){
  int len = flight_data.mma_array.store_freq; //this will get one second of the array
  Mma_Data mma[len];
  flight_data.mma_array.getArray(mma, len);
  int dv = 0;
  for(int i = 0; i<len;i++){
    dv += i;
  }
  if(dv/len > LIFTOFF_DV_MIN) liftoff_accel = true;
  return (liftoff_accel && ((flight_data.getBMPalt()-flight_data.launchpad_alt) > LIFTOFF_ALT_MIN));
}

bool Altimeter::checkLowAlt(){
  int len = flight_data.bmp_array.array_length;
  Bmp_Data data[len];
  Average<float> alt(len);
  flight_data.bmp_array.getArray(data,len);
  for(int i = 0;i<len;i++){
    float a = p2alt((data[i].pressure1 + data[i].pressure2)/2);
    alt.push(a);
  }
  float avg_alt = alt.mean() - flight_data.launchpad_alt;
  Serial.println(avg_alt);
  return avg_alt < RECOVERY_MAX_ALT;
}

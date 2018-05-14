#include <SPI.h>
#include "Altimeter.h"
#include "Logger.h"
#include "Flight_Configuration.h"
#include "Flight_Data.h"
#include "Utils.h"
//#include "min-irec.h"
//#include "min_support.h"
#include "min.c"
#include "min.h"

skyb_data_t skyb_data;
skyb_cmd_t skyb_cmd;

esp_status_t esp_status;
skyb_data_t sb_data;
bool new_esp_status = false;
bool new_sb_data = false;

struct min_context min_ctx_sradio;
struct min_context min_ctx_esp;
uint32_t timer;

void process_commands();
void pollSerialtoMIN();
/*
  This is the only function that runs in the while(1) loop in main.cpp. It simply
  checks the events and executes them
*/
void Altimeter::manageEvents()
{
#ifdef _DEBUG_
  elapsedMicros timer;
#endif
  for (int i = 0; i < NUM_EVENTS; i++)
  {
    event_t event = (event_t)(1 << i);
    if (flight_events.check(event))
    {
      flight_events.processor_busy = event;
#ifdef _DEBUG_
      Serial.printf("%d,", flight_events.processor_busy);
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

void Altimeter::eventHandle(event_t event)
{
  if (event == EVENT_MAIN)
  {
    mainUpdate();
    return;
  }
  if (event == EVENT_READ_BNO)
  {
    Bno_Data bno_data = flight_sensors.readBNO();
    //Bno_Data bno_data;
    flight_data.updateBNO(bno_data);
#ifdef _LOG_
    logger.log_variable(LOG_BNO, &bno_data, flight_data.getGlobaltime());
#endif
    return;
  }
  if (event == EVENT_READ_BMP)
  {
    Bmp_Data bmp_data = flight_sensors.readBMP();
    //Serial.printf("%f,%f,%f,%f\n",bmp_data.pressure1,bmp_data.temp1,bmp_data.pressure2,bmp_data.temp2);
    flight_data.updateBMP(bmp_data);
#ifdef _LOG_
    logger.log_variable(LOG_BMP, &bmp_data, flight_data.getGlobaltime());
#endif
    return;
  }
  if (event == EVENT_READ_MMA)
  {
    Mma_Data mma_data = flight_sensors.readMMA();
    flight_data.updateMMA(mma_data);
#ifdef _LOG_
    logger.log_variable(LOG_MMA, &mma_data, flight_data.getGlobaltime());
#endif
    return;
  }
  if (event == EVENT_READ_GPS)
  {
    Gps_Data gps_data = flight_sensors.readGPS();
    flight_data.updateGPS(gps_data);
#ifdef _LOG_
    logger.log_variable(LOG_GPS, &gps_data, flight_data.getGlobaltime());
#endif
    return;
  }
  if (event == EVENT_BUZZER)
  {
    if (flight_state < FLIGHT)
      manageBuzzer();
    else
      buzzOff();
    return;
  }
  if (event == EVENT_FILTER)
  {
    alt_filter.update(flight_data.getBMPdata(), flight_data.getBNOdata(), flight_data.getMMAdata());
    return;
  }
}

/*
  The is the only function called before the while loop in main.cpp. It sets up
  and initializes everything.
*/
void Altimeter::startup()
{
  pinMode(BUZZER, OUTPUT);
  buzzOff();
  buzzInidicate(false);
  DEBUG_SERIAL.begin(115200); //usb Serial
  ESP_SERIAL.begin(115200);   //Serial to ESP
  RF_SERIAL.begin(115200);    //Serial to Sradio

  min_init_context(&min_ctx_esp, MIN_ESP);   //Context 2, Serial2, ESP_SERIAL
  min_init_context(&min_ctx_sradio, MIN_RF); //Context 3, Serial3, RF_SERIAL

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
  if (flight_sensors.initialize())
  {
    Serial.println("Sensors initialized successfully");
  }
  else
  {
    Serial.println("Sensors not initialized successfully");
  }
#ifdef _LOG_
  /* Pessimistic approximation of the number of bytes:
   * (100 Hz)*(5 sensors)*(16 byte/sensor)*(3 hour) = 86400000 bytes.
   * Incidentally, that's the number of milliseconds in a day
   * without taking into account sidereal periods or leap seconds. */
  if (logger.initialize(86400000))
  {
    Serial.println("Logging initialized successfully");
  }
  logger.init_variable(LOG_BMP, "bmp", sizeof(Bmp_Data));
  logger.init_variable(LOG_MMA, "mma", sizeof(Mma_Data));
  logger.init_variable(LOG_BNO, "bno", sizeof(Bno_Data));
  logger.init_variable(LOG_GPS, "gps", sizeof(Gps_Data));
  logger.init_variable(LOG_EVENT, "event", sizeof(Event_Data));
  logger.init_variable(LOG_VBAT, "vbat", sizeof(float));
  logger.finish_headers();
#endif
  flight_events.initialize();
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
void Altimeter::mainUpdate()
{
  flight_sensors.update();
//logger.log_variable(LOG_VBAT, &vbat, flight_data.getGlobaltime());
#ifdef _LOG_
  logger.log();
#endif
  manageLEDs();
  pollSerialtoMIN();
  transmit_counter++;
  if ((transmit_counter == 4) || (flight_state == ARMED) || (flight_state == FLIGHT))
  {
    transmit_counter = 0;
    //jank
    long gtime = flight_data.getGlobaltime();
    Bmp_Data bmp = flight_data.getBMPdata();
    Mma_Data mma = flight_data.getMMAdata();
    Bno_Data bno = flight_data.getBNOdata();
    Gps_Data gps = flight_data.getGPSdata();
    float vbat = flight_sensors.readVbat();

    skyb_data.packet_num = (flight_data.getGlobaltime() / 1000) % 262143; //number of main loop cycles, about 24 hours @ 3hz
    skyb_data.altitude = flight_data.getBMPalt();                         //meters
    skyb_data.state = flight_state;                                       //4 bits, of skybass state and error
    skyb_data.batt_voltage = vbat;                                        //in volts
    skyb_data.latitude = gps.lat;
    skyb_data.longitude = gps.lon;
    skyb_data.gps_locked = gps.lock;

    min_send_frame(&min_ctx_sradio, SKYB_DATA, (uint8_t *)&skyb_data, sizeof(skyb_data));
  }

  Serial.printf("h:%f v:%f rh:%f bh:%f t:%f,%f, s:%d\n", alt_filter.getAltitude(), alt_filter.getVelocity(), flight_data.getBMPalt(), flight_data.biquad_alt, float(flight_data.flight_time) / 1000000, float(flight_data.appo_time) / 1000000, flight_state);
  byte sense = flight_sensors.readESense();
  flight_data.updateESense(sense);
  if ((sense & (1 << 0)) && (sense & (1 << 1)) && (sense & (1 << 2)))
    digitalWrite(LED_3, true);
  else
    digitalWrite(LED_3, false);

  Gps_Data g = flight_data.getGPSdata();
  if (g.lock)
    digitalWrite(LED_2, true);
  else
    digitalWrite(LED_2, false);

  if (flight_state == ARMED)
    digitalWrite(LED_1, true);
  else
    digitalWrite(LED_1, false);

  manageEmatches();

  switch (flight_state)
  {
  case IDLE:
    if (checkOnRail())
    {
      flight_state = ARMED;
      flight_data.launchpad_alt = flight_data.biquad_alt;
    }
    break;
  case ARMED:
    if (!checkOnRail())
      flight_state = IDLE;
    if (checkLiftoff())
    {
      flight_state = FLIGHT;
      flight_data.flight_time = 0;
    }
    break;
  case FLIGHT:
    if (flight_data.flight_time > MIN_FLIGHT_TIME * 1000000)
    {
      if (alt_filter.getVelocity() < 10 * RECOVERY_SEP_TIME)
      {
        recoveryEvent(SEP);
      }
      if (alt_filter.getVelocity() < 0)
      {
        flight_state = DESCNT;
        flight_data.appo_time = flight_data.flight_time;
      }
    }
    break;
  case DESCNT:

    if (flight_data.flight_time - flight_data.appo_time > RECOVERY_DROGUE_TIME * 1000000)
    {
      recoveryEvent(DROGUE);
    }
    if (flight_data.flight_time - flight_data.appo_time > RECOVERY_SEP_BACKUP_TIME * 1000000)
    {
      recoveryEvent(SEP_BACKUP);
    }
    if ((flight_data.biquad_alt - flight_data.launchpad_alt < RECOVERY_MAIN_ALT) && (flight_data.flight_time - flight_data.appo_time > RECOVERY_MAIN_MIN_DELAY * 1000000))
    {
      recoveryEvent(MAIN);
    }
    break;
  case RCVRED:
    break;
  }
}

/*
  This function manages the LED's of the altimeter. will be expanded in the future,
  maybe made into a full object. right now it simple incrementally blinks the 4 LEDS
*/
void Altimeter::manageLEDs()
{
  digitalWrite(LED_1, led_counter == 0);
  digitalWrite(LED_2, led_counter == 1);
  digitalWrite(LED_3, led_counter == 2);
  digitalWrite(LED_4, led_counter == 3);
  led_counter++;
  if (led_counter > 3)
  {
    led_counter = 0;
  }
}

/*
  This function manages the buzzer. also will be expanded, currently just buzzes
  four times, low or high depending on if an ematch is connected or not. Also may
  be converted into an object at some point.
*/
void Altimeter::manageBuzzer()
{
  if (buzzer_freq_scaler == 0)
  {
    switch (buzzer_counter)
    {
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
    case 12:
      buzzInidicate(flight_sensors.readESense() & (1 << 0));
      break;
    case 14:
      buzzInidicate(flight_sensors.readESense() & (1 << 1));
      break;
    case 16:
      buzzInidicate(flight_sensors.readESense() & (1 << 2));
      break;
    case 18:
      buzzInidicate(flight_sensors.readESense() & (1 << 3));
      break;
    default:
      buzzOff();
      break;
    }
    buzzer_counter++;
    if (buzzer_counter > 100)
      buzzer_counter = 0;
  }
  buzzer_freq_scaler++;
  if (buzzer_freq_scaler >= BUZZER_FREQ / BEEP_FREQ_HZ)
  {
    buzzer_freq_scaler = 0;
  }
}

/*
  This function turns on the buzzer, either high or low depending on the
  bool that it is passed.
*/
void Altimeter::buzzInidicate(bool buzz)
{
  if (buzz)
  {
    analogWriteFrequency(BUZZER, BUZZ_TONE_HIGH);
  }
  else
  {
    analogWriteFrequency(BUZZER, BUZZ_TONE_MID);
  }
  //COMMENT OUT TO DISABLE THE BUZZER
  analogWrite(BUZZER, 128);
}

/*
  Really simple function that just turns off the buzzer. Mostly just made it it's
  own function cause I like the name.
*/
void Altimeter::buzzOff()
{
  analogWrite(BUZZER, BUZZER_OFF);
}

/***** Transition checking functions ******/

bool Altimeter::checkOnRail()
{
  int len = flight_data.bno_array.array_length;
  Bno_Data data[len];
  Average<float> y_angle(len);
  flight_data.bno_array.getArray(data, (int)len);
  for (int i = 0; i < len; i++)
  {
    y_angle.push(data[i].euler.y);
  }
  float std = y_angle.stddev();
  float avg = y_angle.mean();
  //Serial.print("std: ");
  //Serial.print(std);
  //Serial.print(", mean: ");
  //Serial.println(avg);
  if (flight_state == IDLE)
  {
    return ((std < LAUNCH_ANGLE_STD_MAX) && (avg > LAUNCH_ANGLE_AVG_MIN));
  }
  if (flight_state == ARMED)
  {
    return (avg > LAUNCH_ANGLE_AVG_MIN);
  }
  return false;
}

bool Altimeter::checkLiftoff()
{
  int len = flight_data.mma_array.store_freq; //this will get one second of the array
  Mma_Data mma[len];
  flight_data.mma_array.getArray(mma, len);
  float dv = 0;
  for (int i = 0; i < len; i++)
  {
    dv += mma[i].y;
  }
  if (dv / len > LIFTOFF_DV_MIN)
    liftoff_accel = true;
  Serial.printf("%f ", dv / len);
  return (liftoff_accel);
}

void Altimeter::recoveryEvent(int event)
{
  if (event == MAIN && !(event_hist & (1 << event)))
  {
    Serial.printf("MAIN EVENT\n");
    ematch_triggers = ematch_triggers | (1 << 1);
    event_hist = event_hist | (1 << event);
  }
  if (event == DROGUE && !(event_hist & (1 << event)))
  {
    Serial.printf("DROGUE EVENT\n");
    ematch_triggers = ematch_triggers | (1 << 0);
    event_hist = event_hist | (1 << event);
  }
  if (event == SEP_BACKUP && !(event_hist & (1 << event)))
  {
    Serial.printf("SEP_BACKUP EVENT\n");
    ematch_triggers = ematch_triggers | (1 << 2);
    event_hist = event_hist | (1 << event);
  }
  if (event == SEP && !(event_hist & (1 << event)))
  {
    Serial.printf("SEP EVENT\n");
    event_hist = event_hist | (1 << event);
  }
}

void Altimeter::manageEmatches()
{
  for (int i = 0; i < NUM_EMATCHES; i++)
  {
    if (ematch_triggers & (1 << i))
      ematch_counters[i] = MAIN_FREQ;
  }
  ematch_triggers = 0;
  for (int i = 0; i < NUM_EMATCHES; i++)
  {
    if (ematch_counters[i] > 0)
    {
      ematch_counters[i]--;
      digitalWrite(ematch_pins[i], 1);
      digitalWrite(LED_3, 1);
    }
    else
    {
      digitalWrite(ematch_pins[i], 0);
    }
  }
}

void pollSerialtoMIN()
{
  timer = millis();
  //MIN Serial polling code
  char buf[32];
  size_t buf_len;
  if (RF_SERIAL.available() > 0)
  {
    buf_len = RF_SERIAL.readBytes(buf, 32U);
#ifdef DEBUG_MIN
    for (int i = 0; i < 32; i++)
    {
      Serial.printf("Sent: 0x%04x\n", buf[i]);
    }
    Serial.println();
#endif
  }
  else
  {
    buf_len = 0;
  }
  min_poll(&min_ctx_sradio, (uint8_t *)buf, (uint8_t)buf_len);

  if (ESP_SERIAL.available() > 0)
  {
    buf_len = ESP_SERIAL.readBytes(buf, 32U);
  }
  else
  {
    buf_len = 0;
  }
  min_poll(&min_ctx_esp, (uint8_t *)buf, (uint8_t)buf_len);
}

uint16_t min_tx_space(uint8_t port)
{
  switch (port)
  {
  case MIN_ESP:
    return 1000UL;
    break;
  case MIN_RF:
    return 1000UL;
    break;
  }
  return 0;
}

void min_tx_byte(uint8_t port, uint8_t byte)
{
  switch (port)
  {
  case MIN_ESP:
    ESP_SERIAL.write(&byte, 1U);
    break;
  case MIN_RF:
    RF_SERIAL.write(&byte, 1U);
  }

#ifdef DEBUG_MIN
  Serial.printf("Sent: 0x%04x\n", byte);
#endif
}

void min_application_handler(uint8_t min_id, uint8_t *min_payload, uint8_t len_payload, uint8_t port)
{
  switch (min_id)
  {
  case SKYB_CMD:
    Serial.println("Got SKYB_CMD");
    if (len_payload == sizeof(skyb_cmd_t))
    {
      memcpy(&skyb_cmd, min_payload, len_payload);
      process_commands();
    }
    else
    {
      Serial.printf("Size Err: %i/%i", len_payload, sizeof(skyb_cmd_t));
    }
    break;
  case ESP_STATUS:
    Serial.println("Got ESP_STATUS");
    min_send_frame(&min_ctx_sradio, min_id, min_payload, len_payload); //forward packet
    break;
  case ESP_ARM:
    Serial.println("Got ESP_ARM");
    min_send_frame(&min_ctx_esp, min_id, min_payload, len_payload); //forward packet
    break;
  case ESP_STAGE:
    Serial.println("Got ESP_STAGE");
    min_send_frame(&min_ctx_esp, min_id, min_payload, len_payload); //forward packet
    break;
  }
}
//void _softRestart();
void process_commands()
{
  if (skyb_cmd.reset)
  {
    //  _softRestart();
  }
  if (skyb_cmd.ematch1)
  {
    digitalWrite(TRIG_1, 1);
  }
  if (skyb_cmd.ematch2)
  {
    digitalWrite(TRIG_2, 1);
  }
  if (skyb_cmd.ematch3)
  {
    digitalWrite(TRIG_3, 1);
  }
  if (skyb_cmd.ematch4)
  {
    digitalWrite(TRIG_4, 1);
  }
}

/*
 * Breaks the rocket.
 * Do NOT call this function.
 * Definitely a bad idea.
 */
void Altimeter::breakRocket()
{
  Serial.println("FUCK BREAKING ROCKET");
  digitalWrite(TRIG_1, 1);
  digitalWrite(TRIG_2, 1);
  digitalWrite(TRIG_3, 1);
  digitalWrite(TRIG_4, 1);
  while (1)
    ;
}
// unless you like fun.

#ifndef LOGGER_H
#define LOGGER_H

#include "SdFat.h"

struct block_t {
  uint16_t count = 0;
  uint8_t data[510];
} __attribute__((packed));

class Logger{
 public:
   int initialize(int rough_bytes);

   void init_variable(uint8_t id, const char *name, uint8_t size);
   void log_variable(uint8_t id, void *obj, unsigned long log_time);

   void log();

   void finish_headers();


private:

  uint8_t *ptr();
  void inc(uint8_t sz);
  uint8_t next(uint8_t idx);

  SdFat sd;
  SdBaseFile binFile;
  #ifdef SITL_ON
  char fileName[14] = "sitl00.bin";
  #else
  char fileName[14] = "data00.bin";
  #endif
  uint32_t bgnBlock, endBlock;
  uint32_t curBlock;

  static const uint8_t BUFFER_SIZE = 20;

  block_t blocks[BUFFER_SIZE];

  uint8_t to_write = 0;
  uint8_t to_insert = 0;

  elapsedMicros timer;

  /* Sixteen loggable types should be enough for everybody.
   *                                           - Bill Gates */
  uint8_t varsz[16];
 };

#endif

#include "Logger.h"
#include "SdFat.h"
#include "string.h"
#include "Flight_Configuration.h"

/* Adapted from SdFat's LowLatencyLogger. */

int Logger::initialize(int rough_bytes) {
  int success = 1;

  if (!sd.begin(SD_CS, SPI_HALF_SPEED)) {
    sd.initErrorPrint();
  }

  Serial.println(F("Finding a file name"));
  while (sd.exists(fileName)) {
    if (fileName[5] != '9') {
      fileName[5]++;
    } else {
      fileName[5] = '0';
      if (fileName[4] == '9') {
        success = 0;
        break;
      }
      fileName[4]++;
    }
  }

  Serial.println(F("Creating contiguous range"));
  int block_count = rough_bytes/512;
  binFile.close();
  Serial.println(block_count);
  if (!binFile.createContiguous(sd.vwd(), fileName, 512 * block_count)) {
    Serial.println("failed");
    success = 0;
  }

  Serial.println(F("Accessing contiguous range"));
  if (!binFile.contiguousRange(&bgnBlock, &endBlock)) {
    Serial.println("failed");
    success = 0;
  }

  Serial.println(F("Erasing cache"));
  uint8_t* cache = (uint8_t*)sd.vol()->cacheClear();
  if (cache == 0) {
    Serial.println("failed");
    success = 0;
  }

  Serial.println(F("Erasing all data"));
  uint32_t bgnErase = bgnBlock;
  uint32_t endErase;
  while (bgnErase < endBlock) {
    endErase = bgnErase + 262144L; // 512^2
    if (endErase > endBlock) {
      endErase = endBlock;
    }
    if (!sd.card()->erase(bgnErase, endErase)) {
      success = 0;
      Serial.println("failed");
      break;
    }
    bgnErase = endErase + 1;
  }
  curBlock = bgnBlock;
  /*Serial.println(F("Starting multi-block write"));
  if (!sd.card()->writeStart(bgnBlock, block_count)) {
    Serial.println("failed");
    success = 0;
  }*/

  strcpy((char*) blocks[to_insert].data, ":skeleton:");
  blocks[to_insert].count = 11;
  return success;
}

uint8_t *Logger::ptr() {
  return blocks[to_insert].data + blocks[to_insert].count;
}

void Logger::inc(uint8_t sz) {
  blocks[to_insert].count += sz;
}

void Logger::init_variable(uint8_t id, const char* name, uint8_t size) {
  if (id > 15 || id < 0) {
    Serial.println("Something is very wrong.");
  }

  varsz[id] = size;

  *ptr() = id;
  inc(1);

  memcpy(ptr(), name, strlen(name)+1);
  inc(strlen(name)+1);

  *ptr() = size;
  inc(1);

}

void Logger::log_variable(uint8_t id, void *obj) {
  uint8_t sz = varsz[id];
  if (blocks[to_insert].count + sz + 5 > 510) {
    memset(ptr(), 255, 510-blocks[to_insert].count);
    blocks[to_insert].count = 510;
    to_insert = next(to_insert);
    if (blocks[to_insert].count != 0) {
      Serial.println("Wow you're going too fast.");
    }
  }
  *ptr() = id;
  inc(1);
  *(uint32_t*)ptr() = long(timer)/10; // 10^-5 seconds
  inc(4);
  memcpy(ptr(), obj, sz);
  inc(sz);
}

uint8_t Logger::next(uint8_t idx) {
  return (idx + 1) % BUFFER_SIZE;
}

void Logger::log() {
  if (!sd.card()->isBusy() && blocks[to_write].count == 510) {
    uint8_t cpy = to_write;
    uint8_t num = 0;
    while (blocks[cpy].count == 510) {
      num++;
      cpy = next(cpy);
    }
    if (!sd.card()->writeStart(curBlock, num)) {
        Serial.println("SD log: Couldn't write");
    }
    while (blocks[to_write].count == 510) {
      if (!sd.card()->writeData((uint8_t*)&blocks[to_write])) {
          Serial.println("SD log: Couldn't write 2");
      }
      if (to_insert == to_write) {
        to_insert = next(to_insert);
      }
      blocks[to_write].count = 0;
      to_write = next(to_write);
      curBlock++;
    }
    if (!sd.card()->writeStop()) {
      Serial.println("can't stop writing");
    }
    //if (sd.card()->writeBlock(curBlock, (uint8_t*)&blocks[to_write])) {
      /*Serial.println("Wrote");
      for (int i=0; i<512; i++)  {
        Serial.print(*((uint8_t*)&blocks[to_write] + i), HEX);
      }
      Serial.println();*/
    /*  if (to_insert == to_write) {
        to_insert = next(to_insert);
      }
      blocks[to_write].count = 0;
      to_write = next(to_write);
      curBlock++;
    } else {
      Serial.println("write failed?");
    }*/
  }
}

void Logger::finish_headers() {
  *ptr() = 255;
  blocks[to_insert].count = 510;
  to_insert = next(to_insert);
  log();
}

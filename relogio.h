#include <stdint.h>
#ifndef __RELOGIO_
#define __RELOGIO_

#include <Arduino.h>
#include <Ds1302.h>

class relogioObj{
private:
  Ds1302* rtc = nullptr;

public:
  void begin(int CE, int CLK, int IO);
  String getTempoString();
  String formatarSeg(uint32_t segundos);
  uint32_t timestamp();
};

extern relogioObj relogio;

#endif

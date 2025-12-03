#ifndef __TEMPERATURA_
#define __TEMPERATURA_

#include <DallasTemperature.h>
#include <OneWire.h>
#include <stdint.h>

class Temperatura {
private:
  DallasTemperature medidor;
  OneWire oneWire;

  uint8_t pino;

public:
  Temperatura(uint8_t pino);

  void begin();
  float medir();
};
#endif

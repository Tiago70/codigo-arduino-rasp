#include "temperatura.h"
#include <Arduino.h>

Temperatura::Temperatura(uint8_t pino) {
  this->pino = pino;
}

void Temperatura::begin() {
  pinMode(this->pino, INPUT);
  this->oneWire = OneWire(this->pino);
  this->medidor = DallasTemperature(&(this->oneWire));
  this->medidor.begin();
}

float Temperatura::medir(){
  this->medidor.requestTemperatures();
  float temperatura = this->medidor.getTempCByIndex(0);

  return temperatura;
}

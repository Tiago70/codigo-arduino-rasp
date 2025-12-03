#include "condutividade.h"
#include <Arduino.h>

Condutivimetro::Condutivimetro(uint8_t pino, Temperatura* temperatura) {
  this->pino = pino;
  this->sensor_temperatura = temperatura;
}

void Condutivimetro::begin() {
  pinMode(this->pino, INPUT);
}

float Condutivimetro::medir() {
  float vout = analogRead(this->pino);
  vout = (float)vout / 1023 * this->Vin;

  float resistencia = this->resistorPadrao * (vout / (this->Vin - vout));

  float condutividade = this->resistorPadrao / (resistencia * K);
  condutividade = condutividade * (1 / (1 + this->temp_coeficiente * (this->sensor_temperatura->medir() - 25)));

  return condutividade;
}

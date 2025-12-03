#ifndef __CONDUTIVIMETRO_
#define __CONDUTIVIMETRO_

#include "temperatura.h"

class Condutivimetro {
private:
  // atributos para o calculo da condutividade
  uint8_t Vin = 5;                    // valor da entrada em volts (5v do arduino)
  float K = 0.996;                    // valor médio do coeficiente K
  float temp_coeficiente = 0.015;     // também é um valor médio
  uint16_t resistorPadrao = 1000;     // valor em Ohms
  
  Temperatura* sensor_temperatura;
  uint8_t pino;

public:
  Condutivimetro(uint8_t pino, Temperatura* temperatura);

  void begin();
  float medir();
};

#endif

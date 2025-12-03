#include "Arduino.h"
#include "bomba.h"

Bomba::Bomba(uint8_t pino){
  this->pino = pino;
}

void Bomba::begin(){
  pinMode(this->pino, OUTPUT);
}

bool Bomba::definir_estado(bool estado){
  digitalWrite(pino, estado);
  return true;
}

#ifndef __BOMBA_
#define __BOMBA_

class Bomba{
private:
  uint8_t pino;

public:
  Bomba(uint8_t pino);

  void begin();
  bool definir_estado(bool estado);
};

#endif

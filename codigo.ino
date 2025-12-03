#include "temperatura.h"
#include "condutividade.h"
#include "bomba.h"

Temperatura temp_sensor(7);
Condutivimetro condut_sensor(A0, &temp_sensor);
Bomba bomba(2);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; 
  }

  temp_sensor.begin();
  condut_sensor.begin();
  bomba.begin();
}

float pegar_valor(char* equipamento){

    if (strcmp(equipamento, "TEMP") == 0){
        return temp_sensor.medir();
    } else if (strcmp(equipamento, "COND") == 0){
        return condut_sensor.medir();
    }
    else {
      return 0;
    }
}

bool definir_valor(char* equipamento, uint8_t valor){
  if (strcmp(equipamento, "BOMBA") == 0){
        return bomba.definir_estado(valor);
    }
}

const char* DELIMITADOR = " ";
void loop() {
  if (Serial.available() > 0) {
    String recebido = Serial.readStringUntil('\n');
    recebido.trim();
    if (recebido.length() == 0) return;
    char* str_char = recebido.c_str();

    char* metodo = strtok(str_char, DELIMITADOR);
    char* equipamento = strtok(NULL, DELIMITADOR);
    uint8_t valor = (strtok(NULL, DELIMITADOR)[0] - 48);

    if (strcmp(metodo, "GET") == 0){
      float retorno = pegar_valor(equipamento);
      Serial.print(equipamento);
      Serial.print(" ");
      Serial.println(retorno);
    }
    else {
      uint8_t retorno = definir_valor(equipamento, valor);
      Serial.print(equipamento);
      Serial.print(" ");
      Serial.println(retorno);
    }
  } 
}

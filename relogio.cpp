#include "relogio.h"

void relogioObj::begin(int CE, int CLK, int IO) {
  // Instância do RTC com pinos CE, IO, CLK
  this->rtc = new Ds1302(CE, CLK, IO);
  this->rtc->init();

  // código de configuração do horário do rtc
  // (deve ser executado apenas uma vez)
  Ds1302::DateTime dt;

  dt.year = 25;    // Ano 2025 (últimos dois dígitos)
  dt.month = 6;    // Junho
  dt.day = 17;     // Dia 17
  dt.dow = 2;      // Dia da semana (Terça-feira) 0-6
  dt.hour = 15;    // 15h
  dt.minute = 20;  // 20 minutos
  dt.second = 0;   // Começa em 00 segundos

  this->rtc->setDateTime(&dt);  // Define a data e hora no RTC
}

uint32_t relogioObj::timestamp() {
  Ds1302::DateTime now;
  this->rtc->getDateTime(&now);

  const int mes_em_dias[] = {
    0,    // janeiro
    31,   // fevereiro
    59,   // março
    90,   // abril
    120,  // maio
    151,  // junho
    181,  // julho
    212,  // agosto
    243,  // setembro
    273,  // outubro
    304,  // novembro
    334   // dezembro
  };

  uint8_t ano, mes, dia, hora, minuto, segundo;
  ano = now.year;
  mes = now.month;
  dia = now.day;
  hora = now.hour;
  minuto = now.minute;
  segundo = now.second;

  //  compensa o dia ainda não terminado
  dia--;

  // soma todos os dias dos anos bissexto desde 1970
  for (int a = 1970; a < ano; a++) {
    if ((a % 4 == 0 && a % 100 != 0) || a % 400 == 0) {
      dia++;
    }
  }

  // soma mais um dia se o ano atual é bissexto e se já passou fevereiro
  if ((ano % 4 == 0 && ano % 100 != 0) || ano % 400 == 0) {
    if (mes > 2) dia++;
  }

  // calculo final
  dia += (ano - 1970) * 365;
  dia += mes_em_dias[mes-1];

  return (dia * 24 * 60 * 60) + (hora * 60 * 60) + (minuto * 60) + segundo;
}

// retorna as horas minutos e segundos atuais de forma formatada
String relogioObj::getTempoString() {
  Ds1302::DateTime now;
  this->rtc->getDateTime(&now);

  char buffer[9];
  sprintf(buffer, "%02d:%02d:%02d", now.hour, now.minute, now.second);
  return String(buffer);
}


String relogioObj::formatarSeg(uint32_t segundos) {
  uint8_t horas;
  uint8_t minutos;

  horas = segundos / 3600;
  segundos %= 3600;
  minutos = segundos / 60;
  segundos %= 60;

  char buffer[9];
  sprintf(buffer, "%02d:%02d:%02d", horas, minutos, segundos);

  return buffer;
}

relogioObj relogio;

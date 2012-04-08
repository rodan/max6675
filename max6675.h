#ifndef __max6675_h_
#define __max6675_h_

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

void tk_init(uint8_t cs_pin);
uint16_t tk_get_raw(uint8_t cs_pin);

#endif

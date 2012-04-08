
#include "max6675.h"

uint8_t max6675_cs = 9;         // chip select pin
unsigned long prev = 0, interval = 5000;

void setup()
{
    Serial.begin(9600);
    tk_init(max6675_cs);

    // wake up xbee to see the output
    // can be ignored in any other setup
    pinMode(A2, OUTPUT);
    digitalWrite(A2, LOW);
}

void loop()
{
    char buff[20];
    char tmp[7];
    unsigned long now = millis();

    if ((now - prev > interval) && (Serial.available() <= 0)) {
        snprintf(buff, 20, "temp is %s dC",
                 dtostrf(0.25 * tk_get_raw(max6675_cs), 2, 2, tmp));
        Serial.println(buff);
        prev = now;
    }
}


/*
  MAX6675 library for the Arduino.

  This library reads the temperature from a MAX6675 thermocouple-to-digital IC

  Author:          Petre Rodan <petre.rodan@simplex.ro>
  Available from:  https://github.com/rodan/max6675

  The MAX6675 performs cold-junction compensation and digitizes the 
  signal from a type-K thermocouple. The data is output in a 12-bit
  resolution, SPI-compatible, read-only format.

  GNU GPLv3 license:
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
   
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
   
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
   
*/

#include <SPI.h>
#include "max6675.h"

void tk_init(uint8_t cs_pin)
{
    pinMode(10, OUTPUT);        // pin 10 must be an output for SPI to work, 
    // even if cs_pin != 10
    pinMode(cs_pin, OUTPUT);    // MAX6675 chip select (CS) pin
    digitalWrite(cs_pin, HIGH); // disable chip

    SPI.begin();
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE1); // capture data on clock's falling edge.
}

// D15    - bogus bit
// D14-D3 - temperature*4 in degrees celsius, MSB first
// D2     - thermocouple input (1 if open)
// D1     - device id?
// D0     - state ?

uint16_t tk_get_raw(uint8_t cs_pin)
{
    uint16_t rv = 0;
    uint8_t msb, lsb;

    // initiate a new conversion
    digitalWrite(cs_pin, LOW);  // enable chip
    delay(2);
    digitalWrite(cs_pin, HIGH);
    delay(220);                 // wait for conversion

    // read result
    digitalWrite(cs_pin, LOW);

    msb = SPI.transfer(0x0);
    lsb = SPI.transfer(0x0);

    rv = ((uint16_t) msb << 8 ) | ((uint16_t) lsb);

    digitalWrite(cs_pin, HIGH); // disable chip

    // error out if the thermocouple input is open (D2 == 1)
    if (bitRead(rv, 2) == 1)
        return 1023;

    // only send the interesting bits (D14-D3)
    return ((rv & 0x7ff8) >> 3);
}

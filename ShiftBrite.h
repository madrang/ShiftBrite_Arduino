/*This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA*/

#ifndef ShiftBrite_h
#define ShiftBrite_h

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class ShiftBrite {
private:
	uint8_t clockpin;
	uint8_t enablepin;
	uint8_t latchpin;
	uint8_t datapin;

protected:
	boolean useSPI;

	void writeSoft(uint8_t data);
	void writeSPI(uint8_t data);

public:
	ShiftBrite(uint8_t data, uint8_t latch, uint8_t enable, uint8_t clock);

	void SendCurrent(uint16_t RedCurrent, uint16_t GreenCurrent, uint16_t BlueCurrent);
	void SendColor(uint16_t RedValue, uint16_t GreenValue, uint16_t BlueValue);
	void LatchData();		
};

#endif

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

#include "ShiftBrite.h"

ShiftBrite::ShiftBrite(uint8_t data, uint8_t latch, uint8_t enable, uint8_t clock){
	datapin = data;
	latchpin = latch;
	enablepin = enable;
	clockpin = clock;
	
	pinMode(latchpin, OUTPUT);
	pinMode(enablepin, OUTPUT);
	
	digitalWrite(latchpin, LOW);
	digitalWrite(enablepin, LOW);
	
	useSPI = (clockpin == 13 && datapin == 11);
	
	if(useSPI) {
		SPCR = (1<<SPE)|(1<<MSTR)|(0<<SPR1)|(0<<SPR0);
	} else {
		pinMode(datapin, OUTPUT);
		pinMode(clockpin, OUTPUT);
		
		digitalWrite(datapin, LOW);
		digitalWrite(clockpin, LOW);
	}
}

void ShiftBrite::writeSoft(uint8_t data){
	shiftOut(datapin, clockpin, MSBFIRST, data);
}

void ShiftBrite::writeSPI(uint8_t data){
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
}

//
void ShiftBrite::SendColor(uint16_t RedValue, uint16_t GreenValue, uint16_t BlueValue){
	if(useSPI) {
		writeSPI(B00 << 6 | BlueValue >> 4);
		writeSPI(BlueValue << 4 | RedValue >> 6);
		writeSPI(RedValue << 2 | GreenValue >> 8);
		writeSPI(GreenValue);
	} else {
		writeSoft(B00 << 6 | BlueValue >> 4);
		writeSoft(BlueValue << 4 | RedValue >> 6);
		writeSoft(RedValue << 2 | GreenValue >> 8);
		writeSoft(GreenValue);
	}
}

// Write to current control registers
void ShiftBrite::SendCurrent(uint16_t RedCurrent, uint16_t GreenCurrent, uint16_t BlueCurrent){
	if(useSPI) {
		writeSPI(B01 << 6 | BlueCurrent >> 4);
		writeSPI(BlueCurrent << 4 | RedCurrent >> 6);
		writeSPI(RedCurrent << 2 | GreenCurrent >> 8);
		writeSPI(GreenCurrent);
	} else {
		writeSoft(B01 << 6 | BlueCurrent >> 4);
		writeSoft(BlueCurrent << 4 | RedCurrent >> 6);
		writeSoft(RedCurrent << 2 | GreenCurrent >> 8);
		writeSoft(GreenCurrent);
	}
}

// latch data into registers
void ShiftBrite::LatchData(){
	delayMicroseconds(15);
	digitalWrite(latchpin,HIGH);
	delayMicroseconds(15);
	digitalWrite(latchpin,LOW);
	delayMicroseconds(15);
}

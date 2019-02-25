/**
 * @brief Direct port manipulation of a digital output.
 * 
 * @file DigitalOutput.cpp
 * @author Leonardo Molina (leonardomt@gmail.com)
 * @date 2016-12-03
 * @version: 0.1.180723
**/
#include <Arduino.h>
#include "DigitalOutput.h"
#include "tools.h"

namespace bridge {
	DigitalOutput::DigitalOutput(int8_t pin) :
	port(BRIDGE_BASEREG(pin)),
	mask(BRIDGE_BITMASK(pin)),
	state(LOW)
	{
		pinMode(pin, OUTPUT);
	}
	
	void DigitalOutput::Set(bool state) {
		this->state = state;
		if (state)
			BRIDGE_WRITE_HIGH(port, mask);
		else
			BRIDGE_WRITE_LOW(port, mask);
	}
	
	void DigitalOutput::Toggle() {
		Set(!state);
	}
}
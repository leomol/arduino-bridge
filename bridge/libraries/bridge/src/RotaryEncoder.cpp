/*
	Created on: 2016-12-01
	
	@author Leonardo Molina (leonardomt@gmail.com).
	@version 1.1.180622
*/

#include <Arduino.h>
#include "RotaryEncoder.h"
#include "tools.h"
#include "meta.h"

namespace bridge {
	RotaryEncoder::RotaryEncoder(int8_t pin1, int8_t pin2, Function function, FunctionData functionData, int32_t data) :
		// Digital pin numbers, register addresses and pin masks.
		pin1(pin1),
		pin2(pin2),
		function(function),
		functionData(functionData),
		data(data),
		
		port1(BRIDGE_BASEREG(pin1)),
		port2(BRIDGE_BASEREG(pin2)),
		mask1(BRIDGE_BITMASK(pin1)),
		mask2(BRIDGE_BITMASK(pin2)),
		
		// Value synchronization.
		asyncValue(0),
		syncValue(0)
	{
		pinMode(pin1, INPUT_PULLUP);
		pinMode(pin2, INPUT_PULLUP);
		lastPin1State = BRIDGE_READ(port1, mask1);
		int interruptId = digitalPinToInterrupt(pin1);
		if (interruptId >= 0) {
			interruptible = true;
			// std is not supported in Arduino and lambda expressions cannot be passed
			// as arguments to functions when capturing variables. As a solution, map 
			// from (*void)(void) to (*void)(int) "previously" declared:
			attachInterrupt(interruptId, meta::Wrap(OnRise, (Data) this), RISING);
		} else {
			interruptible = false;
		}
	}
	
	RotaryEncoder::~RotaryEncoder() {
		detachInterrupt(digitalPinToInterrupt(pin1));
	}
	
	void RotaryEncoder::OnRise(Data data) {
		((RotaryEncoder*) data)->OnRise();
	}
	
	void RotaryEncoder::OnRise() {
		asyncValue += BRIDGE_READ(port2, mask2) ? +1 : -1;
	}
	
	void RotaryEncoder::Step() {
		int32_t change;
		if (interruptible) {
			noInterrupts();
			change = asyncValue - syncValue;
			syncValue = asyncValue;
			interrupts();
		} else {
			bool currentPin1State = BRIDGE_READ(port1, mask1);
			if (lastPin1State == LOW && currentPin1State == HIGH) {
				change = BRIDGE_READ(port2, mask2) ? +1 : -1;
				syncValue += change;
			} else {
				change = 0;
			}
			lastPin1State = currentPin1State;
		}
	
		if (change != 0) {
			if (function)
				function(this, change);
			else
				functionData(this, change, data);
		}
	}
	
	int32_t RotaryEncoder::GetValue() {
		return syncValue;
	}
	
	int8_t RotaryEncoder::GetPin1() {
		return pin1;
	}
	
	int8_t RotaryEncoder::GetPin2() {
		return pin2;
	}
}
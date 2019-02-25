#include <Arduino.h>
#include "GetRotation.h"
#include "types.h"

namespace bridge {
	GetRotation::GetRotation(int8_t hid0, int8_t hid1, uint8_t factor) :
	hid0(hid0),
	hid1(hid1),
	count(0),
	lastCount(0),
	factor(factor)
	{	
		// Turn pins into inputs.
		pinMode(hid0, INPUT_PULLUP);
		pinMode(hid1, INPUT_PULLUP);
		
		this->state0 = digitalRead(hid0);
	}
	
	// Event receiver.
	void GetRotation::step(uint64_t tic) {
		step(tic, digitalRead(hid0));
	}
	
	void GetRotation::step(uint64_t tic, uint8_t parameter) {
		bool state0;
		if (parameter == 255) {
			state0 = true;
			this->state0 = false;
		} else {
			state0 = parameter;
		}
		if (state0 && !this->state0) {
			int8_t delta = digitalRead(hid1) ? +1 : -1;
			count += delta;
		}
		this->state0 = state0;
	}
	
	void GetRotation::report(ReportFunction reportFunction) {
		noInterrupts();
		int64_t copy = count;
		interrupts();
		int64_t current = copy / factor;
		if (lastCount != current) {
			reportFunction(hid0, copy, current - lastCount);
			lastCount = current;
		}
	}
	
	int GetRotation::index() {
		return hid0;
	}
}
#include <Arduino.h>
#include "GetLevel.h"
#include "types.h"

namespace bridge {
	GetLevel::GetLevel(int8_t hid, uint64_t debounceRise, uint64_t debounceFall) :
	hid(hid),
	// Initialize debouncing function.
	debounceRise(debounceRise),
	debounceFall(debounceFall),
	debounceNext(0),
	// Report first state counting from zero, after debouncing.
	lastState(0),
	debouncingState(0)
	{
		// Turn pin into an input.
		pinMode(hid, INPUT);
		
		state = analogRead(hid);
	}

	// Event receiver.
	void GetLevel::step(uint64_t tic) {
		uint8_t state = analogRead(hid);
		
		// Debounced read.
		if (state != this->debouncingState) {
			bool fall = this->state > state;
			this->debounceNext = tic + (fall ? debounceFall : debounceRise);
			this->debouncingState = state;
		}
		
		if (state != this->state && tic >= debounceNext) {
			this->state = state;
		}
	}
	
	void GetLevel::report(ReportFunction reportFunction) {
		if (state != lastState)
			reportFunction(hid, state, state - lastState);
			lastState = state;
	}
	
	int GetLevel::index() {
		return hid;
	}
}
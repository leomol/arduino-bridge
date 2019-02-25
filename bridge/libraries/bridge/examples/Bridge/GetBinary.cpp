#include <Arduino.h>
#include "GetBinary.h"
#include "types.h"

namespace bridge {
	GetBinary::GetBinary(int8_t hid, uint64_t debounceRise, uint64_t debounceFall, uint8_t factor) :
	hid(hid),
	// Initialize debouncing function.
	debounceRise(debounceRise),
	debounceFall(debounceFall),
	debounceNext(0),
	debouncingState(2),
	factor(factor),
	// Initialize count.
	setup(true)
	{
		// Turn pin into an input.
		pinMode(hid, INPUT_PULLUP);
		
		// Get current state.
		state = digitalRead(hid);
	}
	
	// Event receiver.
	void GetBinary::step(uint64_t tic) {
		step(tic, digitalRead(hid));
	}
	
	/*	step(tic, parameter)
		parameter == 255 ? <toggle-state> : state = parameter
	 */
	void GetBinary::step(uint64_t tic, uint8_t parameter) {
		uint8_t currentState;
		if (parameter == 255)
			currentState = !state;
		else
			currentState = parameter;
		
		// Debounced read.
		if (currentState != debouncingState) {
			// !! 
			/* reporting half the pulses:
				GetBinary(21, 10, 10, 1)
				SetPulse(13, 0, 50000, 50000, 10) --> 5 pulses in Arduino-debug, Matlab, and Serializer
			/* reporting all pulses:
				GetBinary(21, 0, 0, 1)
				SetPulse(13, 0, 50000, 50000, 10) --> 10 pulses in Arduino-debug, Matlab, and Serializer
			*/
			debouncingState = currentState;
			debounceNext = tic + (currentState ? debounceFall : debounceRise);
		}
		
		if (currentState != state && tic >= debounceNext) {
			// State debounced (accepted).
			state = currentState;
			// Increment count on change.
			count[currentState]++;
		}
	}
	
	void GetBinary::report(ReportFunction reportFunction) {
		if (setup) {
			setup = false;
			// First report corresponds to current state.
			int8_t value = state == 1 ? +1 : -1;
			reportFunction(hid, value, value);
		} else {
			// Further reports represent actual changes.
			uint64_t countFactored[2] = {(count[0] + factor - 1) / factor, (count[1] + factor - 1) / factor};
			uint64_t difference[2] = {countFactored[0] - last[0], countFactored[1] - last[1]};
			last[0] = countFactored[0];
			last[1] = countFactored[1];
			if (state) {
				reportFunction(hid, -countFactored[0], -difference[0]);
				reportFunction(hid, +countFactored[1], +difference[1]);
			} else {
				reportFunction(hid, +countFactored[1], +difference[1]);
				reportFunction(hid, -countFactored[0], -difference[0]);
			}
		}
	}
	
	int GetBinary::index() {
		return hid;
	}
}
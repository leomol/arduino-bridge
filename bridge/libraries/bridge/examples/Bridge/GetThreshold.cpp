#include <Arduino.h>
#include "GetThreshold.h"
#include "types.h"

namespace bridge {
	GetThreshold::GetThreshold(int8_t hid, uint8_t threshold, uint64_t debounceRise, uint64_t debounceFall) :
	hid(hid),
	threshold(threshold),
	// Initialize debouncing function.
	debounceRise(debounceRise),
	debounceFall(debounceFall),
	changes(0),
	debounceNext(0)
	{
		// Turn pin into an input.
		pinMode(hid, INPUT);
		
		state = analogRead(hid) < threshold;
		lastState = state;
		debouncingState = state;
	}

	// Event receiver.
	void GetThreshold::step(uint64_t tic) {
		bool current = analogRead(hid) >= threshold;
		
		// Debounced read.
		if (current != debouncingState) {
			debounceNext = tic + (state ? debounceFall : debounceRise);
			debouncingState = current;
		}
		
		if (current != state && tic >= debounceNext) {
			state = current;
			changes++;
		}
	}
	
	void GetThreshold::report(ReportFunction reportFunction) {
		while (changes > 0) {
			changes -= 1;
			lastState = !lastState;
			reportFunction(hid, lastState, lastState ? 1 : -1);
		}
	}
	
	int GetThreshold::index() {
		return hid;
	}
}
#include <Arduino.h>
#include "SetPulse.h"

namespace bridge {
	SetPulse::SetPulse() {
	}
	
	SetPulse::SetPulse(int8_t hid, bool stateStart, uint64_t durationLow, uint64_t durationHigh, uint64_t repetitions) :
	hid(hid),
	state(stateStart),
	stateStart(stateStart),
	durationLow(durationLow),
	durationHigh(durationHigh),
	repetitions(repetitions),
	
	tic(0),
	finite(repetitions > 0),
	// Start in sync with a period.
	starting(true),
	running(true)
	{
		pinMode(hid, OUTPUT);
	}

	// Event receiver.
	void SetPulse::step(uint64_t tic) {
		// Check phase of the square wave.
		if (starting) {
			// Initialize.
			starting = false;
			write(stateStart);
			// Schedule next phase change.
			this->tic = tic + (stateStart ? durationHigh : durationLow);
		} else if (running && tic >= this->tic) {
			// Write opposite state.
			write(!state);
			// A phase is completed at stateEnd
			if (finite && state != stateStart && --repetitions == 0)
				running = false;
			this->tic = tic + (state ? durationHigh : durationLow);
		}
	}
	
	// Set state and stop any schedule.
	void SetPulse::write(bool state) {
		this->state = state;
		digitalWrite(hid, state);
	}
	
	void SetPulse::stop() {
		// Disable routine.
		running = false;
	}
	
	int SetPulse::index() {
		return hid;
	}
}
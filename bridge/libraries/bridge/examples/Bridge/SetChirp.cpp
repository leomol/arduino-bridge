// c 13 10000 10000 400 2200 1000000 
// c 13 10000 10000 2200 400 1000000 
#include <Arduino.h>
#include "SetChirp.h"

namespace bridge {
	SetChirp::SetChirp() {
	}
	
	SetChirp::SetChirp(int8_t hid, uint64_t durationLowStart, uint64_t durationLowStop, uint64_t durationHighStart, uint64_t durationHighStop, uint64_t duration) :
	hid(hid),
	durationLowStart(durationLowStart),
	durationLowStop(durationLowStop),
	durationHighStart(durationHighStart),
	durationHighStop(durationHighStop),
	
	starting(true),
	running(true),
	fixing(true)
	{
		pinMode(hid, OUTPUT);
		this->state = digitalRead(hid);
		
		duration = max(duration, 1);
		if (durationLowStart < durationLowStop) {
			slopeLow = durationLowStop - durationLowStart;
		} else {
			slopeLow = durationLowStart - durationLowStop;
			slopeLow *= -1;
		}
		slopeLow /= duration;
		
		if (durationHighStart < durationHighStop) {
			slopeHigh = durationHighStop - durationHighStart;
		} else {
			slopeHigh = durationHighStart - durationHighStop;
			slopeHigh *= -1;
		}
		slopeHigh /= duration;
		this->duration = duration;
	}
	
	// Event receiver.
	void SetChirp::step(uint64_t tic) {
		// Check phase of the square wave.
		if (starting) {
			// Initialize.
			starting = false;
			// Schedule next phase change.
			ticStart = tic;
			uint64_t width = state ? durationHighStart : durationLowStart;
			this->tic = tic + width;
		} else if (running && tic >= this->tic) {
			// Flip state.
			write(!state);
			// Schedule next phase change.
			uint64_t elapsed = tic - ticStart;
			uint64_t width = state ? durationHighStart + slopeHigh * elapsed : durationLowStart + slopeLow * elapsed;
			this->tic = tic + width;
			if (elapsed > duration)
				running = false;
		}
		// else if (fixing && tic >= this->tic) {
			// // Flip state.
			// write(!state);
			// // Schedule next phase change.
			// uint64_t width = state ? durationHighStop : durationLowStop;
			// this->tic = tic + width;
		// }
	}
	
	void SetChirp::stop() {
		// Disable routine.
		running = false;
		fixing = false;
	}
	
	// Set state and stop any schedule.
	void SetChirp::write(bool state) {
		this->state = state;
		digitalWrite(hid, state);
	}
	
	int SetChirp::index() {
		return hid;
	}
}
#include <Arduino.h>
#include "SetBinary.h"

namespace bridge {
	SetBinary::SetBinary(int8_t hid, bool state) :
	hid(hid),
	state(state)
	{
		pinMode(hid, OUTPUT);
		digitalWrite(hid, state);
	}
	
	void SetBinary::set(bool state) {
		// Write if state changed.
		if (this->state != state) {
			this->state = state;
			digitalWrite(hid, state);
		}
	}
	
	int SetBinary::index() {
		return hid;
	}
}
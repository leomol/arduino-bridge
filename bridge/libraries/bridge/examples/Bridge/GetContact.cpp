/*
	Created on: 2016-12-01
	
	\author Leonardo Molina (leonardomt@gmail.com).
	\version 1.1.180927
*/

#include <Arduino.h>
#include "GetContact.h"
#include "TouchSensor.h"

namespace bridge {
	GetContact::GetContact(int8_t hid0, int8_t hid1, uint8_t nPeriods, uint8_t threshold, uint32_t debounceRise, uint32_t debounceFall) : 
	// Initialize IO.
	hid0(hid0),
	count(0),
	lastReportedCount(0),
	lastReportedState(HIGH)
	{
		touchSensor = new TouchSensor(hid0, hid1, nPeriods, threshold, debounceRise, debounceFall, GetContact::onChange, (uintptr_t) this);
	}

	// Event receiver.
	void GetContact::step(uint64_t tic) {
		touchSensor->Step();
	}
	
	void GetContact::onChange(TouchSensor* touchSensor, bool state, uintptr_t data) {
		GetContact* self = (GetContact*) data;
		self->count += 1;
	}
	
	void GetContact::report(ReportFunction reportFunction) {
		// Report on change.
		while (lastReportedCount < count) {
			reportFunction(hid0, lastReportedCount, lastReportedState ? -1 : +1);
			lastReportedState = !lastReportedState;
			lastReportedCount += 1;
		}
	}
	
	int GetContact::index() {
		return -1;
	}
}
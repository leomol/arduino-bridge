/*
	Created on: 2016-12-01
	
	\author Leonardo Molina (leonardomt@gmail.com).
	\version 1.1.180927
*/

#ifndef BRIDGE_GETTOUCH_H
#define BRIDGE_GETTOUCH_H

#include <stdint.h>
#include "Routine.h"
#include "TouchSensor.h"
#include "types.h"

namespace bridge {
	class GetContact : public Routine {
		public:
			GetContact(int8_t hid0, int8_t hid1, uint8_t nPeriods, uint8_t threshold, uint32_t debounceRise, uint32_t debounceFall);
			//~GetContact();
			bool test(bool &state);
			void step(uint64_t tic);
			void report(ReportFunction reportFunction);
			int index();
			
		private:
			int8_t hid0;				// Active pin. Store it for report purposes.
			TouchSensor* touchSensor;
			
			static void onChange(TouchSensor* touchSensor, bool state, uintptr_t data);
			uint32_t count;				// Current contact count.
			bool lastReportedState;		// Last reported state.
			uint32_t lastReportedCount;	// Last reported count.
	};
}

#endif
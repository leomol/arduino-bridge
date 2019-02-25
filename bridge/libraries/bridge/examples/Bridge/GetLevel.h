#ifndef GETLEVEL_H
#define GETLEVEL_H

#include <stdint.h>
#include "Routine.h"
#include "types.h"

namespace bridge {
	class GetLevel : public Routine {
		public:
			GetLevel(int8_t hid, uint64_t debounceRise, uint64_t debounceFall);
			void step(uint64_t tic);
			void report(ReportFunction reportFunction);
			int index();
			
		private:
			int8_t hid;					// Active pin.
			uint8_t state;				// Last known pin state.
			uint8_t lastState;
			uint8_t debouncingState;	// Debounce state.
			uint64_t debounceRise;		// Debounce duration from low to high.
			uint64_t debounceFall;		// Debounce duration from high to low.
			uint64_t debounceNext;		// Ticker for debounce control.
	};
}

#endif
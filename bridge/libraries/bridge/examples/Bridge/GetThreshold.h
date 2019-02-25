#ifndef GETTRESHOLD_H
#define GETTRESHOLD_H

#include <stdint.h>
#include "Routine.h"
#include "types.h"

namespace bridge {
	class GetThreshold : public Routine {
		public:
			GetThreshold(int8_t hid, uint8_t threshold, uint64_t debounceRise, uint64_t debounceFall);
			void step(uint64_t tic);
			void report(ReportFunction reportFunction);
			int index();
			
		private:
			int8_t hid;				// Active pin.
			uint8_t threshold;		// Threshold to toggle state.
			int32_t changes;
			bool state;				// Last known pin state.
			bool lastState;			// 
			bool debouncingState;	// Debounce state.
			uint64_t debounceRise;	// Debounce duration from low to high.
			uint64_t debounceFall;	// Debounce duration from high to low.
			uint64_t debounceNext;	// Ticker for debounce control.
	};
}

#endif
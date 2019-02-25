#ifndef GETBINARY_H
#define GETBINARY_H

#include <stdint.h>
#include "Routine.h"
#include "types.h"

namespace bridge {
	class GetBinary : public Routine {
		public:
			GetBinary(int8_t hid, uint64_t debounceRise, uint64_t debounceFall, uint8_t factor);
			void step(uint64_t tic);
			void step(uint64_t tic, uint8_t state);
			void report(ReportFunction reportFunction);
			int index();
			
		private:
			bool setup;
			int8_t hid;					// Active pin.
			uint64_t count[2]{0};		// Number of counts for each state.
			uint64_t last[2]{0};
			uint8_t state;				// Last known pin state.
			uint8_t debouncingState;	// Debounce state.
			uint64_t debounceRise;		// Debounce duration from low to high.
			uint64_t debounceFall;		// Debounce duration from high to low.
			uint64_t debounceNext;		// Ticker for debounce control.
			uint8_t factor;				// 
	};
}

#endif
#ifndef GETROTATION_H
#define GETROTATION_H

#include <stdint.h>
#include "Routine.h"
#include "types.h"

namespace bridge {
	class GetRotation : public Routine {
		public:
			GetRotation(int8_t hid0, int8_t hid1, uint8_t factor);
			void step(uint64_t tic);
			void step(uint64_t tic, uint8_t parameter);
			void report(ReportFunction reportFunction);
			int index();
			
		private:
			bool state0;					// Last known pin state for hid0.
			int8_t hid0;					// Active pin.
			int8_t hid1;					// Passive pin.
			int64_t count;					// Number of counts for each state.
			int64_t lastCount;
			uint8_t factor;					// 
	};
}

#endif
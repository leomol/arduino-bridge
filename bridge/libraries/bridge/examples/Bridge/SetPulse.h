#ifndef SETPULSE_H
#define SETPULSE_H

#include <stdint.h>
#include "Routine.h"

namespace bridge {
	class SetPulse : public Routine {
		public:
			SetPulse();
			SetPulse(int8_t hid, bool stateStart, uint64_t durationLow, uint64_t durationHigh, uint64_t repetitions);
			void step(uint64_t tic);
			int index();
			
		private:
			int8_t hid;					// Pin id in hardware.
			bool starting;				// Whether ticker will start with the next step.
			bool running;				// Whether the routine is executing.
			bool finite;				// Whether the number of repetitions is finite.
			bool state;					// Last known state.
			bool stateStart;			// Make this the first state.
			uint64_t tic;				// Next scheduled phase change.
			uint64_t durationLow;		// Duration of low phase.
			uint64_t durationHigh;		// Duration of high phase.
			uint64_t repetitions;		// Phase control.
			void write(bool state);
			void stop();
	};
}

#endif
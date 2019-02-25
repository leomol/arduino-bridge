#ifndef SETCHIRP_H
#define SETCHIRP_H

#include <stdint.h>
#include "Routine.h"

namespace bridge {
	class SetChirp : public Routine {
		public:
			SetChirp();
			SetChirp(int8_t hid, uint64_t durationLowStart, uint64_t durationLowStop, uint64_t durationHighStart, uint64_t durationHighStop, uint64_t duration);
			void step(uint64_t tic);
			int index();
			
		private:
			int8_t hid;					// Pin id in hardware.
			bool state;					// Last state.
			bool starting;				// Whether ticker will start with the next step.
			volatile bool running;		// Whether the routine is executing.
			volatile bool fixing;		// 
			uint64_t ticStart;			// 
			uint64_t tic;				// Next scheduled phase change.
			double slopeLow;
			double slopeHigh;
			uint64_t durationLowStart;	// Duration of low phase.
			uint64_t durationLowStop;	// 
			uint64_t durationHighStart;	// Duration of high phase.
			uint64_t durationHighStop;	// 
			uint64_t duration;			// Phase control.
			void write(bool state);
			void stop();
	};
}

#endif
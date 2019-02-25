#ifndef ROUTINE_H
#define ROUTINE_H

#include <stdint.h>
#include "types.h"

namespace bridge {
	class Routine {
		public:
			virtual void report(ReportFunction reportFunction) {
			}
			
			virtual void step() {
			}
			
			virtual void step(uint64_t tic) {
			}
			
			virtual void step(uint64_t tic, uint8_t parameter) {
			}
			
			virtual void stop() {
			}
			
			virtual int index() {
				return -1;
			}
	};
}
#endif
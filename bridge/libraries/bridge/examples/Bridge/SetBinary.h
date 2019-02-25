#ifndef SETBINARY_H
#define SETBINARY_H

#include <stdint.h>
#include "Routine.h"

namespace bridge {
	class SetBinary : public Routine {
		public:
			SetBinary(int8_t hid, bool state);
			void write(bool state);
			int index();
			
		private:
			int8_t hid;			// Pin id.
			bool state;			// Last known state.
			void set(bool state);
	};
}

#endif
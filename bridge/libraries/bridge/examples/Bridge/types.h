#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

namespace bridge {
	typedef void (*IntFunction   ) (uint8_t id);
	typedef void (*VoidFunction  ) (void);
	typedef void (*TicFunction   ) (uint64_t tic);
	typedef void (*ReportFunction) (int8_t hid, int32_t current, int32_t delta);
}

#endif
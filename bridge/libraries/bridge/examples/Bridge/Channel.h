#ifndef CHANNEL_H
#define CHANNEL_H

#include <stdint.h>
#include "HardwareSerial.h"

namespace bridge {
	class Channel {
		public:
			Channel();
			Channel(HardwareSerial* serial);
			bool available();
			uint8_t read();								// Block-read one byte from Serial.
			uint8_t peek();								// Block-peek one byte from Serial.
			void read(uint8_t* bytes, uint8_t nbytes);	// Read n bytes from Serial into input array.
			uint64_t parse(uint64_t max, bool trim);	// 
			uint64_t parse(uint64_t max);				// 
			uint64_t next(uint8_t nbits);				// Read the next n bits from serial (up to 64).
			void next();								// Drop bits to the right of current byte.
		
		private:
			uint64_t pack(uint8_t* bytes, uint8_t nbits);
			uint64_t decompress(uint64_t number, uint8_t offset, uint8_t width); // Extract a number from a subset of bits in another number.
			uint8_t shift(uint8_t* bytes, uint8_t nbytes, int8_t s);
			uint64_t rmask(uint8_t width);
			uint8_t modcom(int8_t x, int8_t k);
			uint8_t nremainder;
			uint8_t buffer[8 + 1];
			HardwareSerial* serial;
	};
}

#endif
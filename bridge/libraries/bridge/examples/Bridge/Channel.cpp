#include <Arduino.h>
#include "Channel.h"

namespace bridge {
	Channel::Channel() {
	}
	
	Channel::Channel(HardwareSerial* serial) : nremainder(0) {
		this->serial = serial;
		buffer[0] = 0;
	}
	 
	// Block until successful peek.
	uint8_t Channel::peek() {
		while (!serial->available()) {}
		return serial->peek();
	}
	
	// Block until successful read.
	uint8_t Channel::read() {
		while (!serial->available()) {}
		return serial->read();
	}
	
	// Read a number of bytes.
	void Channel::read(uint8_t* bytes, uint8_t nbytes) {
		for (uint8_t i = 0; i < nbytes; i++)
			bytes[i] = read();
	}
	
	// ....m.... --> max
	// ....123.. --> 123
	// ....123m. --> 123
	uint64_t Channel::parse(uint64_t max, bool trim) {
		uint64_t number = 0;
		// Digit: 0 to 9 and the letter m (max).
		// Drop all characters until the first digit is found.
		uint8_t k = 0;
		while (true) {
			k = peek();
			if (k == 'm')
				break;
			else if (k < '0' || k > '9')
				read();
			else
				break;
		}
		
		if (k == 'm') {
			// Remove m from buffer.
			read();
			return max;
		} else {
			// Read all consecutive numeric characters.
			// 2^64: 9223372036854775807
			uint8_t buffer[20]{0};
			uint8_t pos = 0;
			bool bound = true;
			while (true) {
				k = peek();
				if (k >= '0' && k <= '9') {
					if (pos < 20)
						buffer[pos++] = k - '0';
					else
						bound = false;
					// Remove digit from buffer.
					read();
				} else {
					break;
				}
			}
			
			// Clamp the parsed number between 0 and max.
			if (bound) {
				uint64_t p10 = 1;
				while (pos > 0) {
					number += p10 * buffer[--pos];
					// Stop early if number is out of limits.
					if (number > max) {
						number = max;
						break;
					}
					p10 *= 10;
				}
			} else {
				number = max;
			}
			
			// Right trim if requested.
			if (trim)
				read();
			
			return number;
		}
	}
	
	uint64_t Channel::parse(uint64_t max) {
		return parse(max, true);
	}
	
	uint64_t Channel::next(uint8_t width) {
		uint8_t nbytes = ceil((width - nremainder) / 8.0);
        read(buffer + 1, nbytes);
		uint8_t last = buffer[nbytes];
		shift(buffer, nbytes + 1, nremainder - 8);
		uint64_t aligned = pack(buffer, nbytes + 1);
        nremainder = modcom(width - nremainder, 8);
		buffer[0] = last & rmask(nremainder);
		return decompress(aligned, 8 * (nbytes + 1), width);
	}

	void Channel::next() {
		buffer[0] = 0;
		nremainder = 0;
	}

	// Remainder is on the far right of the returned octect.
	uint8_t Channel::shift(uint8_t* bytes, uint8_t nbytes, int8_t s) {
		uint8_t remainder = 0;
        if (s > 0) {
            for (int b = 0; b < nbytes; b++) {
                uint8_t byte = bytes[b];
                bytes[b] = (bytes[b] >> s) | remainder;
                remainder = byte << (8 - s);
            }
        } else if (s < 0) {
            s *= -1;
            for (int b = nbytes - 1; b >= 0; b--) {
                uint8_t byte = bytes[b];
                bytes[b] = bytes[b] << s | remainder;
                remainder = byte >> (8 - s);
            }
        }
		return remainder;
	}

	// modcom( 5, 8) --> 3
	// modcom(16, 8) --> 0
	// Complement of modulus after division, by the same divisor.
	inline uint8_t Channel::modcom(int8_t x, int8_t k) {
	    return (k - (x % k)) % k;
	}

	// Convert an array of bytes to a 64 bit number.
	uint64_t Channel::pack(uint8_t* bytes, uint8_t nbytes) {
		// Combine bytes into one number.
		uint64_t number = 0;
		for (uint8_t b = 0; b < nbytes; b++)
			number |= (uint64_t) bytes[b] << 8 * (nbytes - 1 - b);
		return number;
	}

	uint64_t Channel::decompress(uint64_t number, uint8_t offset, uint8_t width) {
		number = (number >> offset - width) & rmask(width);
		return number;
	}

	uint64_t Channel::rmask(uint8_t width) {
	    // The mask will be constructed correctly in both 32- and 64-bit systems as long as the width is less than 32 and 64, respectively.
		uint64_t mask = 0;
		for (uint8_t w = 0; w < width; w++)
			mask = (mask << 1) | 1;
        return mask;
	}
}
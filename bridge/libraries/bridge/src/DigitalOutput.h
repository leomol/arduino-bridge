/**
 * @brief Direct port manipulation of a digital output.
 * 
 * @file DigitalOutput.h
 * @author Leonardo Molina (leonardomt@gmail.com)
 * @date 2016-12-03
 * @version: 0.1.180720
**/
 
#ifndef BRIDGE_DIGITALOUTPUT_H
#define BRIDGE_DIGITALOUTPUT_H

#include <Arduino.h>
#include "tools.h"

namespace bridge {
	class DigitalOutput {
		public:
			DigitalOutput(int8_t pin);
			void Set(bool state);
			void Toggle();
			
		private:
			bool state;
			volatile BRIDGE_IO_REG_TYPE* port;			///< Hardware address of the pin.
			BRIDGE_IO_REG_TYPE mask;						///< Mask to single out in the hardware address.
	};
}

#endif
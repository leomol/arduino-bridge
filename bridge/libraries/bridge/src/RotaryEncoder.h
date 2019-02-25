/*
	Created on: 2016-12-01
	
	@author Leonardo Molina (leonardomt@gmail.com).
	@version 1.1.180720
*/

#ifndef BRIDGE_ROTARYENCODER_H
#define BRIDGE_ROTARYENCODER_H

#include <stdint.h>
#include "Stepper.h"
#include "tools.h"

namespace bridge {
	class RotaryEncoder : public Stepper {
		public:
			typedef uintptr_t Data; 
			typedef void (*Function) (RotaryEncoder* rotaryEncoder, int32_t change);
			typedef void (*FunctionData) (RotaryEncoder* rotaryEncoder, int32_t change, Data data);
			RotaryEncoder() {};
			RotaryEncoder(int8_t pin1, int8_t pin2, Function function) : RotaryEncoder(pin1, pin2, function, nullptr, 0) {};
			RotaryEncoder(int8_t pin1, int8_t pin2, FunctionData functionData, Data data) : RotaryEncoder(pin1, pin2, nullptr, functionData, data) {};
			void Step() override;
			int8_t GetPin1();
			int8_t GetPin2();
			int32_t GetValue();
			~RotaryEncoder();
			
		private:
			RotaryEncoder(int8_t pin1, int8_t pin2, Function function, FunctionData functionData, int32_t data);
			
			volatile BRIDGE_IO_REG_TYPE* port1;			///< Hardware address of pin1.
			volatile BRIDGE_IO_REG_TYPE* port2;			///< Hardware address of pin2.
			BRIDGE_IO_REG_TYPE mask1;						///< Mask to single out pin1 in the hardware address.
			BRIDGE_IO_REG_TYPE mask2;						///< Mask to single out pin2 in the hardware address.
			
			Function function;
			FunctionData functionData;
			Data data;
			int8_t pin1;
			int8_t pin2;
			static void OnRise(Data data);
			void OnRise();
			bool interruptible;
			bool lastPin1State;				// Current contact state.
			int32_t asyncValue;
			int32_t syncValue;
	};
}

#endif
#include <stdint.h>

#include "Stepper.h"

namespace bridge {
	class Debounce : public Stepper {
		public:
			typedef uintptr_t Data;
			typedef void (*Function) (Debounce* debounce, int32_t change);
			typedef void (*FunctionData) (Debounce* debounce, int32_t change, Data data);
			
			Debounce() {};
			Debounce(int32_t value, uint32_t debounceIncrease, uint32_t debounceDecrease, Function function) : Debounce(value, debounceIncrease, debounceDecrease, function, nullptr, 0) {};
			Debounce(int32_t value, uint32_t debounceIncrease, uint32_t debounceDecrease, FunctionData functionData, Data data) : Debounce(value, debounceIncrease, debounceDecrease, nullptr, functionData, data) {};
			void Set(int32_t value);
			void Step() override;
			int32_t GetValue();
		private:
			Debounce(int32_t value, uint32_t debounceIncrease, uint32_t debounceDecrease, Function function, FunctionData functionData, Data data);
			Function function;
			FunctionData functionData;
			Data data;
			bool changed;
			int32_t debouncedValue;			// Debounced state.
			int32_t debouncingValue;		// Debounce state.
			uint32_t debounceIncrease;		// Debounce duration from low to high, adjusted to deadline.
			uint32_t debounceDecrease;		// Debounce duration from high to low, adjusted to deadline.
			uint32_t debounceNext;			// Ticker for debounce control.
	};
}
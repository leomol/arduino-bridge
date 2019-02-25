#include "Debounce.h"

namespace bridge {
	Debounce::Debounce(int32_t value, uint32_t debounceIncrease, uint32_t debounceDecrease, Function function, FunctionData functionData, Data data) :
	changed(false),
	debouncedValue(value),
	debouncingValue(value),
	debounceIncrease(debounceIncrease),
	debounceDecrease(debounceDecrease),
	debounceNext(0),
	
	function(function),
	functionData(functionData),
	data(data)
	{}
	
	void Debounce::Set(int32_t value) {
		if (value > debouncedValue) {
			changed = true;
			debouncingValue = value;
			debounceNext = micros() + debounceIncrease;
		} else if (value < debouncedValue) {
			changed = true;
			debouncingValue = value;
			debounceNext = micros() + debounceDecrease;
		}
	}
	
	void Debounce::Step() {
		if (changed && micros() >= debounceNext) {
			changed = false;
			// Value debounced.
			debouncedValue = debouncingValue;
			if (function)
				function(this, debouncedValue);
			else
				functionData(this, debouncedValue, data);
		}
	}
}
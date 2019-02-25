/**
 * @file TouchSensor.h
 * @author Leonardo Molina (leonardomt@gmail.com).
 * @date 2016-12-03
 * @version 1.1.180927
 * 
 * @brief Setup a touch sensor and report detected changes.
 * The sensor consists of a resistor with large value of resistance (e.g. 20MOhms) connected from the signal pin
 * to the sensor pin, which is connected to a conductive surface (e.g. a screw, an aluminum sheet, water) using a
 * shielded wire. Rule of thumb for the parameter periods: start with 10 and increase by 5 for every added sensor.
 * The first call(s) to Step() performs a calibration in which a baseline signal is read from the hardware setup.
 * Principle of operation:
 *   Detection occurs by listening to capacitive disturbances of a square-wave signal sent from a signal pin to a
 *   sensor pin. Two assumptions are made:
 *   1) During start (first call to the method Step()) the sensor is in the untouched state.
 *   2) The intrinsic capacitance of the sensor does not change over time.
 
**/

#ifndef BRIDGE_TOUCHSENSOR_H
#define BRIDGE_TOUCHSENSOR_H

#include <stdint.h>
#include "Stepper.h"
#include "tools.h"

namespace bridge {
	class TouchSensor : public Stepper {
		public:
			/// Type of data to include in the callback.
			typedef uintptr_t Data;
			/// Type of function to call when a result is available.
			typedef void (*Function) (TouchSensor* touchSensor, bool state);
			/// Type of function to call when a result is available, which includes user data.
			typedef void (*FunctionData) (TouchSensor* touchSensor, bool state, Data data);
			
			TouchSensor() {};
			
			/** @brief Setup a touch sensor and report detected changes.
			 *  @param[in] sensorPin Input pin detecting capacitive changes.
			 *  @param[in] signalPin Input pin connected to sensorPin with a large resistor.
			 *  @param[in] nPeriods Number of periods to test; higher values result in increased accuracy at the expense of performance.
			 *  @param[in] threshold Separation between baseline and signal; lower values result in higher false positives, higher values result in less true positives.
			 *  @param[in] debounceRise Duration before accepting a change from no-contact to contact.
			 *  @param[in] debounceFall Duration before accepting a change from contact to no-contact.
			 *  @param[in] function Function to call when a result is available.
			 *  @param[in] data User data to include in the callback when a result is available.
			 */
			TouchSensor(int8_t sensorPin, int8_t signalPin, uint8_t nPeriods, uint8_t threshold, uint32_t debounceRise, uint32_t debounceFall, FunctionData functionData, Data data) : TouchSensor(sensorPin, signalPin, nPeriods, threshold, debounceRise, debounceFall, nullptr, functionData, data) {};
			
			/** @brief Setup a touch sensor and report detected changes. Debouncing mechanism is disabled.
			 *  @param[in] sensorPin Input pin detecting capacitive changes.
			 *  @param[in] signalPin Input pin connected to sensorPin with a large resistor.
			 *  @param[in] nPeriods Number of periods to test; higher values result in increased accuracy at the expense of performance.
			 *  @param[in] threshold Separation between baseline and signal; lower values result in higher false positives, higher values result in less true positives.
			 *  @param[in] function Function to call when a result is available.
			 *  @param[in] data User data to include in the callback when a result is available.
			 */
			TouchSensor(int8_t sensorPin, int8_t signalPin, uint8_t nPeriods, uint8_t threshold, FunctionData functionData, Data data) : TouchSensor(sensorPin, signalPin, nPeriods, threshold, 0, 0, nullptr, functionData, data) {};
			
			/** @brief Setup a touch sensor and report detected changes.
			 *  @param[in] sensorPin Input pin detecting capacitive changes.
			 *  @param[in] signalPin Input pin connected to sensorPin with a large resistor.
			 *  @param[in] nPeriods Number of periods to test; higher values result in increased accuracy at the expense of performance.
			 *  @param[in] threshold Separation between baseline and signal; lower values result in higher false positives, higher values result in less true positives.
			 *  @param[in] debounceRise Duration before accepting a change from no-contact to contact.
			 *  @param[in] debounceFall Duration before accepting a change from contact to no-contact.
			 *  @param[in] function Function to call when a result is available.
			 */
			TouchSensor(int8_t sensorPin, int8_t signalPin, uint8_t nPeriods, uint8_t threshold, uint32_t debounceRise, uint32_t debounceFall, Function function = nullptr) : TouchSensor(sensorPin, signalPin, nPeriods, threshold, debounceRise, debounceFall, function, nullptr, 0) {};
			
			
			/** @brief Setup a touch sensor and report detected changes. Debouncing mechanism is disabled.
			 *  @param[in] sensorPin Input pin detecting capacitive changes.
			 *  @param[in] signalPin Input pin connected to sensorPin with a large resistor.
			 *  @param[in] nPeriods Number of periods to test; higher values result in increased accuracy at the expense of performance.
			 *  @param[in] threshold Separation between baseline and signal; lower values result in higher false positives, higher values result in less true positives.
			 *  @param[in] function Function to call when a result is available.
			 */
			TouchSensor(int8_t sensorPin, int8_t signalPin, uint8_t nPeriods, uint8_t threshold, Function function) : TouchSensor(sensorPin, signalPin, nPeriods, threshold, 0, 0, function, nullptr, 0) {};
			
			/**
			 * @brief Test and report state changes, if any.
			 * @return void
			 */
			void Step() override;
			
			/// @return Sensor pin number.
			int8_t GetSensorPin();
			
			/// @return Signal pin number (pin connected to sensorPin with a large resistor).
			int8_t GetSignalPin();
			
			/// @return Touch state detected.
			bool GetState();
			
		private:
			/// @brief Generic constructor. Public constructors delegate to this one by setting the parameters function or data to 0 or nullptr, respectively.
			TouchSensor(int8_t sensorPin, int8_t signalPin, uint8_t nPeriods, uint8_t threshold, uint32_t debounceRise, uint32_t debounceFall, Function function, FunctionData functionData, Data data);
			
			int8_t sensorPin;					///< Input pin detecting capacitive changes.
			int8_t signalPin;					///< Input pin connected to sensorPin with a large resistor.
		
			Function function;					///< User provided data-less function.
			FunctionData functionData;			///< User provided function with data.
			Data data;							///< User provided data.
			
			volatile BRIDGE_IO_REG_TYPE* rPort;	///< Hardware address of pin1.
			volatile BRIDGE_IO_REG_TYPE* sPort;	///< Hardware address of pin2.
			BRIDGE_IO_REG_TYPE rMask;			///< Mask to single out pin1 in the hardware address.
			BRIDGE_IO_REG_TYPE sMask;			///< Mask to single out pin2 in the hardware address.
			
			uint8_t setup;						///< Flag initialization period.
			uint32_t deadline;					///< Calibrated deadline; capacitive disturbance before this time yields a HIGH state.
			
			bool state;							///< Current contact state.
			bool debouncingState;				///< State under test.
			uint32_t debounceRise;				///< Debounce duration from low to high, adjusted to deadline.
			uint32_t debounceFall;				///< Debounce duration from high to low, adjusted to deadline.
			uint32_t debounceNext;				///< Ticker for debounce control.
			uint32_t start;						///< Time at which test pulse train started.
			uint8_t nPeriods;					///< Max number of periods to test.
			uint8_t threshold;					///< Percentage threshold to split contact to no-contact (0 --> 100%, 255 --> 355%)
			
			bool Test(bool &state);				//   Asynchronous test for state changes.
			bool phase;							//   Current phase, when running asynchronous tests.
			uint8_t period;						//   Current period, when running asynchronous tests.
			bool send;							//   Whether a pulse has been sent, when running asynchronous tests.
	};
}

#endif
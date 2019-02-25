/**
	
	Creates a link between an Arduino board and a PC to read and write on digital and analog pins, defined during runtime.
	Commands permit scheduling of trains of pulses (i.e. pulse waves) and listening to changes in single inputs (digital or analog) or compound inputs (e.g. rotary encoders). Commands and outputs are compressed to save bandwidth during serial communication. Debug mode is available to execute commands and print data in plain text to aid in the maintenance of the libraries. Maximum pin number is 125.
	
	# Brief command description
	
		## Set address
			Set the microcontroller's address to a value.
		
		## Set binary
			Change the value of a pin to either binary state.
			
		## Set pulse
			Output a rectangular waveform for a number of repetitions.
			
		## Set chirp
			Output a waveform with a frequency that changes linearly over time.
			
		## Set PWM driver frequency (24 to 1024 Hz)
			Instructs Adafruit's 16-Channel 12-bit PWM driver (PCA9685) to oscillate at the given frequency.
			
		## Set PWM driver pulse duration of a channel (0 to 4095)
			Instructs Adafruit's 16-Channel 12-bit PWM driver (PCA9685) to update the duration of the pulse:
				high_s = high / 4095 / frequency
				 low_s = (4095 - high) / frequency
			
		## Stop set
			Stop the output signal at the given pin.
			
		## Get binary
			Listen to a pin for LOW or HIGH changes.
		
		## Get level
			Listen to an analog pin for changes in the range 0 to 255.
		
		## Get rotation
			Listen to changes in rotation produced by a rotary encoder in two pins.
			
		## Stop get
			Stop listening to inputs in the given pin.
	
	# Brief parameter description
	
		## debounce
			Duration a state must be held in order to be read as a change.
			
		## factor
			Reduce reports by this factor.
			
		## state
			LOW or HIGH represented by 0 or 1 respectively.
			
	The communication protocols for both debug and raw modes are layed out below.
	
	# Debug mode
		## Summary
			Parameters are communicated in plain text (JSON-like format) for debugging purposes, for which the bandwidth usage is much higher than in raw mode.
		
		## Inputs (data sent to Arduino)
			Each command consists of a name and one or more numeric parameters separated by spaces. For example:
			
				<command-name> <parameter1> <parameter2> ... <parameterN>
			
			### set-address
			
				a <address> <value>
				
			### set-binary
			
				b <pin> <state>
				
			### set-pulse
			
				p <pin> <state-start> <duration-low> <duration-high> <repetitions>
				
			### set-chirp
			
				c <pin> <duration-low-start> <duration-low-stop> <duration-high-start> <duration-high-stop> <duration>
				
			Zero repetitions means infinite
				
			### Set PWM driver frequency
				
				q <frequency>
				
			### Set PWM driver pulse duration of a channel
				
				w <channel> <duration>
			
			### stop-set
			
				s <pin>
				
			### get-binary
			
				B <pin> <debounce-rise> <debounce-fall> <factor>
				
			### get-level
			
				L <pin> <factor>
				
			### get-rotation
			
				R <active-pin> <passive-pin> <factor>
				
			### stop-get
			
				S <pin>
		
		## Outputs (data sent from Arduino)
			Data consists of two pin-value pairs (pin:\<pin\>,value:\<value\>); the first one is the pin number and the second is a value which varies in meaning according to the command assigned to that pin:
			
			### get-binary
				Number of times the pin was on low (negative number) or high (positive number).
			
			### get-level
				Value of the analog pin.
			
			### get-rotation
				Positive or negative step of the rotation encoder.

	# Raw mode
		## Summary
			Parameters are compressed into a fixed number of bits to save bandwidth.
			
			Reports and commands _set-binary_ and _stop_ encode in one byte a pin number and an operand (pin*operand), where the operand varies in meaning according to the context:
				| Pin*Operand |    Pin   |  Operand |         Meaning        |
				|:-----------:|:--------:|:--------:|:----------------------:|
				|  000 to 126 | 0 to 126 |    0     | low, set, or negative  |
				|  127 to 253 | 0 to 126 |    1     | high, get, or positive |

		## Inputs (data sent to Arduino):
			The signature for each command is given below where each parameter is assigned a fixed number of bits, for example:
			### test
				| Number of bits |           Description           |
				|:--------------:|:-------------------------------:|
				|       07       | param1                          |
				|       16       | param2                          |
			This means that test takes two parameters; the first one ranges from 0 to 127 (in 7 bits) and the second from 0 to 65535 (in 16 bits).
				
			### set-binary
				| Number of bits |           Description 
				|:--------------:|:-------------------------------:|
				|       08       | pin*operand\n0: LOW 1: HIGH     |
				
			### set-address
				| Number of bits |           Description 
				|:--------------:|:-------------------------------:|
				|       08       | entry-key: 11111110             |
				|       08       | address                         |
				|       08       | value                           |
				
			### stop-set
				| Number of bits |           Description 
				|:--------------:|:-------------------------------:|
				|       13       | entry-key: 11111111 11110       |
				|       07       | pin                             |
				
			### stop-get
				| Number of bits |           Description 
				|:--------------:|:-------------------------------:|
				|       13       | entry-key: 11111111 11111       |
				|       07       | pin                             |
				
			### set-pulse
				| Number of bits |           Description 
				|:--------------:|:-------------------------------:|
				|       12       | entry-key: 11111111 0000        |
				|       07       | pin                             |
				|       01       | state-start                     |
				|       24       | duration-low                    |
				|       24       | duration-high                   |
				|       24       | repetitions                     |
				
			### set-chirp
				| Number of bits |           Description 
				|:--------------:|:-------------------------------:|
				|       12       | entry-key: 11111111 0001        |
				|       07       | pin                             |
				|       24       | duration-low-start              |
				|       24       | duration-low-stop               |
				|       24       | duration-high-start             |
				|       24       | duration-high-stop              |
				|       24       | duration                        |
				
			### set-PWM-driver-frequency
				| Number of bits |           Description 
				|:--------------:|:-------------------------------:|
				|       12       | entry-key: 11111111 0100        |
				|       10       | frequency                       |
				
			### set-PWM-duration
				| Number of bits |           Description 
				|:--------------:|:-------------------------------:|
				|       12       | entry-key: 11111111 0101        |
				|       04       | channel                         |
				|       12       | fall-tic                        |
				
			### get-binary
				| Number of bits |           Description 
				|:--------------:|:-------------------------------:|
				|       12       | entry-key: 11111111 1000        |
				|       07       | pin                             |
				|       24       | debounce-rise                   |
				|       24       | debounce-fall                   |
				|       24       | factor                          |
				
			### get-level
				| Number of bits |           Description 
				|:--------------:|:-------------------------------:|
				|       12       | entry-key: 11111111 1001        |
				|       07       | pin                             |
				|       07       | factor                          |
				
			### get-rotation
				| Number of bits |           Description 
				|:--------------:|:-------------------------------:|
				|       12       | entry-key: 11111111 1010        |
				|       07       | active-pin                      |
				|       07       | passive-pin                     |
				|       07       | factor                          |
			
		## Outputs (data sent from Arduino):
			Data consist of 1 byte encoding the pin number and the direction of change using the pin*operand definition described above. When get-level is setup, several bytes will be sent to catch-up with the current value.
		
	# Considerations
		## PWM Driver
			- Driver uses D20 and D21 for communication. Grounding D21 may cause the device to freeze.
		## Microcontroller
			- board.cpp targets an Arduino Mega 2560; behavior implemented or assumed for _timer1_ and _interrupts_ may differ on other boards.
		## Development
			- Most C++ libraries are not available in embedded systems like the Arduino, consequently _vector_ and _iostream_ could not be used.
			- SRAM available is 8KBs.
		## Debugging
			- Serial communication interferes with timer events, hence pulse width modulation is coarse in debug mode where bandwidth use is high.
		## Register map for Atmega1280
			http://www.atmel.com/Images/Atmel-2549-8-bit-AVR-Microcontroller-ATmega640-1280-1281-2560-2561_datasheet.pdf
	
	Created on: 2016-12-01
	
	\author Leonardo Molina (leonardomt@gmail.com).
	\version 1.1.180406
*/

#include "Arduino.h"
#include "WString.h"
#include "HardwareSerial.h"
#include "Adafruit_PWMServoDriver.h"

#include "Bridge.h"
#include "Channel.h"
#include "GetBinary.h"
#include "GetRotation.h"
#include "GetContact.h"
#include "GetLevel.h"
#include "GetThreshold.h"
#include "LinkedIndex.h"
#include "SetBinary.h"
#include "SetChirp.h"
#include "SetPulse.h"

#include "meta.h"
#include "types.h"

using PWMDriver = Adafruit_PWMServoDriver;
namespace bridge {
	LinkedIndex<Routine*> Bridge::setters(nHid);
	LinkedIndex<Routine*> Bridge::getters(nHid);
	Bridge* Bridge::instance;
	HardwareSerial* Bridge::serial;
	Bridge::Status Bridge::status;
	ReportFunction Bridge::reportFunction;
	uint32_t Bridge::baudrate;
	uint8_t Bridge::tonePin;
	PWMDriver Bridge::pwmDriver = PWMDriver();

	static volatile uint64_t tic = 0;
	
	Bridge::Bridge() {
	}
	
	Bridge::Bridge(HardwareSerial* serial, uint32_t baudrate) : pwmSetup(true) {
		Bridge::instance = this;
		Bridge::serial = serial;
		Bridge::baudrate = baudrate;
		Bridge::reportFunction = reportText;
		Bridge::status = Status::disabled;
		
		Bridge::tonePin = 13;
		
		// Start communication.
		serial->begin(baudrate);
		channel = Channel(serial);
		handshake();
		serial->flush();
		// All data received by serialEvent up to this point was discarded.
		Bridge::status = Status::handshake;
		
		// // Testing.
		// blink(13, 50, 10);
		// status = Status::debug;
		// reportFunction = reportText;
	}
	
	void Bridge::Step() {
		//Serial.println("x");
		tic = micros();
		
		// Read serial and report state of getters.
		instance->read();
		setterRoutine();
		getterRoutine();
		// Report state of getters.
		Routine* routine;
		getters.begin();
		while (getters.next(routine))
			routine->report(reportFunction);
	}
	
	void Bridge::handshake() {
		serial->write(255);
		serial->write(255);
		serial->write(255);
	}
	
	void Bridge::read() {
		while (serial->available()) {
			if (status == Status::debug) {
				char header = channel.read();
				if (header == 'a') {
					// Change microcontroller's address.
					uint8_t address = channel.parse(255);
					uint8_t value   = channel.parse(255);
					serial->print(String() + "set-address:{address:" + (int) address + ",value:" + (int) value + "}\n");
					*reinterpret_cast<volatile uint8_t*>(address) = value;
				} else if (header == 'b') {
					// Set state of the pin to a fixed value.
					uint8_t hid   = channel.parse(nHid);
					uint8_t state = channel.parse( 1);
					removeSetter(hid);
					SetBinary(hid, state);
					serial->print(String() + "set-binary:{pin:" + hid + ",state:" + state + "}\n");
				} else if (header == 'p') {
					uint8_t hid           = channel.parse(nHid);
					bool stateStart       = channel.parse( 1);
					uint32_t durationLow  = channel.parse(-1);
					uint32_t durationHigh = channel.parse(-1);
					uint32_t repetitions  = channel.parse(-1);
					removeSetter(hid);
					setters.set(hid, new SetPulse(hid, stateStart, durationLow, durationHigh, repetitions));
					serial->print(String() + "set-pulse:{pin:" + hid + ",state-start:" + stateStart + ",duration-low:" + durationLow + ",duration-high:" + durationHigh + ",repetitions:" + ((repetitions == 0) ? "infinite" : String() + repetitions) + "}\n");
				} else if (header == 'c') {
					uint8_t hid                = channel.parse(nHid);
					uint32_t durationLowStart  = channel.parse(-1);
					uint32_t durationLowStop   = channel.parse(-1);
					uint32_t durationHighStart = channel.parse(-1);
					uint32_t durationHighStop  = channel.parse(-1);
					uint32_t duration          = channel.parse(-1);
					removeSetter(hid);
					setters.set(hid, new SetChirp(hid, durationLowStart, durationLowStop, durationHighStart, durationHighStop, duration));
					serial->print(String() + "set-chirp:{pin:" + hid + ",duration-low-start:" + durationLowStart + ",duration-low-stop:" + durationLowStop + ",duration-high-start:" + durationHighStart + ",duration-high-stop:" + durationHighStop + ",duration:" + duration + "}\n");
				} else if (header == 'q') {
					uint32_t frequency = channel.parse(-1);
					frequency = max(frequency, 24);
					SetupPWM();
					pwmDriver.setPWMFreq(frequency);
					serial->print(String() + "set-driver-frequency:{frequency:" + frequency + "}\n");
				} else if (header == 'w') {
					uint8_t hid       = channel.parse(16);
					uint16_t duration = channel.parse(-1);
					SetPWM(hid, duration);
					serial->print(String() + "set-driver-duration:{channel:" + hid + ",duration:" + duration + "}\n");
				} else if (header == 's') {
					uint8_t hid = channel.parse(nHid);
					removeSetter(hid);
					serial->print(String() + "stop-set:{pin:" + hid + "}\n");
				} else if (header == 't') {
					uint8_t hid        = channel.parse(nHid);
					uint32_t frequency = channel.parse(-1);
					uint32_t duration  = channel.parse(-1);
					noTone(tonePin);
					tonePin = hid;
					uint32_t durationMs = round(0.001L * duration);
					if (frequency > 0 && durationMs > 0)
						tone(hid, frequency, durationMs);
					serial->print(String() + "set-tone:{pin:" + hid + ",frequency:" + frequency + ",duration:" + duration + "}\n");
				} else if (header == 'B') {
					uint8_t hid           = channel.parse(nHid);
					uint32_t debounceRise = channel.parse(-1);
					uint32_t debounceFall = channel.parse(-1);
					uint8_t factor        = channel.parse(255);
					removeGetter(hid);
					getters.set(hid, new GetBinary(hid, debounceRise, debounceFall, max(factor, 1)));
					//!! board::attachInterrupt(hid, changeCallback, CHANGE);
					int8_t it = digitalPinToInterrupt(hid);
					attachInterrupt(it, meta::Wrap(changeCallback, hid), CHANGE);
					serial->print(String() + "get-binary:{pin:" + hid + ",debounce-rise:" + debounceRise + ",debounce-fall:" + debounceFall + ",factor:" + factor + "}\n");
				} else if (header == 'C') {
					uint8_t hid0          = channel.parse(nHid);
					uint8_t hid1          = channel.parse(nHid);
					uint8_t samples       = channel.parse(255);
					uint8_t snr           = channel.parse(255);
					uint32_t debounceRise = channel.parse(-1);
					uint32_t debounceFall = channel.parse(-1);
					removeGetter(hid0);
					removeGetter(hid1);
					getters.set(hid0, new GetContact(hid0, hid1, samples, snr, debounceRise, debounceFall));
					serial->print(String() + "get-contact:{pins:[" + hid0 + "," + hid1 + "]" + ",samples:" + samples + ",SNR:" + snr + ",debounce-rise:" + debounceRise + ",debounce-fall:" + debounceFall + "}\n");
				} else if (header == 'L') {
					uint8_t hid           = channel.parse(nHid);
					uint32_t debounceRise = channel.parse(-1);
					uint32_t debounceFall = channel.parse(-1);
					removeGetter(hid);
					getters.set(hid, new GetLevel(hid, debounceRise, debounceFall));
					serial->print(String() + "get-level:{pin:" + hid + ",debounce-rise:" + debounceRise + ",debounce-fall:" + debounceFall + "}\n");
				} else if (header == 'R') {
					uint8_t hid0   = channel.parse(nHid);
					uint8_t hid1   = channel.parse(nHid);
					uint8_t factor = channel.parse(255);
					removeGetter(hid0);
					removeGetter(hid1);
					getters.set(hid0, new GetRotation(hid0, hid1, max(factor, 1)));
					//!! board::attachInterrupt(hid0, risingCallback, RISING);
					int8_t it = digitalPinToInterrupt(hid0);
					attachInterrupt(it, meta::Wrap(risingCallback, hid0), RISING);
					serial->print(String() + "get-rotation:{pins:[" + hid0 + "," + hid1 + "]" + ",factor:" + factor + "}\n");
				} else if (header == 'S') {
					uint8_t hid = channel.parse(nHid);
					removeGetter(hid);
					serial->print(String() + "stop-get:{pin:" + hid + "}\n");
				} else if (header == 'T') {
					uint8_t hid           = channel.parse(nHid);
					uint8_t threshold     = channel.parse(255);
					uint32_t debounceRise = channel.parse(-1);
					uint32_t debounceFall = channel.parse(-1);
					removeGetter(hid);
					getters.set(hid, new GetThreshold(hid, threshold, debounceRise, debounceFall));
					serial->print(String() + "get-threshold:{pin:" + hid + ",threshold:" + threshold + ",debounce-rise:" + debounceRise + ",debounce-fall:" + debounceFall + "}\n");
				}
			} else if (status == Status::raw) {
				uint8_t key = channel.read();
				if (key < 254) {
					// set-binary.
					uint8_t hid;
					bool state;
					if (decodeState(key, hid, state)) {
						removeSetter(hid);
						SetBinary(hid, state);
					}
				} else if (key == 254) {
					// set-address.
					uint8_t address = channel.read();
					uint8_t value = channel.read();
					*reinterpret_cast<volatile uint8_t*>(address) = value;
				} else if (key == 255) {
					// Key continues with 1 more byte.
					key = channel.read();
					// Reset bit carret.
					channel.next();
					if (key == 0) {
						// stop.
						uint8_t hid = channel.next( 7);
						bool choice	= channel.next( 1);
						if (choice)
							removeGetter(hid);
						else
							removeSetter(hid);
					} else if (key == 1) {
						// set-pulse.
						uint8_t hid           = channel.next( 7);
						bool stateStart       = channel.next( 1);
						uint64_t durationLow  = channel.next(24);
						uint64_t durationHigh = channel.next(24);
						uint64_t repetitions  = channel.next(24);
						removeSetter(hid);
						setters.set(hid, new SetPulse(hid, stateStart, durationLow, durationHigh, repetitions));
					} else if (key == 2) {
						// set-chirp.
						uint8_t hid                = channel.next( 8);
						uint64_t durationLowStart  = channel.next(24);
						uint64_t durationLowStop   = channel.next(24);
						uint64_t durationHighStart = channel.next(24);
						uint64_t durationHighStop  = channel.next(24);
						uint64_t duration          = channel.next(24);
						removeSetter(hid);
						setters.set(hid, new SetChirp(hid, durationLowStart, durationLowStop, durationHighStart, durationHighStop, duration));
					} else if (key == 3) {
						uint16_t frequency = channel.next(16);
						// set-pwm frequency.
						frequency = max(frequency, 24);
						SetupPWM();
						pwmDriver.setPWMFreq(frequency);
					} else if (key == 4) {
						// set-pwm value.
						uint8_t hid       = channel.next( 4);
						uint16_t duration = channel.next(12);
						SetPWM(hid, duration);
					} else if (key == 5) {
						// Set tone.
						uint8_t hid        = channel.next( 8);
						uint32_t frequency = channel.next(16);
						uint32_t duration  = channel.next(24);
						noTone(tonePin);
						tonePin = hid;
						uint32_t durationMs = round(0.001L * duration);
						if (frequency > 0 && durationMs > 0)
							tone(hid, frequency, durationMs);
					} else if (key == 255) {
						// get-binary.
						uint8_t hid           = channel.next( 8);
						uint64_t debounceRise = channel.next(24);
						uint64_t debounceFall = channel.next(24);
						uint8_t factor        = channel.next( 8);
						removeGetter(hid);
						getters.set(hid, new GetBinary(hid, debounceRise, debounceFall, max(factor, 1)));
						//!! board::attachInterrupt(hid, changeCallback, CHANGE);
						int8_t it = digitalPinToInterrupt(hid);
						attachInterrupt(it, meta::Wrap(changeCallback, hid), CHANGE);
					} else if (key == 254) {
						// get-contact.
						uint8_t hid0          = channel.next( 8);
						uint8_t hid1          = channel.next( 8);
						uint8_t samples       = channel.next( 8);
						uint8_t snr           = channel.next( 8);
						uint32_t debounceRise = channel.next(24);
						uint32_t debounceFall = channel.next(24);
						removeGetter(hid0);
						removeGetter(hid1);
						getters.set(hid0, new GetContact(hid0, hid1, samples, snr, debounceRise, debounceFall));
					} else if (key == 253) {
						// get-level.
						uint8_t hid           = channel.next( 8);
						uint64_t debounceRise = channel.next(24);
						uint64_t debounceFall = channel.next(24);
						removeGetter(hid);
						getters.set(hid, new GetLevel(hid, debounceRise, debounceFall));
					} else if (key == 252) {
						// get-rotation.
						uint8_t hid0   = channel.next( 8);
						uint8_t hid1   = channel.next( 8);
						uint8_t factor = channel.next( 8);
						removeGetter(hid0);
						removeGetter(hid1);
						getters.set(hid0, new GetRotation(hid0, hid1, max(factor, 1)));
						//!! board::attachInterrupt(hid0, risingCallback, RISING);
						int8_t it0 = digitalPinToInterrupt(hid0);
						attachInterrupt(it0, meta::Wrap(risingCallback, hid0), RISING);
					} else if (key == 251) {
						uint8_t hid           = channel.next(8);
						uint8_t threshold     = channel.next(8);
						uint32_t debounceRise = channel.next(24);
						uint32_t debounceFall = channel.next(24);
						removeGetter(hid);
						getters.set(hid, new GetThreshold(hid, threshold, debounceRise, debounceFall));
					}
				}
			} else if (status == Status::handshake) {
				switch (channel.read()) {
					case 'r':
						blink(13, 50, 10);
						status = Status::raw;
						reportFunction = reportRaw;
						// instance->handshake();
						break;
					case 'd':
						blink(13, 50, 10);
						status = Status::debug;
						reportFunction = reportText;
						// instance->handshake();
						break;
				}
			}
		}
	}
	
	void Bridge::removeSetter(int8_t hid) {
		// cli();
		Routine* routine;
		if (setters.get(hid, routine)) {
			setters.unset(hid);
			routine->stop();
			delete routine;
		}
		// sei();
	}
	
	void Bridge::removeGetter(int8_t hid) {
		// cli();
		//!! board::detachInterrupt(hid);
		detachInterrupt(digitalPinToInterrupt(hid));
		Routine* routine;
		if (getters.get(hid, routine)) {
			getters.unset(hid);
			routine->stop();
			delete routine;
		}
		// sei();
	}
	
	void Bridge::SetPWM(uint8_t hid, uint16_t duration) {
		SetupPWM();
		if (duration == 0)
			pwmDriver.setPWM(hid, 4096, 0);
		else
			pwmDriver.setPWM(hid, 0, duration);
	}
	
	void Bridge::SetupPWM() {
		if (pwmSetup) {
			pwmSetup = false;
			Bridge::pwmDriver.begin();
			Bridge::pwmDriver.setPWMFreq(60);
		}
	}
	
	void Bridge::blink(int8_t hid, uint16_t halfDuration, uint16_t repetitions) {
		uint64_t tics = 1000 * halfDuration;
		removeSetter(hid);
		setters.set(hid, new SetPulse(hid, 1, tics, tics, repetitions));
	}
	
	uint8_t Bridge::encodeState(uint8_t hid, bool state) {
		return state ? hid + 127 : hid;
	}
	
	bool Bridge::decodeState(uint8_t code, uint8_t &pin, bool &state) {
		if (code <= 126) {
			pin = code;
			state = false;
			return true;
		} else if (code <= 253) {
			pin = code - 127;
			state = true;
			return true;
		} else {
			return false;
		}
	}
	
	// Notify inputs of an iteration.
	void Bridge::getterRoutine() {
		Routine* routine;
		getters.begin();
		while (getters.next(routine)) {
			if (digitalPinToInterrupt(routine->index()) == -1)
				routine->step(tic);
		}
	}
	
	// Notify outputs of an iteration.
	void Bridge::setterRoutine() {
		Routine* routine;
		setters.begin();
		while (setters.next(routine))
			routine->step(tic);
	}
	
	void Bridge::risingCallback(uintptr_t hid) {
		Routine* routine;
		if (getters.get(hid, routine))
			routine->step(tic, 255);
	}
	
	void Bridge::changeCallback(uintptr_t hid) {
		Routine* routine;
		if (getters.get(hid, routine))
			routine->step(tic, 255);
	}
	
	void Bridge::reportText(int8_t hid, int32_t value, int32_t delta) {
		if (delta != 0) {
			serial->print(String() + "pin:" + hid + ",value:" + value + ",delta:" + delta + "\n");
		}
	}
	
	// Change reports consist of 1 byte indicating pin and direction of change.
	void Bridge::reportRaw(int8_t hid, int32_t value, int32_t delta) {
		// serial->println(delta);
		
		uint8_t state;
		if (delta > 0) {
			state = encodeState(hid, 1);
		} else if (delta < 0) {
			state = encodeState(hid, 0);
			delta = -delta;
		}
		for (int8_t v = 0; v < delta; v++) {
			serial->write(state);
		}
	}
}

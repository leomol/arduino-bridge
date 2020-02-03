/**
 * @brief This is a minimalistic version of the library Bridge. The protocol is compatible with Bridge to an extent:
 *    Only pulse, input, and contact commands at the given pins are processed.
 *    Pulse ignores all parameters except high-duration.
 *    Input and contact ignores all parameters.
 * @author Leonardo Molina (leonardomt@gmail.com).
 * @file BridgeForParser.ino
 * @date 2019-02-10
 * @version: 0.1.191005
*/

#include "DigitalInput.h"
#include "TouchSensor.h"
#include "Oscillator.h"

using namespace bridge;

const bool debug = false;
const uint8_t touchPeriods = 35;				///< Lick sensor - Number of periods to test.
const uint8_t touchThreshold = 35;				///< Lick sensor - Percentage from baseline for touch detection.
const  int8_t touchSensorPins[] = {42, 43};		///< Lick sensor - Lick sensor pins.
const  int8_t rewardPin = 8;					///< Reward - Reward pin.
const  int8_t framePin = 20; 					///< Frame trigger input.
bool frameRequested = false;
bool touchRequested = false;

enum class Status {
	disabled,	///< Setting up.
	handshake,	///< Waiting for handshake.
	raw = 255	///< Using raw mode as communication protocol.
};
Status status = Status::disabled;

/// Forward declaration.
void onFrame(DigitalInput* digitalInput, bool state);
void onTouchSensor(TouchSensor* touchSensor, bool state);

/// Digital input for frame inputs.
DigitalInput frame = DigitalInput(framePin, onFrame);

/// Oscillator for triggering rewards.
Oscillator oscillator = Oscillator();

/// Touch sensor for detecting licks at the feeder.
TouchSensor touchSensor = TouchSensor(touchSensorPins[0], touchSensorPins[1], touchPeriods, touchThreshold, onTouchSensor);

void setup() {
	Serial.begin(115200);
	status = Status::handshake;
	handshake();
	
	if (debug) {
		encodeState(framePin, frame.GetState());
		frameRequested = true;
	}
}

/// Greet party.	
void handshake() {
	if (debug) {
		Serial.println("!!!");
	} else {
		Serial.write(255);
		Serial.write(255);
		Serial.write(255);
	}
}

/// Arduino library loop: Update all steppers and read serial port.
void loop() {
	frame.Step();
	touchSensor.Step();
	oscillator.Step();
	
	/// Read Serial port. This is consistent with a larger protocol defined in the library Bridge.
	while (Serial.available()) {
		if (status == Status::raw) {
			uint8_t key = read();
			if (key == 255) {
				key = read();
				if (key == 1) {
					// set-pulse.
					drop(4);
					uint32_t duration = 0;
					duration |= read() << 16;
					duration |= read() <<  8;
					duration |= read();
					oscillator.Start(rewardPin, duration);
					drop(3);
				} else if (key == 255) {
					// get-binary.
					drop(8);
					encodeState(framePin, frame.GetState());
					frameRequested = true;
				} else if (key == 254) {
					// get-touch.
					drop(10);
					encodeState(touchSensorPins[0], touchSensor.GetState());
					touchRequested = true;
				}
			}
		} else if (status == Status::handshake) {
			switch (read()) {
				case 'r':
					status = Status::raw;
					break;
			}
		}
	}
}

/// Process a response from frame inputs.
void onFrame(DigitalInput* digitalInput, bool state) {
	if (frameRequested)
		encodeState(digitalInput->GetPin(), state);
}

/// Trigger a reward when available.
void onTouchSensor(TouchSensor* touchSensor, bool state) {
	if (touchRequested) {
		encodeState(touchSensor->GetSensorPin(), state);
	}
}

/// Encode pin in 7 bits and its state in 1 bit.
void encodeState(uint8_t hid, bool state) {
	if (debug)
		Serial.println(String() + hid + ":" + state);
	else
		Serial.write(state ? hid + 127 : hid);
}


/// Block-read 1 byte.
uint8_t read() {
	while (Serial.available() == 0) {}
	return Serial.read();
}

/// Distard n bytes from Serial port.
void drop(int n) {
	for (int i = 0; i < n; i++)
		read();
}
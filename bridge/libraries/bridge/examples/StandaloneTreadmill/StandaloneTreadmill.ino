/**
 * Setup an Arduino for a standalone treadmill task.
 * The treadmill consists of a rotary encoder, a photo sensor, and a belt with a reflective tape to detect reward
 * zones, also a pinch valve and a lick sensor for reward delivery.
 * Two modes of operation are available. Within three seconds after Arduino starts:
 *   -Mode 1 if a contact is not detected: A reward will be delivered every reward zone.
 *   -Mode 2 if a contact is detected: A reward will be enabled every reward zone but must be claimed within a time window.
 * 
 * @file StandaloneTreadmill.ino
 * @author Leonardo Molina (leonardomt@gmail.com)
 * @date 2016-12-03
 * @version: 0.1.180828
**/

#include "DigitalInput.h"
#include "Oscillator.h"
#include "RotaryEncoder.h"
#include "TouchSensor.h"

using namespace bridge;

const int32_t rewardDuration = 40000;			///< Reward - Duration (us) the pinch valve remains open for reward delivery.
const int32_t rewardTimeWindow = 2000000;		///< Reward - Time window (us) to claim a reward.
const int32_t toneDuration = 100000;			///< Reward - Duration (us) of the reward tone.
const int32_t toneFrequency = 2750000;			///< Reward - Frequency (uHz) of the reward tone.
const  int8_t tonePin = 53;						///< Reward - Speaker pin.
const  int8_t rewardPin = 8;					///< Reward - Reward pin.
const  int8_t rewardForwardPin = 31;			///< Reward - Reward forward pin.

const  int8_t encoderPins[] = {2, 4};			///< Belt - Rotary encoder pins.
const  int8_t framePin = 20;					///< Belt - Frame trigger input.
const  int8_t photoSensorPin = 3;				///< Belt - Photo sensor pin.
const  int8_t photoSensorForwardPin = 24;		///< Belt - Photo sensor forward pin.

const  int8_t touchForwardPin = LED_BUILTIN;	///< Lick sensor - Forward lick state to this pin.
const uint8_t touchPeriods = 35;				///< Lick sensor - Number of periods to test.
const uint8_t touchThreshold = 35;				///< Lick sensor - Percentage from baseline for touch detection.
const  int8_t touchSensorPins[] = {42, 43};		///< Lick sensor - Lick sensor pins.

// Initial value for state variables.
uint32_t frameCount = 0;						// Lap counter; updates when photo sensor detects reflective tape.
int32_t lapCount = 0;							// Lap counter; updates when photo sensor detects reflective tape.
uint32_t lickCount = 0;							// Keep track of delivery of licks.
bool movingForward = true;						// Direction the belt is currently moving.
bool mustLick = false;							// Whether a licking is required to claim a reward.
bool rewardAvailable = false;					// Enable one reward per lap.
int32_t rewardedLap = 1;						// Which lap triggers a reward.
uint32_t rewardCount = 0;						// Keep track of reward deliveries.
int32_t rewardTimeout = 0;						// Time (us) at which the reward expires.
int8_t testPhase = 1;							// Test for modes: Lick to reward vs reward always.
bool wasMovingForward = true;

void onFrame(DigitalInput* digitalInput, bool state);
void onPhotoSensor(DigitalInput* digitalInput, bool state);
void onRotaryEncoder(RotaryEncoder* rotaryEncoder, int32_t change);
void onTouchSensor(TouchSensor* touchSensor, bool state);

/// Digital input for photo sensor.
DigitalInput photoSensor = DigitalInput(photoSensorPin, onPhotoSensor);

/// Digital input for frame inputs.
DigitalInput frame = DigitalInput(framePin, onFrame);

/// Oscillator for triggering rewards.
Oscillator oscillator1 = Oscillator();
Oscillator oscillator2 = Oscillator();

/// Rotary encoder for the treadmill belt.
RotaryEncoder rotaryEncoder = RotaryEncoder(encoderPins[0], encoderPins[1], onRotaryEncoder);

/// Touch sensor for detecting licks at the feeder.
TouchSensor touchSensor = TouchSensor(touchSensorPins[0], touchSensorPins[1], touchPeriods, touchThreshold, onTouchSensor);

/// Arduino library setup.
void setup() {
	Serial.begin(115200);
	Serial.println("tm,0,nt,started");
	
	pinMode(photoSensorForwardPin, OUTPUT);
	digitalWrite(photoSensorForwardPin, LOW);
	
	pinMode(touchForwardPin, OUTPUT);
	digitalWrite(touchForwardPin, LOW);
}

/// Arduino library loop: Update all steppers.
void loop() {
	frame.Step();
	rotaryEncoder.Step();
	photoSensor.Step();
	touchSensor.Step();
	oscillator1.Step();
	oscillator2.Step();
}

/// Play a tone according to settings.
void cue() {
	noTone(tonePin);
	tone(tonePin, round(0.001L * toneFrequency), round(0.001L * toneDuration));
}

/// Process a response from photo sensor.
void onPhotoSensor(DigitalInput* digitalInput, bool state) {
	// Photo sensor states are reversed.
	state = !state;
	digitalWrite(photoSensorForwardPin, state);
	if (state) {
		wasMovingForward = movingForward;
	} else if (wasMovingForward == movingForward) {
		// Forward and backwards motion when photo sensor triggers indicates a full change.
		lapCount += movingForward ? +1 : -1;
		
		if (lapCount == rewardedLap) {
			// Cue reward availability with a tone.
			cue();
			// Mark next rewarded lap.
			rewardedLap += 1;
			
			if (mustLick) {
				// Mode 1: Must claim reward by licking within a time window.
				rewardAvailable = true;
				rewardTimeout = micros() + rewardTimeWindow;
			} else {
				// Mode 2: Reward is delivered every lap.
				reward();
			}
		}
		
		Serial.println(String() + "tm," + millis() + ",lp," + lapCount + ",lk," + lickCount);
	}
}

/// Process a response from frame inputs.
void onFrame(DigitalInput* digitalInput, bool state) {
	if (state) {
		frameCount += 1;
		Serial.println(String() + "tm," + millis() + ",fm," + frameCount + ",rr," + rotaryEncoder.GetValue());
	}
}

/// Update direction of movement from rotary encoder.
void onRotaryEncoder(RotaryEncoder* rotaryEncoder, int32_t change) {
	movingForward = change > 0;
}

/// Trigger a reward when available.
void onTouchSensor(TouchSensor* touchSensor, bool state) {
	// Forward sensor state to a digital pin.
	digitalWrite(touchForwardPin, state);
	
	// Check for availability and expiration of a reward.
	if (state) {
		lickCount += 1;
		if (rewardAvailable) {
			rewardAvailable = false;
			if (micros() <= rewardTimeout) {
				rewardCount += 1;
				Serial.println(String() + "tm," + millis() + ",rw," + rewardCount);
				reward();
			}
		}
	}
	
	if (testPhase == 1) {
		// State is always low in the first message (calibration). Time to choose a mode.
		testPhase = 2;
		// Cue.
		cue();
	} else if (testPhase == 2) {
		// Contact has been made for the first time. Timing indicates a choice of mode.
		if (micros() < 3e6) {
			mustLick = true;
			reward();
		} else {
			mustLick = false;
		}
		testPhase = 0;
	}
}

/// Trigger a reward according to settings.
void reward() {
	oscillator1.Start(rewardPin, rewardDuration);
	oscillator2.Start(rewardForwardPin, rewardDuration);
}
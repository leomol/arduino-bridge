/**
 * @brief Setup an Arduino for detecting laps with 5 IR sensors.
 * Lap counter with 5 IR sensors
 * Sensor 1: Pin 4, corner 1a.
 * Sensor 2: Pin 5, corner 2.
 * Sensor 3: Pin 6, corner 3.
 * Sensor 4: Pin 7, corner 4.
 * Sensor 5: Pin 8, corner 1b.
 * Arduino and PC via USB to programming port.
 * Sensor 4 to sensor 5 increases one lap.
 * Sensor 5 to sensor 4 decreases one lap.
 * 
 * Application: Enrichment task.
 *
 * @file LapCounterIRx5.ino
 * @author Leonardo Molina (leonardomt@gmail.com)
 * @date 2018-08-23
 * @version: 0.1.180917
**/

#include "DigitalInput.h"

using namespace bridge;

const  int8_t forwardPin = LED_BUILTIN;			///< Forward state to this pin.
const uint8_t vertices[] = {4, 5, 6, 7, 8};		///< Sensor pin numbers. Signal pin is sensor + 1.

void onSensor(DigitalInput* sensor, bool state, uintptr_t currentVertex);
const uint8_t nVertices = sizeof(vertices);

uint8_t ledState = 0;
int32_t lap = 0;
int8_t previousVertex = -1;
int32_t seconds = 0;

/// Input objects.
DigitalInput sensor1 = DigitalInput(vertices[0], onSensor, 1);
DigitalInput sensor2 = DigitalInput(vertices[1], onSensor, 2);
DigitalInput sensor3 = DigitalInput(vertices[2], onSensor, 3);
DigitalInput sensor4 = DigitalInput(vertices[3], onSensor, 4);
DigitalInput sensor5 = DigitalInput(vertices[4], onSensor, 5);

/// Arduino library setup.
void setup() {
	Serial.begin(115200);
	Serial.println("Started");
	pinMode(forwardPin, OUTPUT);
	digitalWrite(forwardPin, LOW);
}

/// Arduino library loop: Update all steppers.
void loop() {
	sensor1.Step();
	sensor2.Step();
	sensor3.Step();
	sensor4.Step();
	sensor5.Step();
	seconds = millis() / 1e3L;
}

/// Logic to count laps.
void onSensor(DigitalInput* sensor, bool state, uintptr_t currentVertex) {
	// Forward sensor state to a digital pin.
	ledState = bitWrite(ledState, currentVertex, !state);
	digitalWrite(forwardPin, ledState > 0);
	// Print sensor state to serial port.
	Serial.println(String() + seconds + ". Sensor " + currentVertex + (state ? " ON" : " OFF"));
	
	if (!state) {
		if (currentVertex == nVertices && previousVertex > 1 && previousVertex < nVertices) {
			// Increase one lap: 2 | 3 | 4 then 5.
			setLap(lap + 1);
		} else if (previousVertex == nVertices && currentVertex > 1 && currentVertex < nVertices) {
			// Decrease one lap: 5 then 4 | 3 | 2.
			setLap(lap - 1);
		}
		previousVertex = currentVertex;
	}
}

void setLap(int32_t n) {
	lap = n;
	Serial.println(String() + seconds + ". Lap " + lap);
}
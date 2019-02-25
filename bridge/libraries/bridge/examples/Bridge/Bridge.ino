/*
	\author Leonardo Molina (leonardomt@gmail.com).
	\version 1.1.180921
*/

#include "Bridge.h"
bridge::Stepper* stepper;

void setup() {
	const uint32_t baudrate = 115200;
	stepper = new bridge::Bridge(&Serial, baudrate);
}

void loop() {
	stepper->Step();
}
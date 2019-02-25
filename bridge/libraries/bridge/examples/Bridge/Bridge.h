#ifndef BRIDGE_H
#define BRIDGE_H

#include <stdint.h>
#include "WString.h"
#include "HardwareSerial.h"
#include "Adafruit_PWMServoDriver.h"

#include "Channel.h"
#include "LinkedIndex.h"
#include "Stepper.h"
#include "Routine.h"

using PWMDriver = Adafruit_PWMServoDriver;

namespace bridge {
	class Bridge : public Stepper {
		public:
			Bridge();
			Bridge(HardwareSerial* serial, uint32_t baudrate);
			void Step() override;
		
		// private:
			enum class Status {
				disabled,	///< Setting up
				handshake,	///< Waiting for handshake
				debug,		///< Using debug mode as communication protocol
				raw = 255	///< Using raw mode as communication protocol
			};
			
			static Bridge* instance;
			static HardwareSerial* serial;
			static LinkedIndex<Routine*> setters;	// Linked list of not null output* elements from the array.
			static LinkedIndex<Routine*> getters;	// Linked list of not null input* elements from the array.
			static const uint8_t nHid = 69;			// Max number of indexed elements.
			static uint32_t baudrate;
			static PWMDriver pwmDriver;
			static uint8_t tonePin;
			static Status status;
			static ReportFunction reportFunction;
			
			bool pwmSetup;
			Channel channel;
			void handshake();
			void read();
			void blink(int8_t hid, uint16_t halfDuration, uint16_t repetitions);
			void addGetter(int8_t hid, Routine* routine);
			void addSetter(int8_t hid, Routine* routine);
			void removeSetter(int8_t hid);
			void removeGetter(int8_t hid);
			void SetPWM(uint8_t hid, uint16_t duration);
			void SetupPWM();
			
			static void getterRoutine();
			static void setterRoutine();
			static void risingCallback(uintptr_t hid);
			static void changeCallback(uintptr_t hid);
			static uint8_t encodeState(uint8_t hid, bool state);
			static bool decodeState(uint8_t code, uint8_t &pin, bool &state);
			
			static void reportText(int8_t hid, int32_t current, int32_t delta);
			static void reportRaw(int8_t hid, int32_t current, int32_t delta);
	};
}
#endif
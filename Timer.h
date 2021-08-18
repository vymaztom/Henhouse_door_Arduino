#ifndef Timer_h
#define Timer_h

#include <Arduino.h>
#include <stdint.h>
#include <stdarg.h>




class Timer {

public:
	Timer(unsigned long diffrentMillis ,void (*foo)());
	void run();
private:
	void (*function)();
	unsigned long _startMillis = millis();
	unsigned long _diffrentMillis;
};



#endif

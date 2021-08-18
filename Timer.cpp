#include "Timer.h"


Timer::Timer(unsigned long diffrentMillis ,void (*foo)()){
	_startMillis = millis();
	_diffrentMillis = diffrentMillis;
	function = foo;
}

void Timer::run(){
	unsigned long diffrentMillis = millis() - _startMillis;
	if(diffrentMillis > _diffrentMillis){
		_startMillis = millis();
		function();
	}
}

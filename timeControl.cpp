#include "Arduino.h"

#include "timeControl.h"

#ifndef TIMECONTROL_CPP
#define TIMECONTROL_CPP

/*******************************************************************************

						DEFINE PUBLIC FUNCTIONS OF CLASS

*******************************************************************************/

TC::TC(unsigned long offset){
	this->offset = offset;
	this->last = 0;
	this->active = false;
}

void TC::start(){
	this->last = millis();
	this->active = true;
}
void TC::stop(){
	this->active = false;
}
bool TC::getState(){
	return this->active;
}
bool TC::isAviable(){
	if((timeDiff() > this->offset) && this->active){
		start();
		return true;
	}
	return false;
}

void TC::setOffset(unsigned long offset){
	this->offset = offset;
}

/*******************************************************************************

					DEFINE PRIVATE FUNCTIONS OF CLASS

*******************************************************************************/

unsigned long TC::timeDiff(){
	return(millis()-this->last);
}

#endif

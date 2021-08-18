#include "Arduino.h"


#ifndef TIMECONTROL_H
#define TIMECONTROL_H

/*******************************************************************************

						DEFINE CLASS STRUCTURE

*******************************************************************************/

class TC{
	public:
		TC(unsigned long offset);
		void start();
		void stop();
		bool getState();
		bool isAviable();
		void setOffset(unsigned long offset);
	private:
		bool active;
		unsigned long last;
		unsigned long offset;
		unsigned long timeDiff();
};



#endif

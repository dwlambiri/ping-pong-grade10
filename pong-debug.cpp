/*
 * pong-debug.cpp
 *
 *  Created on: May 31, 2017
 *      Author: dwlambiri
 */

#include <stdio.h>
#include <time.h>
#include "pong-debug.h"

bool debugon = false;
bool traceon = false;
int  debuglevel = debug_c;

/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 31, 2017
   @mname   printDebugTime
   @details
	  \n
  --------------------------------------------------------------------------
 */
char*
printDebugTime() {

	static clock_t firstClock;
	static char buffer[100];
	static time_t rawtime;
	static bool single = true;
	struct tm * timeinfo;

	if (single == true) {
		single = false;
		rawtime = time (NULL);
		firstClock = clock();

	} //end-of-if(single == true)

	clock_t difftime = clock() - firstClock;
	int seconds = difftime /CLOCKS_PER_SEC ;
	int usec = difftime % CLOCKS_PER_SEC;
	time_t newtime = rawtime + seconds;

	char tb[40];
	timeinfo = localtime (&newtime);
	strftime (tb,40,"%F %T",timeinfo);

	sprintf(buffer, "%s.%d:%d", tb, seconds, usec);

	return buffer;
} // end-of-function printDebugTime






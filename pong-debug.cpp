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
	  I am trying to print the time when the macros are called.
	  However the function time() has a resolution of seconds.
	  The function clock() returns higher granularity.
	  I tried to print a time with a resolution better than one second.
	  Unfortunately the clock() function does not seem to work on Mac.
	  This function does not print the proper time on my Mac.
	  The function works on Windows machines.
	  I calculated the difference in clock() between the first call of the function
	  and each individual following call.
	  I add this difference to the time() at the moment of the first call.
	  sprintf() formats the result with resolution better than seconds\n
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

	sprintf(buffer, "%s.%d", tb,  usec);

	return buffer;
} // end-of-function printDebugTime






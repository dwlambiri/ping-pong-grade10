/*
 * write-results.cpp
 *
 *  Created on: May 30, 2017
 *      Author: dwlambiri
 */

#include <stdio.h>
#include "pong-debug.h"

static const char resultsFileName[] = "pong-results.txt";

/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 30, 2017
   @mname   recordResult
   @details
	  \n
  --------------------------------------------------------------------------
 */
bool
recordResult(char* p) {


	FILE* fptr;
	fptr = fopen(resultsFileName, "a");
	if(fptr == NULL){
		INFO("The file does not exist");
		return false;
	}
	fprintf(fptr,"%s \n", p);
	fclose(fptr);
	return true;
} // end-of-function recordResult

//bool debugon = true;
//bool traceon;
//int  debuglevel = 0;
///**
//  ---------------------------------------------------------------------------
//   @author  dwlambiri
//   @date    May 30, 2017
//   @name    main
//   @param   argc number of command line arguments
//   @param   argv command line arguments
//   @return  the return code
//   @details
//	The main function of the program.
//	\n
//  --------------------------------------------------------------------------
// */
//int
//main() {
//
//	recordResult("ONE");
//	recordResult("TWO");
//	recordResult("THREE");
//	return 0;
//}

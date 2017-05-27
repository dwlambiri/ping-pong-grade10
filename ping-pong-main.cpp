/*
 * ping-pong-main.cpp
 *
 *  Created on: May 14, 2017
 *      Author: dwlambiri
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "d-allegro.h"


/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 22, 2017
   @mname   Usage
   @details
	  \n
  --------------------------------------------------------------------------
 */
//static void
//Usage(void) {
//
//	printf("Program Options\n");
//	printf("   -a starts the program in arcade mode. by default the game is two player mode	\n");
//	printf("   -x number - sets the display  width. default is 1600\n");
//	printf("   -y number - sets the display height. default is 1200\n");
//	printf("   -s number - set font size. default is 24\n");
//	printf("   -l number - set level value. default is 10. higher is faster\n");
//	printf("   -m number - set max score value. default is 10.\n");
//	printf("   -p1 name - player1 name\n");
//	printf("   -p2 name - player2 name\n");
//
//} // end-of-function Usage

char** GetParameters(int* n);

/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 22, 2017
   @name    main
   @param   argc number of command line arguments
   @param   argv command line arguments
   @return  the return code
   @details
	The main function of the program.
	\n
  --------------------------------------------------------------------------
 */
int
main(int argc, char **argv) {


	int num = 0;
	char** p = GetParameters(&num);
	printf("HERE");
	if(CreateGameData(num, p) == false) return 0;

	if(InitGame() == false ) {
		//error initializing the game;
		return 22;
	}

	GameRun();

	return 0;
} // end-of-function main

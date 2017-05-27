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
static void
Usage(void) {

	printf("Program Options\n");
	printf("   -c configFileName - selects the config file, if no config file present, default is config.txt	\n");
	printf("   -h - displays command line parameters \n");

} // end-of-function Usage

char** GetParameters(int* , char*);

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

	char* configFileName = NULL;
	int num = 0;

	for (int  param = 1; param < argc; param++ ) {
		if (strcmp(argv[param], "-c") == 0) {
			if (++param < argc) {
				configFileName = argv[param];
			} //end-of-if(++param < argc)
		} else if (strcmp(argv[param], "-h") == 0) {
			Usage();
			return 1;
		}
	} //end-of-for

	char** p = GetParameters(&num, configFileName);
	//printf("Adress of GameRun = %x \n", &GameRun);
	if(CreateGameData(num, p) == false) return 0;

	if(InitGame() == false ) {
		//error initializing the game;
		return 22;
	}

	GameRun();

	return 0;
} // end-of-function main

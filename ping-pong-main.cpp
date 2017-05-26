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

	if(CreateGameData(argc, argv) == false) return 0;

	if(InitGame() == false ) {
		//error initializing the game;
		return 22;
	}

	GameRun();

	return 0;
} // end-of-function main

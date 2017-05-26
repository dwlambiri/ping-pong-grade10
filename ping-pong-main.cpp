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
	printf("   -a starts the program in arcade mode. by default the game is two player mode	\n");
	printf("   -x number - sets the display  width. default is 1600\n");
	printf("   -y number - sets the display height. default is 1200\n");
	printf("   -s number - set font size. default is 24\n");
	printf("   -l number - set level value. default is 10. higher is faster\n");
	printf("   -m number - set max score value. default is 10.\n");
	printf("   -p1 name - player1 name\n");
	printf("   -p2 name - player2 name\n");

} // end-of-function Usage

/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 24, 2017
   @mname   ProcessParams
   @details
	  \n
  --------------------------------------------------------------------------
 */
static bool
ProcessParams(int argc, char **argv, struct PongData* p) {
	int param = 0;
	//loop that processes the command line arguments.
	//argc is the size of the argument's array and argv is the array itself
	//argv[0] contains the name of the program
	for ( param = 1; param < argc; param++ ) {
		if(strcmp(argv[param],"-a")==0) {
			//arcade mode
			//player 2 is the computer
			p->arcade = true;
		}
		else if(strcmp(argv[param],"-x")==0) {
			//display width
			if(++param < argc) p->display.width = atoi(argv[param]);
		}
		else if(strcmp(argv[param],"-y")==0) {
			//display height
			if(++param < argc) p->display.height = atoi(argv[param]);
		}
		else if(strcmp(argv[param],"-s")==0) {
			//font size
			if(++param < argc) p->fontsize = atoi(argv[param]);
		}
		else if(strcmp(argv[param],"-l")==0) {
			//level
			if(++param < argc) p->level = atoi(argv[param]);
		}
		else if(strcmp(argv[param],"-p1")==0) {
			//player1 name
			if(++param < argc) strcpy(p->p1.name, argv[param]);
		}
		else if(strcmp(argv[param],"-p2")==0) {
			//player2 name
			if(++param < argc) strcpy(p->p2.name, argv[param]);
		}
		else if(strcmp(argv[param],"-m")==0) {
			//maxscore
			if(++param < argc) p->maxscore = atoi(argv[param]);
		}
		else if(strcmp(argv[param],"-h")==0) {
			Usage();
			return false;
		}
	}//end-of-for

	return true;
} // end-of-function ProcessParams



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

	if(ProcessParams(argc, argv, CreateGameData()) == false) return 0;

	if(InitGame() == false ) {
		//error initializing the game;
		return 22;
	}

	GameRun();

	return 0;
} // end-of-function main

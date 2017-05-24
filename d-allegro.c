//============================================================================
// Name        : d-allegro.c
// Author      : dwlambiri
// Version     :
// Copyright   : (c) 2017 Darius William Lambiri
// Description : pong game
//============================================================================

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "d-allegro.h"

/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 22, 2017
   @mname   SetBackgroundColor
   @details
	  \n
  --------------------------------------------------------------------------
 */
void
SetBackgroundColor(ALLEGRO_COLOR color) {
	// set to yellow
	al_clear_to_color(color);
} // end-of-function SetBackgroundColor



/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 22, 2017
   @mname   LoadBitmap
   @details
	  return true if ok false otherwise\n
  --------------------------------------------------------------------------
 */
bool
LoadBitmap(struct GameEntity* g, char* fname) {
	g->fname = fname;
	if((g->bmap = al_load_bitmap(fname)) == NULL ) {
		printf("cannot load %s\n ", fname);
		return false;
	}
	g->width = al_get_bitmap_width(g->bmap);
	g->height = al_get_bitmap_height(g->bmap);

	return true;
} // end-of-function LoadBitmap

/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 22, 2017
   @mname   InitialPosition
   @details
	  \n
  --------------------------------------------------------------------------
 */
void
InitialPosition(struct PongData* p) {

	p->ball.xspeed = rand() % p->level - p->level/2;
	if((p->ball.xspeed >= -4) && (p->ball.xspeed <= 0))
		p->ball.xspeed = -7;
	if((p->ball.xspeed > 0) && (p->ball.xspeed <= 4))
			p->ball.xspeed = 7;
	p->ball.yspeed = rand() % p->level - p->level/2;
	if(p->ball.yspeed == 0) p->ball.yspeed = 3;

	p->p1.ge.xposition = p->display.width - p->p1.ge.width;
	p->p1.ge.yposition = p->display.height/2 - (p->p1.ge.height/2);
	p->p1.ge.yspeed = 0;
	p->p2.ge.xposition = 0;
	p->p2.ge.yposition = p->display.height/2 - (p->p2.ge.height/2);
	p->p2.ge.yspeed = 0;

	if(p->ball.xspeed > 0 ) p->ball.xposition = p->p2.ge.xposition + p->p2.ge.width;
	else p->ball.xposition = p->p1.ge.xposition - p->ball.width;
	p->ball.yposition = p->display.height/2 - (p->ball.height/2);


} // end-of-function InitialPosition



/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 22, 2017
   @mname   InitGame
   @details
	  returns 1 if init ok, 0 otherwise\n
  --------------------------------------------------------------------------
 */
bool
InitGame(struct PongData* p, int screenheight, int screenwidth, float refreshtime,  int fontsize, int level, bool arcade) {
	//seed random number generator with time
	srand (time(NULL));
	//initiallises allegro libraries
	al_init();
	al_init_primitives_addon();
	al_init_image_addon();
	al_install_keyboard();
	al_init_font_addon(); // initialize the font addon
	al_init_ttf_addon();// initialize the ttf (True Type Font) addon
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(2);

	//tries to load font file
	p->font = al_load_ttf_font("pirulen.ttf", fontsize,0 );

	//error message if the font file is NULL
	if (p->font == NULL){
	      printf("Could not load 'pirulen.ttf'.\n");
	      return false;
	}

	p->arcade = arcade;
	p->display.height = screenheight;
	p->display.width  = screenwidth;
	p->display.display = al_create_display(p->display.width, p->display.height);
	p->bcolor = al_map_rgb(255, 255, 0);
	p->fcolor = al_map_rgb(0, 0, 0);
	p->timer = al_create_timer(refreshtime);
	p->eventqueue = al_create_event_queue();

	al_register_event_source(p->eventqueue, al_get_keyboard_event_source());
	al_register_event_source(p->eventqueue, al_get_display_event_source(p->display.display));
	al_register_event_source(p->eventqueue, al_get_timer_event_source(p->timer));
	if(p->arcade == true) {
		printf("Arcade Mode Detected\n");
		p->hal9000 = al_create_timer(1.0/COMPUTERSPEED);
		al_register_event_source(p->eventqueue, al_get_timer_event_source(p->hal9000));
	}
	else p->hal9000 = NULL;

	if(LoadBitmap(&(p->p1.ge), P1FNAME) == false) return false;
	if(LoadBitmap(&(p->p2.ge), P2FNAME) == false) return false;
	if(LoadBitmap(&(p->ball), BALLFNAME) == false) return false;
	p->p1.sample = al_load_sample( P1SOUND );
	if (p->p1.sample == NULL) {
	   printf( "Audio clip sample for player 1 not loaded!\n" );
	   return false;
	}
	p->p2.sample = al_load_sample( P2SOUND );
	if (p->p2.sample == NULL) {
	   printf( "Audio clip sample for player 2 not loaded!\n" );
	   return false;
	}
	p->p1.score = 0;
	p->p2.score = 0;
	p->level    = level;
	p->roundWinner = NULL;

	InitialPosition(p);

	SetBackgroundColor(p->bcolor);

	return true;
} // end-of-function InitGame

/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 22, 2017
   @mname   ProcessKeyPress
   @details
	  \n
  --------------------------------------------------------------------------
 */
bool
ProcessKeyPress(struct PongData* p) {

	if (p->ev.type == ALLEGRO_EVENT_KEY_DOWN){
		switch (p->ev.keyboard.keycode){
		case ALLEGRO_KEY_UP:
			p->p1.ge.yposition -= p->display.height/PLAYERSPEED;
			if(p->p1.ge.yposition < 0) p->p1.ge.yposition = 0;
			break;
		case ALLEGRO_KEY_DOWN:
			p->p1.ge.yposition += p->display.height/PLAYERSPEED;
			if(p->p1.ge.yposition >= (p->display.height - p->p1.ge.height))
				p->p1.ge.yposition = (p->display.height - p->p1.ge.height);
			break;
		case ALLEGRO_KEY_W:
			if(p->arcade == false ) p->p2.ge.yposition -= p->display.height/PLAYERSPEED;
			else break;
			if(p->p2.ge.yposition < 0) p->p2.ge.yposition = 0;
			break;
		case ALLEGRO_KEY_S:
			if(p->arcade == false ) p->p2.ge.yposition += p->display.height/PLAYERSPEED;
			else break;
			if(p->p2.ge.yposition >= (p->display.height - p->p2.ge.height))
				p->p2.ge.yposition = (p->display.height - p->p2.ge.height);
			break;
		case ALLEGRO_KEY_ESCAPE:
			//exit game
			return false;

		}
	}
	return true;
} // end-of-function ProcessKeyPress

/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 22, 2017
   @mname   DrawText
   @details
	  \n
  --------------------------------------------------------------------------
 */
void
DrawText(struct PongData* p, char* text, int x ,int y) {
    al_draw_text(p->font, p->fcolor, x, y,ALLEGRO_ALIGN_CENTRE, text);
} // end-of-function DrawText


/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 22, 2017
   @mname   DisplayText
   @details
	  \n
  --------------------------------------------------------------------------
 */
void
DisplayTextQH(struct PongData* p, char* text) {
	DrawText(p, text, p->display.width/2, p->display.height/4);
} // end-of-function DisplayText

/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 22, 2017
   @mname   DisplayTextAndWaitForKey
   @details
	  Returns false if escape key is pressed\n
  --------------------------------------------------------------------------
 */
bool
DisplayTextAndWaitForKey(struct PongData* p,char* text) {

	DisplayTextQH(p, text);
	al_flip_display();
	al_wait_for_event(p->eventqueue, &(p->ev));
	if (p->ev.type == ALLEGRO_EVENT_KEY_DOWN){
		switch (p->ev.keyboard.keycode){
		case ALLEGRO_KEY_ESCAPE:
			//exit game
			return false;

		}
	}
	return true;
} // end-of-function DisplayTextAndWaitForKey

/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 22, 2017
   @mname   DrawBitmap
   @details
	  \n
  --------------------------------------------------------------------------
 */
void
DrawBitmap(struct GameEntity* g) {
	al_draw_bitmap(g->bmap, g->xposition, g->yposition, 0);

} // end-of-function DrawBitmap

/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 22, 2017
   @mname   DrawObjects
   @details
	  \n
  --------------------------------------------------------------------------
 */
void
DrawObjects(struct PongData* p) {

	SetBackgroundColor(p->bcolor);
	DrawBitmap(&(p->p1.ge));
	DrawBitmap(&(p->p2.ge));
	DrawBitmap(&(p->ball));
} // end-of-function DrawObjects

/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 22, 2017
   @mname   CheckTopBottomCollision
   @details
	  true if there is a collision with top or bottom\n
  --------------------------------------------------------------------------
 */
bool
CheckTopBottomCollision(struct PongData* p) {

	if (p->ball.yposition > (p->display.height-p->ball.height)) {
		p->ball.yposition = p->display.height-p->ball.height;
		if(p->ball.yspeed > 0) p->ball.yspeed *= -1;
		return true;
	}
	else if(p->ball.yposition < 0 )	{
		p->ball.yposition = 0;
		if(p->ball.yspeed < 0 ) p->ball.yspeed *= -1;
		return true;
	}

	return false;
} // end-of-function CheckTopBottomCollision

/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 22, 2017
   @mname   CheckSideCollitions
   @details
	  \n
  --------------------------------------------------------------------------
 */
bool
CheckSideCollitions(struct PongData* p) {

	if (p->ball.xposition >= (p->display.width-p->ball.width) ){
		p->p2.score++;
		p->roundWinner = &(p->p2);
		return true;

	}
	else if (p->ball.xposition <= 0 ){
		p->p1.score++;
		p->roundWinner = &(p->p1);
		return true;
	}
	return false;
} // end-of-function CheckSideCollitions

/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 22, 2017
   @mname   PrintRoundWinner
   @details
	  \n
  --------------------------------------------------------------------------
 */
bool
PrintRoundWinner(struct PongData* p) {

	al_stop_timer(p->timer);
	if(p->arcade) al_stop_timer(p->hal9000);

	InitialPosition(p);
	DrawObjects(p);
	DrawText(p, "Press any key to start or ESC to exit", p->display.width/2, p->display.height/3);

	char textBuffer[255];
	sprintf(textBuffer, "%s Wins!! Score: %s %d %s %d",p->roundWinner->name, p->p2.name, p->p2.score, p->p1.name, p->p1.score);
	if(DisplayTextAndWaitForKey(p, textBuffer) == false) {
		return false;
	}
	else {
		al_start_timer(p->timer);
		if(p->arcade) al_start_timer(p->hal9000);
	}

	return true;
} // end-of-function PrintRoundWinner


/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 23, 2017
   @mname   PlaySound
   @details
	  \n
  --------------------------------------------------------------------------
 */
void
PlaySound(ALLEGRO_SAMPLE* s) {
	al_play_sample(s, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
} // end-of-function PlaySound



/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 22, 2017
   @mname   CheckPaletteCollision
   @details
	  true if there is a collision false otherwise\n
  --------------------------------------------------------------------------
 */
bool
CheckPaletteCollision(struct PongData* p) {

	if (    p->ball.xposition+ p->ball.width>= p->p1.ge.xposition &&
			p->ball.yposition + p->ball.height >= p->p1.ge.yposition  &&
			p->ball.yposition  <= p->p1.ge.yposition + p->p1.ge.height){

			p->ball.xspeed *= -1;
			p->ball.xspeed -= rand() %2;
			if(p->ball.xspeed >=0) p->ball.xspeed = -5;
			PlaySound(p->p1.sample);
			return true;
	}

	else if (   p->ball.xposition < p->p2.ge.xposition +p->p2.ge.width &&
			p->ball.yposition + p->ball.height >= p->p2.ge.yposition  &&
			p->ball.yposition  <= p->p2.ge.yposition + p->p2.ge.height){

		    p->ball.xspeed *= -1;
		    p->ball.xspeed += rand() %2;
		    if(p->ball.xspeed <=0) p->ball.xspeed = 5;
		    PlaySound(p->p2.sample);
			return true;
	}

	return false;
} // end-of-function CheckPaletteCollision



/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 22, 2017
   @mname   UpdateBallPosition
   @details
	  return true if round is finished\n
  --------------------------------------------------------------------------
 */
bool
UpdateBallPosition(struct PongData* p) {

	p->ball.xposition = p->ball.xposition + p->ball.xspeed;
	p->ball.yposition = p->ball.yposition + p->ball.yspeed;

	CheckPaletteCollision(p);
	CheckTopBottomCollision(p);
	if(CheckSideCollitions(p) == true) return true;


	return false;
} // end-of-function UpdateBallPosition

/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 23, 2017
   @mname   minSpeed
   @details
	  \n
  --------------------------------------------------------------------------
 */
int
minSpeed(int a, int b) {

	if(a < b) return a;
	else return b;
} // end-of-function minSpeed



/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 22, 2017
   @mname   UpdatePlayer2
   @details
	  \n
  --------------------------------------------------------------------------
 */
void
UpdatePlayer2(struct PongData* p) {

	//update only when ball moves towards the player
	if(p->ball.xspeed > 0) return;
	if(p->ball.yspeed > 0) {
		if(p->ball.yposition > p->p2.ge.yposition )
			p->p2.ge.yposition += p->display.height/minSpeed(COMPUTERSPEED,p->ball.yspeed) ;
		if(p->p2.ge.yposition >= (p->display.height - p->p2.ge.height))
			p->p2.ge.yposition = (p->display.height - p->p2.ge.height);
	}
	else {
		if(p->ball.yposition < p->p2.ge.yposition )
			p->p2.ge.yposition -= p->display.height/minSpeed(COMPUTERSPEED,-1*p->ball.yspeed);
		if(p->p2.ge.yposition < 0) p->p2.ge.yposition = 0;
	}
} // end-of-function UpdatePlayer2



/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 22, 2017
   @mname   GameLoop
   @details
	  \n
  --------------------------------------------------------------------------
 */
bool
GameLoop(struct PongData* p) {

	al_start_timer(p->timer);
	if(p->arcade == true) {
		al_start_timer(p->hal9000);
	}

	bool roundwin = false;
	int skipCounter = 0;
	int maxSkip = 45;

	while (true){

		al_wait_for_event(p->eventqueue, &(p->ev));

		if(roundwin == true) {
			if(p->ev.type == ALLEGRO_EVENT_TIMER &&
					   p->ev.timer.source == p->timer) {
				//skip maxSkip frames
				if(skipCounter++ >= maxSkip) {
					skipCounter = 0;
					roundwin = false;
					if(PrintRoundWinner(p) == false ) {
						//user has pressed ESC to end the game
						return false;
					}
				}

			}
			else continue;
		}
		else {
			if(ProcessKeyPress(p) == false) {
				//user has ended game
				return false;
			}

			if(p->arcade == true &&
			   p->ev.type == ALLEGRO_EVENT_TIMER &&
			   p->ev.timer.source == p->hal9000) {
				UpdatePlayer2(p);
			}
			if(p->ev.type == ALLEGRO_EVENT_TIMER &&
			   p->ev.timer.source == p->timer) {
				roundwin = UpdateBallPosition(p);
				DrawObjects(p);
				al_flip_display();
				if(roundwin == true) {
				}
			}
		}
	}

	return true;
} // end-of-function GameLoop

/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 22, 2017
   @mname   Usage
   @details
	  \n
  --------------------------------------------------------------------------
 */
void
Usage(void) {

	printf("Program Options\n");
	printf("   -a starts the program in arcade mode. by default the game is two player mode	\n");
	printf("   -x number - sets the display  width. default is 1600\n");
	printf("   -y number - sets the display height. default is 1200\n");
	printf("   -s number - set font size. default is 24\n");
	printf("   -l number - set level value. default is 10. higher is faster\n");
	printf("   -p1 name - player1 name\n");
	printf("   -p2 name - player2 name\n");
} // end-of-function Usage


/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 22, 2017
   @mname   GameExit
   @details
	  \n
  --------------------------------------------------------------------------
 */
void
GameExit(struct PongData* p) {

	al_rest(0.0);
	al_destroy_display(p->display.display);
} // end-of-function GameExit



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

	//declaring the main data variable of the game
	struct PongData pong;
	//declaring temporary variable for screen height
	int screenheight = SCREEN_H;
	//declares temporary variable for screen width
	int screenwidth = SCREEN_W;
	//declares a temporary variable for font size
	int fontsize = FONTSIZE;
	//declares a temporary variable for the maximum speed of the ball
	int level = LEVEL;

	//declares a temporary variable that allows iteration through the command line array
	int param = 0;
	//declares a temporary variable that determines whether the game shall played in 2 player (false) or vs ai mode (true)
	bool arcade = false;

	//sets the default player 1 and player 2 names
	strcpy(pong.p1.name, "Player1");
	strcpy(pong.p2.name, "Player2");

	//loop that processes the command line arguments.
	//argc is the size of the argument's array and argv is the array itself
	//argv[0] contains the name of the program
	for ( param = 1; param < argc; param++ ) {
		if(strcmp(argv[param],"-a")==0) {
			//arcade mode
			//player 2 is the computer
			arcade = true;
		}
		else if(strcmp(argv[param],"-x")==0) {
			//display width
			if(++param < argc)
				screenwidth = atoi(argv[param]);
		}
		else if(strcmp(argv[param],"-y")==0) {
			//display height
			if(++param < argc)
				screenheight = atoi(argv[param]);
		}
		else if(strcmp(argv[param],"-s")==0) {
			//font size
			if(++param < argc)
				fontsize = atoi(argv[param]);
		}
		else if(strcmp(argv[param],"-l")==0) {
			//font size
			if(++param < argc)
				level = atoi(argv[param]);
		}
		else if(strcmp(argv[param],"-p1")==0) {
			//font size
			if(++param < argc)
				strcpy(pong.p1.name, argv[param]);
		}
		else if(strcmp(argv[param],"-p2")==0) {
			//font size
			if(++param < argc)
				strcpy(pong.p2.name, argv[param]);
		}
		else if(strcmp(argv[param],"-h")==0) {
			Usage();
			return 0;
		}
	}
	//IF game is in arcade mode, player 2 is HAL9000
	if(arcade ==true) {
		strcpy(pong.p2.name, "HAL9000");
	}

	if(InitGame(&pong, screenheight, screenwidth, REFRESHTIME, fontsize, level, arcade) == false ) {
		//error initializing the game;
		return 22;
	}
	SetBackgroundColor(pong.bcolor);
	if(DisplayTextAndWaitForKey(&pong,"Press Any Key To Begin or ESC to Terminate") == true) {
		GameLoop(&pong);
	}


	GameExit(&pong);
	return 0;
} // end-of-function main

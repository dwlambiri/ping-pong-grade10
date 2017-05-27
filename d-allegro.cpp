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

static const int MAXNAME = 200;
static const int SCREEN_W = 1600;
static const int SCREEN_H = 1200;
//static const int PLAYEROFFSET = 100;
static const float FRAMERATE = 60.0;
static const float REFRESHTIME = 1.0/FRAMERATE;
static const float PLAYERSPEED = 10.0;
static const float COMPUTERSPEED = 10.0;
//static const int SOMENUMBER = 11;
static const int FONTSIZE = 24;
static const int LEVEL = 20;
static const int MAXSCORE = 10;


static const char P1FNAME[]  =  "player1.png";
static const char P2FNAME[] =   "player2.png";
static const char BALLFNAME[] = "ball.png";
static const char P1SOUND[] =   "p1sound.ogg";
static const char P2SOUND[] =   "p2sound.ogg";
static const char FONTNAME[] =  "pirulen.ttf";


static const char halname[] = "HAL9000";

/**
  ---------------------------------------------------------------------------
   @author     dwlambiri
   @date       May 22, 2017
   @name       GameEntity
   @details
	\n
  ---------------------------------------------------------------------------
 */
typedef struct GameEntity {
	int xposition;
	int yposition;
	int xspeed;
	int yspeed;
	int width;
	int height;
	ALLEGRO_BITMAP* bmap;
	char* fname;
}GameEntity;

#define INITGE {0, 0, 0, 0, 0, 0, NULL, NULL }

/**
  ---------------------------------------------------------------------------
   @author     dwlambiri
   @date       May 22, 2017
   @name       Player
   @details
	\n
  ---------------------------------------------------------------------------
 */
typedef struct Player {
	unsigned int score;
	GameEntity ge;
	char name[MAXNAME];
	ALLEGRO_SAMPLE *sample;
}Player;


/**
  ---------------------------------------------------------------------------
   @author     dwlambiri
   @date       May 22, 2017
   @name       Display
   @details
	\n
  ---------------------------------------------------------------------------
 */
typedef struct Display {
	int width;
	int height;
	ALLEGRO_DISPLAY *display;
} Display;

/**
  ---------------------------------------------------------------------------
   @author     dwlambiri
   @date       May 22, 2017
   @name       PongData
   @details
	\n
  ---------------------------------------------------------------------------
 */
typedef struct PongData {
	Player p1;
	Player p2;
	GameEntity   ball;
	Display display;
	bool   arcade;
	int    level;
	Player* roundWinner;
	int    fontsize;
	unsigned int maxscore;

	ALLEGRO_EVENT ev;
	ALLEGRO_EVENT_QUEUE *eventqueue;
	ALLEGRO_TIMER *timer;
	ALLEGRO_TIMER *hal9000;
	ALLEGRO_FONT *font;
	ALLEGRO_COLOR bcolor;
	ALLEGRO_COLOR fcolor;
} PongData;

//======== Game Data ===========

//declaring the main data variable of the game
static PongData pong = {
		{0, INITGE, "", NULL},
		{0, INITGE, "", NULL},
		INITGE,
		{SCREEN_W, SCREEN_H, NULL},
		false,
		LEVEL,
		NULL,
		FONTSIZE,
		MAXSCORE
};


//======= PRIVATE FUNCTIONS =========
/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 22, 2017
   @mname   SetBackgroundColor
   @details
	  \n
  --------------------------------------------------------------------------
 */
static void
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
static bool
LoadBitmap(GameEntity* g, char* fname) {
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
static void
InitialPosition(PongData* p) {

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
   @mname   ProcessKeyPress
   @details
	  \n
  --------------------------------------------------------------------------
 */
static bool
ProcessKeyPress(PongData* p) {

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
static void
DrawText(PongData* p, char* text, int x ,int y) {
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
static void
DisplayTextQH(PongData* p, char* text) {
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
static bool
DisplayTextAndWaitForKey(PongData* p,char* text) {

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
static void
DrawBitmap(GameEntity* g) {
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
static void
DrawObjects(PongData* p) {

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
static bool
CheckTopBottomCollision(PongData* p) {

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
static bool
CheckSideCollitions(PongData* p) {

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
static bool
PrintRoundWinner(PongData* p) {

	al_stop_timer(p->timer);
	if(p->arcade) al_stop_timer(p->hal9000);

	InitialPosition(p);
	DrawObjects(p);

	char textBuffer[255];
	if(p->roundWinner->score == p->maxscore) {
		DrawText(p, (char*)"Press any key to start a new game or ESC to exit", p->display.width/2, p->display.height/3);
		sprintf(textBuffer, "%s Wins The Game!! Score: %s %d %s %d",p->roundWinner->name, p->p2.name, p->p2.score, p->p1.name, p->p1.score);
		p->p2.score = 0;
		p->p1.score = 0;
	}
	else {
		DrawText(p, (char*)"Press any key to start or ESC to exit", p->display.width/2, p->display.height/3);
		sprintf(textBuffer, "%s Wins The Round!! Score: %s %d %s %d",p->roundWinner->name, p->p2.name, p->p2.score, p->p1.name, p->p1.score);
	}
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
static void
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
static bool
CheckPaletteCollision(PongData* p) {

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
static bool
UpdateBallPosition(PongData* p) {

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
static int
minSpeed(int a, int b) {

	if(a < b) return a;
	else return b;
} // end-of-function minSpeed



/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 22, 2017
   @mname   HAL9000AI
   @details
	  \n
  --------------------------------------------------------------------------
 */
static void
HAL9000AI(PongData* p) {

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
} // end-of-function HAL9000AI



/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 22, 2017
   @mname   GameLoop
   @details
	  \n
  --------------------------------------------------------------------------
 */
static bool
GameLoop(PongData* p) {

	al_start_timer(p->timer);
	if(p->arcade == true) {
		al_start_timer(p->hal9000);
	}

	bool roundwin = false;
	int skipCounter = 0;
	int maxSkip = 45;

	while (true){

		al_wait_for_event(p->eventqueue, &(p->ev));

//		if(p->ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
//					return false;
//		}
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
				HAL9000AI(p);
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
   @mname   GameExit
   @details
	  \n
  --------------------------------------------------------------------------
 */
static void
GameExit(PongData* p) {

	al_rest(0.0);
	al_destroy_display(p->display.display);
	al_destroy_timer(p->hal9000);
	al_destroy_timer(p->timer);
	al_destroy_event_queue(p->eventqueue);
	al_destroy_font(p->font);
	al_destroy_bitmap(p->p1.ge.bmap);
	al_destroy_bitmap(p->p2.ge.bmap);
	al_destroy_bitmap(p->ball.bmap);
} // end-of-function GameExit



//======== PUBLIC FUNCITONS ===========



/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 25, 2017
   @mname   CreateGameData
   @details
	  \n
  --------------------------------------------------------------------------
 */
bool
CreateGameData(int argc, char **argv) {
	//sets the default player 1 and player 2 names
	strcpy(pong.p1.name, "Player1");
	strcpy(pong.p2.name, "Player2");
	PongData* p = &pong;
	int param = 0;
	//loop that processes the command line arguments.
	//argc is the size of the argument's array and argv is the array itself
	//argv[0] contains the name of the program
	for ( param = 0; param < argc; param++ ) {
		if(strcmp(argv[param],"arcade")==0) {
			//arcade mode
			//player 2 is the computer
			if(++param < argc && (strcmp(argv[param],"true") == 0)) p->arcade = true;
			else p->arcade = false;
		}
		else if(strcmp(argv[param],"screenwidth")==0) {
			//display width
			if(++param < argc) p->display.width = atoi(argv[param]);
		}
		else if(strcmp(argv[param],"screenheight")==0) {
			//display height
			if(++param < argc) p->display.height = atoi(argv[param]);
		}
		else if(strcmp(argv[param],"fontsize")==0) {
			//font size
			if(++param < argc) p->fontsize = atoi(argv[param]);
		}
		else if(strcmp(argv[param],"level")==0) {
			//level
			if(++param < argc) p->level = atoi(argv[param]);
		}
		else if(strcmp(argv[param],"p1name")==0) {
			//player1 name
			if(++param < argc) strcpy(p->p1.name, argv[param]);
		}
		else if(strcmp(argv[param],"p2name")==0) {
			//player2 name
			if(++param < argc) strcpy(p->p2.name, argv[param]);
		}
		else if(strcmp(argv[param],"maxscore")==0) {
			//maxscore
			if(++param < argc) p->maxscore = atoi(argv[param]);
		}
	}//end-of-for

	return true;

	return true;
} // end-of-function CreateGameData


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
InitGame() {
	PongData* p = &pong;
	//seed random number generator with time
	srand (time(NULL));
	//initiallises allegro libraries
	al_init();
	al_init_primitives_addon();
	al_init_image_addon();
	al_install_keyboard();
	al_install_mouse();
	al_init_font_addon(); // initialize the font addon
	al_init_ttf_addon();// initialize the ttf (True Type Font) addon
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(2);

	//tries to load font file
	p->font = al_load_ttf_font(FONTNAME, p->fontsize,0 );

	//error message if the font file is NULL
	if (p->font == NULL){
	      printf("Could not load %s.\n", FONTNAME);
	      return false;
	}

	//IF game is in arcade mode, player 2 is HAL9000
	if(p->arcade ==true) {
		strcpy(p->p2.name, halname);
	}

	p->display.display = al_create_display(p->display.width, p->display.height);
	p->bcolor = al_map_rgb(255, 255, 0);
	p->fcolor = al_map_rgb(0, 0, 0);
	p->timer = al_create_timer(REFRESHTIME);
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

	if(LoadBitmap(&(p->p1.ge), (char*)P1FNAME) == false) return false;
	if(LoadBitmap(&(p->p2.ge), (char*)P2FNAME) == false) return false;
	if(LoadBitmap(&(p->ball), (char*)BALLFNAME) == false) return false;
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

	InitialPosition(p);

	SetBackgroundColor(p->bcolor);

	return true;
} // end-of-function InitGame

/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 25, 2017
   @mname   GameRun
   @details
	  \n
  --------------------------------------------------------------------------
 */
void
GameRun() {

	PongData* p = &pong;
	SetBackgroundColor(p->bcolor);
	if(DisplayTextAndWaitForKey(p,(char*)"Press Any Key To Begin or ESC to Terminate") == true) {
		GameLoop(p);
	}

	GameExit(p);
} // end-of-function GameRun

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
#include <math.h>
#include "d-allegro.h"
#include "pong-debug.h"

typedef unsigned int uint;

static const int MAXNAME = 200;
static const int SCREEN_W = 1600;
static const int SCREEN_H = 1200;
//static const int PLAYEROFFSET = 100;
static const float FRAMERATE = 60.0;
static const float REFRESHTIME = 1.0/FRAMERATE;
static const float PLAYERSPEED = 10.0;
//static const float COMPUTERSPEED = 10.0;
//static const int SOMENUMBER = 11;
static const int FONTSIZE = 24;
static const int MAXSCORE = 10;
static const int maxfonts_c = 3;
static const int maxcolours_c = 4;
static const int minballspeed_c = PLAYERSPEED;
static const int maxballspeed_c = 2*minballspeed_c;
static const uint maxlevel_c = 7;
static const uint maxdiff_c = 3;

enum FONTSIZES { smallFont_c = 0, regularFont_c = 1, largeFont_c =2};
enum COLOURS {yellow_c = 0, blue_c = 1, white_c = 2, green_c = 3};

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
	char bitmapFileName[MAXNAME];
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
	uint score;
	GameEntity ge;
	char name[MAXNAME];
	char audioFileName[MAXNAME];
	ALLEGRO_SAMPLE *sample;
	bool keyPress[2];
	int paddleSpeed;
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
	int    maxballspeed;
	Player* roundWinner;
	int    fontsize;
	uint maxscore;
	uint level;
	char fontFileName[MAXNAME];
	char winSoundFile[MAXNAME];


	ALLEGRO_EVENT ev;
	ALLEGRO_EVENT_QUEUE *eventqueue;
	ALLEGRO_TIMER *timer;
	ALLEGRO_TIMER *hal9000;
	ALLEGRO_FONT *font[maxfonts_c];
	ALLEGRO_COLOR bcolorarray[maxcolours_c];
	ALLEGRO_COLOR* bcolor;
	ALLEGRO_COLOR fcolor;
	ALLEGRO_SAMPLE *startsample;
	ALLEGRO_SAMPLE *winsample;
} PongData;

//======== Game Data ===========

//declaring the main data variable of the game
static PongData pong = {
		{0, INITGE, "", "",NULL, false, false, PLAYERSPEED},
		{0, INITGE, "", "", NULL, false, false, PLAYERSPEED},
		INITGE,
		{SCREEN_W, SCREEN_H, NULL},
		false,
		maxballspeed_c,
		NULL,
		FONTSIZE,
		MAXSCORE,
		1
};
//first array represents where in the field HAL will start to move
static int cond1[] = {2, 2, 3, 4, 8};
//This array is a multiplier to determine how much HAL should move
//setting an entry to zero will prevent HAL from moving
static float val1[] = {1, 1, 1 , 1, 1};


static int cond2[] = {2, 2, 3, 4, 8};
static float val2[] = {0, 1, 1.5 , 2, 3};

static int* condp = cond1;
static float* valptr = val1;



//======= EXTERNAL FUNCTION DECLARATION=====//
bool recordResult(char* p);

//======= FUNCTION DECLARATIONS =====
static void SetBackgroundColor(ALLEGRO_COLOR color);
static bool LoadBitmap(GameEntity* g);
static bool LoadAudio(Player* p);
static bool LoadWinAudio(PongData* p );
static bool LoadFont(PongData* p, int size);
static void InitialPosition(PongData* p);
static bool ProcessKeyPress(PongData* p);
static void MovePaddles(PongData* p);
static int DrawText(PongData* p, char* text, int x ,int y, int size);
static bool DisplayTextAndWaitBegin(PongData* p);
static bool DisplayTextAndWaitRoundWin(PongData* p);
static void DrawBitmap(GameEntity* g);
static void DrawObjects(PongData* p);
static bool CheckTopBottomCollision(PongData* p);
static bool CheckSideCollitions(PongData* p);
static void PlaySound(ALLEGRO_SAMPLE* s);
static bool PrintRoundWinner(PongData* p);
static bool CheckPaletteCollision(PongData* p);
static bool UpdateBallPosition(PongData* p);
static int minSpeed(int a, int b);
static void HAL9000AI(PongData* p);
static bool GameLoop(PongData* p);
static void GameExit(PongData* p);
static void PaletteBounceCalc(GameEntity* ball, Player* p, int, int);
static int SignOfNumber(int value);
static void DrawBitmapSection(GameEntity* g);
static bool LoadPlayerBitmap(GameEntity* g, int level);
static void SetHalIntelligence(PongData* p);

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
	// set to color
	TRACE();
	al_clear_to_color(color);
} // end-of-function SetBackgroundColor

/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 28, 2017
   @mname   LoadPlayerBitmap
   @details
	  \n
  --------------------------------------------------------------------------
 */
static bool
LoadPlayerBitmap(GameEntity* g, int level) {
	TRACE();
	if((g->bmap = al_load_bitmap(g->bitmapFileName)) == NULL ) {
		printf("cannot load %s\n ", g->bitmapFileName);
		return false;
	}
	g->width = al_get_bitmap_width(g->bmap);
	g->height = (al_get_bitmap_height(g->bmap)*(maxlevel_c + 1 - level)) / maxlevel_c;
	return true;
} // end-of-function LoadPlayerBitmap



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
LoadBitmap(GameEntity* g) {
	TRACE();
	if((g->bmap = al_load_bitmap(g->bitmapFileName)) == NULL ) {
		printf("cannot load %s\n ", g->bitmapFileName);
		return false;
	}
	g->width = al_get_bitmap_width(g->bmap);
	g->height = al_get_bitmap_height(g->bmap);

	return true;
} // end-of-function LoadBitmap


/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 27, 2017
   @mname   LoadAudio
   @details
	  \n
  --------------------------------------------------------------------------
 */
static bool
LoadAudio(Player* p) {
	TRACE();
	p->sample = al_load_sample( p->audioFileName );
	if (p->sample == NULL) {
	   printf( "Audio clip sample %s not loaded!\n", p->audioFileName );
	   return false;
	}
	return true;
} // end-of-function LoadAudio

/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 28, 2017
   @mname   LoadWinAudio
   @details
	  \n
  --------------------------------------------------------------------------
 */
static bool
LoadWinAudio(PongData* p ) {
	TRACE();
	p->winsample = al_load_sample( p->winSoundFile );
	if (p->winsample == NULL) {
	   printf( "Audio clip sample %s not loaded!\n", p->winSoundFile );
	   return false;
	}
	return true;
} // end-of-function LoadWinAudio



/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 27, 2017
   @mname   LoadFont
   @details
	  \n
  --------------------------------------------------------------------------
 */
static bool
LoadFont(PongData* p, int size) {

	TRACE();
	int fontSize = p->fontsize;
	switch (size) {
		case smallFont_c:
			fontSize /= 2;
			break;
		case largeFont_c:
			fontSize *= 2;
			break;
		default:
			break;
	} //end-switch(size)
	p->font[size] = al_load_ttf_font(p->fontFileName, fontSize,0 );

	//error message if the font file is NULL
	if (p->font[size] == NULL){
	      printf("Could not load %s.\n", p->fontFileName);
	      return false;
	}
	return true;
} // end-of-function LoadFont


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

	TRACE();
	p->ball.xspeed =  minballspeed_c + rand() % (p->maxballspeed -minballspeed_c);
	if (p->roundWinner) {
		//printf("Round winner name %s\n",p->roundWinner->name);
		if (p->roundWinner == &(p->p1)) {
			//printf("Assigning ball to %s\n",p->roundWinner->name);
			p->ball.xspeed *= -1;
		} //end-of-if(p->roundWinner == &(p->p1))
	} else {
		//if there is no roundwinnner, it is the first serve of the game
		//we need to pick at random a starting player
		switch (rand() %2) {
			case 0:
				// player 1
				p->ball.xspeed *= -1;
				break;
			default:
				//player 2
				break;
		} //end-switch(rand() %2)
	}//end-of-if(p->roundWinner)

	static const float ratio_c = (float) p->display.height / (2*p->display.width);
	float maxyspeed = ratio_c * abs(p->ball.xspeed);


	p->ball.yspeed = rand() % (int) maxyspeed;
	if(p->ball.yspeed == 0) p->ball.yspeed = 3;
	switch (rand() %2) {
		case 0:
			//serve up
			p->ball.yspeed *= -1;
			break;
		default:
			break;
	} //end-switch(rand() %2)

	p->p1.ge.xposition = p->display.width - p->p1.ge.width;
	p->p1.ge.yposition = p->display.height/2 - (p->p1.ge.height/2);
	p->p1.ge.yspeed = 0;
	p->p2.ge.xposition = 0;
	p->p2.ge.yposition = p->display.height/2 - (p->p2.ge.height/2);
	p->p2.ge.yspeed = 0;

	if(p->ball.xspeed > 0 ) p->ball.xposition = p->p2.ge.xposition + p->p2.ge.width;
	else p->ball.xposition = p->p1.ge.xposition - p->ball.width;
	p->ball.yposition = p->display.height/2 - (p->ball.height/2);
	if (p->ball.xspeed > 0) {
		p->startsample = p->p2.sample;
	} else {
		p->startsample = p->p1.sample;
	}


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

	TRACE();
	if (p->ev.type == ALLEGRO_EVENT_KEY_DOWN){
		switch (p->ev.keyboard.keycode){
		case ALLEGRO_KEY_UP:
			p->p1.keyPress[0] = true;
			p->p1.keyPress[1] = false;
			break;
		case ALLEGRO_KEY_DOWN:
			p->p1.keyPress[0] = false;
			p->p1.keyPress[1] = true;
			break;
		case ALLEGRO_KEY_W:
			if(p->arcade == false ) p->p2.keyPress[0] = true;
			else p->p2.keyPress[0] = false;
			p->p2.keyPress[1] = false;
			break;
		case ALLEGRO_KEY_S:
			if(p->arcade == false ) p->p2.keyPress[1] = true;
			else p->p2.keyPress[1] = true;
			p->p1.keyPress[0] = false;
			break;
		case ALLEGRO_KEY_ESCAPE:
			//exit game
			return false;
		}
	}
	else if (p->ev.type == ALLEGRO_EVENT_KEY_UP){
		switch (p->ev.keyboard.keycode){
		case ALLEGRO_KEY_UP:
			p->p1.keyPress[0] = false;
			break;
		case ALLEGRO_KEY_DOWN:
			p->p1.keyPress[1] = false;
			break;
		case ALLEGRO_KEY_W:
			if(p->arcade == false ) p->p2.keyPress[0] = false;
			break;
		case ALLEGRO_KEY_S:
			if(p->arcade == false ) p->p2.keyPress[1] = false;
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
   @date    May 27, 2017
   @mname   MovePaddles
   @details
	  \n
  --------------------------------------------------------------------------
 */
static void
MovePaddles(PongData* p) {

	TRACE();
	if(p->p1.keyPress[0] ==true){
		p->p1.ge.yposition -= p->p1.paddleSpeed;
		if(p->p1.ge.yposition < 0) p->p1.ge.yposition = 0;
	}
	if (p->p1.keyPress[1] ==true) {
		p->p1.ge.yposition += p->p1.paddleSpeed;
		if(p->p1.ge.yposition >= (p->display.height - p->p1.ge.height))
				p->p1.ge.yposition = (p->display.height - p->p1.ge.height);
	} //end-of-if(p->p1.keyPress[1] ==true)

	if (p->p2.keyPress[0] == true) {
		p->p2.ge.yposition -= p->p2.paddleSpeed;
		if(p->p2.ge.yposition < 0) p->p2.ge.yposition = 0;
	} //end-of-if(p->p2.keyPress[0] == true)

	if (p->p2.keyPress[1] == true) {
		p->p2.ge.yposition += p->p2.paddleSpeed;
		if(p->p2.ge.yposition >= (p->display.height - p->p2.ge.height))
			p->p2.ge.yposition = (p->display.height - p->p2.ge.height);
	} //end-of-if(p->p2.keyPress[1] == true)

} // end-of-function MovePaddles



/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 22, 2017
   @mname   DrawText
   @details
	  \n
  --------------------------------------------------------------------------
 */
static int
DrawText(PongData* p, char* text, int x ,int y, int size) {
	TRACE();
    al_draw_text(p->font[size], p->fcolor, x, y,ALLEGRO_ALIGN_CENTRE, text);
    int fsize = p->fontsize;
    switch (size) {
		case smallFont_c:
			fsize /= 2;
			break;
		case largeFont_c:
			fsize *= 2;
			break;
		default:
			break;
	} //end-switch(size)
    return y+fsize+10;
} // end-of-function DrawText



/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 22, 2017
   @mname   DisplayTextAndWaitBegin
   @details
	  Returns false if escape key is pressed\n
  --------------------------------------------------------------------------
 */
static bool
DisplayTextAndWaitBegin(PongData* p) {

	TRACE();
	int next = DrawText(p, (char*)"Welcome to Pong", p->display.width/2, p->display.height/4, largeFont_c);
	al_flush_event_queue(p->eventqueue);
	DrawText(p, (char*)"(c) dwlambiri 2017", p->display.width/2, next, smallFont_c);

	if (p->arcade == true) {
		next = DrawText(p, (char*)"Arcade Mode (HAL is left)", p->display.width/2, p->display.height/2, regularFont_c);
	} else {
		next = DrawText(p, (char*)"Two Player Mode", p->display.width/2, p->display.height/2, regularFont_c);
	}
	char buffer[100];
	sprintf(buffer, "First to %d Wins!", p->maxscore);
	next = DrawText(p, buffer, p->display.width/2, next, regularFont_c);
	next = DrawText(p, (char*)"Press a key to begin", p->display.width/2, next, regularFont_c);

	if(p->level == maxlevel_c) {
		DrawText(p, (char*)"You've got balls mate: Balls of Fury Mode activated!!", p->display.width/2, next, regularFont_c);
	}
	al_flip_display();
	al_flush_event_queue(p->eventqueue);
	al_wait_for_event(p->eventqueue, &(p->ev));
	if (p->ev.type == ALLEGRO_EVENT_KEY_DOWN){
		switch (p->ev.keyboard.keycode){
		case ALLEGRO_KEY_ESCAPE:
			//exit game
			return false;

		}
	}
	return true;
} // end-of-function DisplayTextAndWaitBegin

/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 22, 2017
   @mname   DisplayTextAndWaitBegin
   @details
	  Returns false if escape key is pressed\n
  --------------------------------------------------------------------------
 */
static bool
DisplayTextAndWaitRoundWin(PongData* p) {

	TRACE();
	char textBuffer[255];
	if(p->roundWinner->score == p->maxscore) {
		sprintf(textBuffer, "%s Wins The Game!!",p->roundWinner->name);
		int next = DrawText(p, textBuffer, p->display.width/2, p->display.height/4, largeFont_c);
		sprintf(textBuffer, "Score: %s %d %s %d",p->p2.name, p->p2.score, p->p1.name, p->p1.score);
		DrawText(p, textBuffer, p->display.width/2, next, regularFont_c);
		PlaySound(p->winsample);
		sprintf(textBuffer, "[Mode: %s Level: %d %s %d %s %d]",(p->arcade?"Arcade":"Human"), p->level, p->p2.name, p->p2.score, p->p1.name, p->p1.score);
		recordResult(textBuffer);
		SetHalIntelligence(p);
		p->p2.score = 0;
		p->p1.score = 0;
	}
	else {
		sprintf(textBuffer, "%s Wins The Round!! Score: %s %d %s %d",p->roundWinner->name, p->p2.name, p->p2.score, p->p1.name, p->p1.score);
		int next = DrawText(p, textBuffer, p->display.width/2, p->display.height/4, regularFont_c);
		char buffer[100];
		sprintf(buffer, "First to %d Wins!", p->maxscore);
		next = DrawText(p, buffer, p->display.width/2, next, regularFont_c);
		DEBUG(" =======\n");
	}


	DrawText(p, (char*)"Press a key to begin or ESC to exit", p->display.width/2, p->display.height/2, regularFont_c);
	al_flip_display();
	al_flush_event_queue(p->eventqueue);
	al_wait_for_event(p->eventqueue, &(p->ev));
	if(ProcessKeyPress(p) == false) return false;
	for (int i = 0; i < 2; i++ ) {
		p->p1.keyPress[i] = false;
		p->p2.keyPress[i] = false;
	} //end-of-for
	al_flush_event_queue(p->eventqueue);


	return true;
} // end-of-function DisplayTextAndWaitBegin

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
	TRACE();
	al_draw_bitmap(g->bmap, g->xposition, g->yposition, 0);

} // end-of-function DrawBitmap

/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 28, 2017
   @mname   DrawBitmapSection
   @details
	  \n
  --------------------------------------------------------------------------
 */
static void
DrawBitmapSection(GameEntity* g) {
	TRACE();
	al_draw_bitmap_region(g->bmap, 0, 0, g->width, g->height, g->xposition, g->yposition, 0);

} // end-of-function DrawBitmapSection


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

	TRACE();
	SetBackgroundColor(*(p->bcolor));
	DrawBitmapSection(&(p->p1.ge));
	DrawBitmapSection(&(p->p2.ge));
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

	TRACE();
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

	TRACE();
	if ((p->ball.xposition >= (p->display.width-p->ball.width)) &&(p->ball.xspeed > 0)){
		p->p2.score++;
		p->roundWinner = &(p->p2);
		return true;

	}
	else if ((p->ball.xposition <= 0) && (p->ball.xspeed < 0)){
		TRACE();
		p->p1.score++;
		p->roundWinner = &(p->p1);
		return true;
	}
	return false;
} // end-of-function CheckSideCollitions


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
	TRACE();
	if(s) al_play_sample(s, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
} // end-of-function PlaySound



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

	TRACE();
	al_stop_timer(p->timer);
	if(p->arcade) al_stop_timer(p->hal9000);

	InitialPosition(p);
	DrawObjects(p);

	if(DisplayTextAndWaitRoundWin(p) == false) {
		return false;
	}
	else {
		al_start_timer(p->timer);
		if(p->arcade) al_start_timer(p->hal9000);
		PlaySound(p->startsample);
	}

	return true;
} // end-of-function PrintRoundWinner



/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 28, 2017
   @mname   SignOfNumber
   @details
	  \n
  --------------------------------------------------------------------------
 */
static int
SignOfNumber(int value) {

	if (value >= 0) {
		return 1;
	} //end-of-if(value > 0)
	return -1;
} // end-of-function SignOfNumber



/**
  ---------------------------------------------------------------------------
   @author  dwlambiri
   @date    May 28, 2017
   @mname   PaletteBounceCalc
   @details
	  \n
  --------------------------------------------------------------------------
 */
static void
PaletteBounceCalc(GameEntity* ball, Player* p, int maxballspeed, int level) {
	TRACE();
	int newxspeed = abs(ball->xspeed) + (rand()% (minballspeed_c / 2));
	if (newxspeed > maxballspeed) newxspeed = maxballspeed;
	ball->xspeed = SignOfNumber(ball->xspeed) *-1 *newxspeed;

	static const int zones_c = 8 - level;
	if(level == maxlevel_c) {
		ball->yspeed += 5;
	}
	else {
		int zonelength = p->ge.height/zones_c;
		int zonenum = (ball->yposition - p->ge.yposition) / zonelength;
		if (zonenum < 0) {
			zonenum = 0;
		}
		if (zonenum > zones_c -1) {
			zonenum = zones_c -1;
		} //end-of-if(zonenum > zones_c -1)

		ball->yspeed += 5*(zonenum - zones_c / 2);
	}
	PlaySound(p->sample);



} // end-of-function PaletteBounceCalc


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
	TRACE();
	if (    p->ball.xposition+ p->ball.width>= p->p1.ge.xposition &&
			p->ball.yposition + p->ball.height >= p->p1.ge.yposition  &&
			p->ball.yposition  <= p->p1.ge.yposition + p->p1.ge.height){

			PaletteBounceCalc(&(p->ball), &(p->p1), p->maxballspeed, p->level);
			return true;
	}

	else if (   p->ball.xposition < p->p2.ge.xposition +p->p2.ge.width &&
			p->ball.yposition + p->ball.height >= p->p2.ge.yposition  &&
			p->ball.yposition  <= p->p2.ge.yposition + p->p2.ge.height){

		    PaletteBounceCalc(&(p->ball), &(p->p2), p->maxballspeed, p->level);
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
	TRACE();
	p->ball.xposition = p->ball.xposition + p->ball.xspeed;
	p->ball.yposition = p->ball.yposition + p->ball.yspeed;

	if(CheckPaletteCollision(p) == false) {
		if(CheckSideCollitions(p) == true) return true;
	}
	CheckTopBottomCollision(p);

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
   @date    May 31, 2017
   @mname   SetHalIntelligence
   @details
	  \n
  --------------------------------------------------------------------------
 */
static void
SetHalIntelligence(PongData* p) {

   if(p->p1.score > p->p2.score + maxdiff_c) {
	   condp =  cond2;
	   valptr = val2;
   }
   if(p->p1.score + maxdiff_c <  p->p2.score ) {
	   condp =  cond1;
	   valptr = val1;
   }


} // end-of-function SetHalIntelligence



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

	TRACE();
    	//update only when ball moves towards the player
	if(p->ball.xspeed > 0) return;
	float mult = 1;
	if(p->ball.xposition > p->display.width/condp[0]) mult = valptr[0];
	if(p->ball.xposition <= p->display.width/condp[1]) mult = valptr[1];
	if(p->ball.xposition <= p->display.width/condp[2]) mult = valptr[2];
	if(p->ball.xposition <= p->display.width/condp[3]) mult = valptr[3];
	if(p->ball.xposition <= p->display.width/condp[4]) mult = valptr[4];
	if(p->ball.xposition < p->p2.ge.width) mult =  0;
	if(p->ball.yspeed > 0) {
		if(p->ball.yposition > (p->p2.ge.yposition + p->p2.ge.height/2)  ){
			float f = p->p2.paddleSpeed*mult;
			DEBUG2("HAL Moving DOWN", (int)f);
			p->p2.ge.yposition += (int)f;
		}
		else if(p->ball.yposition < p->p2.ge.yposition){
			float f = p->p2.paddleSpeed*mult;
			DEBUG2("HAL Moving UP", (int)f);
			p->p2.ge.yposition -= (int) f;
		}
		if(p->p2.ge.yposition >= (p->display.height - p->p2.ge.height))
			p->p2.ge.yposition = (p->display.height - p->p2.ge.height);
	}
	else {
		if(p->ball.yposition < (p->p2.ge.yposition + p->p2.ge.height/2) ) {
			float f = p->p2.paddleSpeed*mult;
			DEBUG2("HAL Moving UP", (int)f);
			p->p2.ge.yposition -= (int) f;
		}
		else if(p->ball.yposition > (p->p2.ge.yposition + p->p2.ge.height)){
			float f = p->p2.paddleSpeed*mult;
			DEBUG2("HAL Moving DOWN", (int)f);
			p->p2.ge.yposition += (int)f;
		}
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

	TRACE();
	al_start_timer(p->timer);
	if(p->arcade == true) {
		al_start_timer(p->hal9000);
	}

	bool roundwin = false;
	int skipCounter = 0;
	int maxSkip = 45;
	PlaySound(p->startsample);
	while (true){

		al_wait_for_event(p->eventqueue, &(p->ev));

		if(p->ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
					return false;
		}
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
			MovePaddles(p);
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

	TRACE();
	al_rest(0.0);
	al_destroy_display(p->display.display);
	al_destroy_timer(p->hal9000);
	al_destroy_timer(p->timer);
	al_destroy_event_queue(p->eventqueue);
	for (int  i = 0; i < maxfonts_c; i++ ) {
		if (p->font[i]) {
			al_destroy_font(p->font[i]);
		} //end-of-if(p->font[i])
	} //end-of-for

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

	TRACE();
	PongData* p = &pong;

	strcpy(p->p1.name, "Player1");
	strcpy(p->p2.name, "Player2");
	strcpy(p->fontFileName, FONTNAME);
	strcpy(p->p1.audioFileName, P1SOUND);
	strcpy(p->p2.audioFileName, P2SOUND);
	strcpy(p->p1.ge.bitmapFileName, P1FNAME);
	strcpy(p->p2.ge.bitmapFileName, P2FNAME);
	strcpy(p->ball.bitmapFileName, BALLFNAME);

	p->bcolor = &(p->bcolorarray[yellow_c]);

	//loop that processes the command line arguments.
	//argc is the size of the argument's array and argv is the array itself
	for ( int param = 0; param < argc; param++ ) {
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
		else if(strcmp(argv[param],"maxballspeed")==0) {
			//level
			if(++param < argc) {
				p->maxballspeed = atoi(argv[param]);
				if (p->maxballspeed <= maxballspeed_c) p->maxballspeed = maxballspeed_c;
			}
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
		else if(strcmp(argv[param],"fontfile")==0) {
			//font file name
			if(++param < argc) strcpy(p->fontFileName, argv[param]);
		}
		else if(strcmp(argv[param],"winSound")==0) {
			//font file name
			if(++param < argc) strcpy(p->winSoundFile, argv[param]);
		}
		else if(strcmp(argv[param],"player1bmp")==0) {
			//player 1 bitmap file name
			if(++param < argc) strcpy(p->p1.ge.bitmapFileName, argv[param]);
		}
		else if(strcmp(argv[param],"player2bmp")==0) {
			//player 2 bitmap file name
			if(++param < argc) strcpy(p->p2.ge.bitmapFileName, argv[param]);
		}
		else if(strcmp(argv[param],"ballbmp")==0) {
			//ball bitmap file name
			if(++param < argc) strcpy(p->ball.bitmapFileName, argv[param]);
		}
		else if(strcmp(argv[param],"player1sound")==0) {
			//player 1 sound file name
			if(++param < argc) strcpy(p->p1.audioFileName, argv[param]);
		}
		else if(strcmp(argv[param],"player2sound")==0) {
			//player 2 sound file name
			if(++param < argc) strcpy(p->p2.audioFileName, argv[param]);
		}
		else if(strcmp(argv[param],"p1paddleSpeed")==0) {
			//player 1 paddle speed
			if(++param < argc) p->p1.paddleSpeed = atoi(argv[param]);
		}
		else if(strcmp(argv[param],"p2paddleSpeed")==0) {
			//player 2 paddle speed
			if(++param < argc) p->p2.paddleSpeed = atoi(argv[param]);
		}
		else if(strcmp(argv[param],"level")==0) {
			//level (controls the paddle size)
			if(++param < argc){
				p->level = atoi(argv[param]);
				if(p->level>maxlevel_c) p->level = maxlevel_c;
			}
		}
		else if(strcmp(argv[param],"colourscheme")==0) {
			//player 2 bitmap file name
			if(++param < argc) {
				switch (argv[param][0]) {
					case 'y':
						p->bcolor = &(p->bcolorarray[yellow_c]);
						break;
					case 'b':
						p->bcolor = &(p->bcolorarray[blue_c]);
						break;
					case 'w':
						p->bcolor = &(p->bcolorarray[white_c]);
						break;
					case 'g':
						p->bcolor = &(p->bcolorarray[green_c]);
						break;
					default:
						break;
				} //end-switch(argv[param][0])
			}
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
	TRACE();
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
	if (LoadFont(p, smallFont_c) == false) {
		return false;
	} //end-of-if(LoadFont(p, smallFont_c) == false)

	if (LoadFont(p, regularFont_c) == false) {
		return false;
	} //end-of-if(LoadFont(p, regularFont_c) == false)

	if (LoadFont(p, largeFont_c) == false) {
		return false;
	} //end-of-if(LoadFont(p, largeFont_c) == false)


	//IF game is in arcade mode, player 2 is HAL9000
	if(p->arcade ==true) {
		strcpy(p->p2.name, halname);
	}

	p->display.display = al_create_display(p->display.width, p->display.height);
	p->bcolorarray[yellow_c] = al_map_rgb(255, 255, 0);
	p->bcolorarray[blue_c] = al_map_rgb(200, 200, 255);
	p->bcolorarray[white_c] = al_map_rgb(255, 255, 255);
	p->bcolorarray[green_c] = al_map_rgb(0, 255, 0);

	p->fcolor = al_map_rgb(0, 0, 0);
	p->timer = al_create_timer(REFRESHTIME);
	p->eventqueue = al_create_event_queue();
	if(al_is_event_queue_empty(p->eventqueue) == false) ERROR("Event queue not empty after creation");

	al_register_event_source(p->eventqueue, al_get_keyboard_event_source());
	al_register_event_source(p->eventqueue, al_get_display_event_source(p->display.display));
	al_register_event_source(p->eventqueue, al_get_timer_event_source(p->timer));
	if(p->arcade == true) {
		printf("Arcade Mode Detected\n");
		p->hal9000 = al_create_timer(1.0/(float) p->p2.paddleSpeed);
		al_register_event_source(p->eventqueue, al_get_timer_event_source(p->hal9000));
	}
	else p->hal9000 = NULL;

	if(LoadPlayerBitmap(&(p->p1.ge), p->level) == false) return false;
	if(LoadPlayerBitmap(&(p->p2.ge), p-> level) == false) return false;
	if(LoadBitmap(&(p->ball)) == false) return false;

	LoadAudio(&(p->p1));
	LoadAudio(&(p->p2));
	LoadWinAudio(p);

	InitialPosition(p);

	SetBackgroundColor(*(p->bcolor));

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

	TRACE();
	PongData* p = &pong;
	SetBackgroundColor(*(p->bcolor));
	if(DisplayTextAndWaitBegin(p) == true) {
		GameLoop(p);
	}

	GameExit(p);
} // end-of-function GameRun

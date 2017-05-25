/*
 * d-allegro.h
 *
 *  Created on: May 22, 2017
 *      Author: dwlambiri
 */

#ifndef D_ALLEGRO_H_
#define D_ALLEGRO_H_

#include <allegro5/allegro.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro5.h>
#include <allegro5/color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#define SCREEN_W 1600
#define SCREEN_H 1200
#define PLAYEROFFSET 100
#define FRAMERATE 60.0
#define REFRESHTIME 1.0/FRAMERATE
#define PLAYERSPEED 10.0
#define COMPUTERSPEED 10.0
#define SOMENUMBER 11
#define FONTSIZE 24
#define LEVEL 10
#define MAXNAME 200
#define MAXSCORE 10


#define P1FNAME "player1.png"
#define P2FNAME "player2.png"
#define BALLFNAME "ball.png"
#define P1SOUND  "p1sound.ogg"
#define P2SOUND  "p2sound.ogg"


/**
  ---------------------------------------------------------------------------
   @author     dwlambiri
   @date       May 22, 2017
   @name       GameEntity
   @details
	\n
  ---------------------------------------------------------------------------
 */
struct GameEntity {
	int xposition;
	int yposition;
	int xspeed;
	int yspeed;
	int width;
	int height;
	ALLEGRO_BITMAP* bmap;
	char* fname;
};

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
struct Player {
	unsigned int score;
	struct GameEntity ge;
	char name[MAXNAME];
	ALLEGRO_SAMPLE *sample;
};


/**
  ---------------------------------------------------------------------------
   @author     dwlambiri
   @date       May 22, 2017
   @name       Display
   @details
	\n
  ---------------------------------------------------------------------------
 */
struct Display {
	int width;
	int height;
	ALLEGRO_DISPLAY *display;
};

/**
  ---------------------------------------------------------------------------
   @author     dwlambiri
   @date       May 22, 2017
   @name       PongData
   @details
	\n
  ---------------------------------------------------------------------------
 */
struct PongData {
	struct Player p1;
	struct Player p2;
	struct GameEntity   ball;
	struct Display display;
	bool   arcade;
	int    level;
	struct Player* roundWinner;
	int    fontsize;
	unsigned int maxscore;

	ALLEGRO_EVENT ev;
	ALLEGRO_EVENT_QUEUE *eventqueue;
	ALLEGRO_TIMER *timer;
	ALLEGRO_TIMER *hal9000;
	ALLEGRO_FONT *font;
	ALLEGRO_COLOR bcolor;
	ALLEGRO_COLOR fcolor;
};

bool InitGame(struct PongData* p, float refreshtime);
void GameRun(struct PongData* p);

#endif /* D_ALLEGRO_H_ */

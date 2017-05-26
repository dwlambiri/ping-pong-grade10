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

#define MAXNAME 200

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

bool InitGame();
void GameRun();
struct PongData* CreateGameData();

#endif /* D_ALLEGRO_H_ */

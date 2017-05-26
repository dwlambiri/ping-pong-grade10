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


bool InitGame();
void GameRun();
bool CreateGameData(int argc, char **argv);

#endif /* D_ALLEGRO_H_ */

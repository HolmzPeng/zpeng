/***************************************************************************
 *   Copyright (C) 2004 by tiancai_pgy                                     *
 *   pgy5@163.com                                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "public.h"
/* ----------------------------------- */
int bg[][34] = {
                   OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
                   OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
                   OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
                   OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
                   OO, OO, A1, A2, OO, OO, A1, A2, OO, OO, B1, B2, B2, B2, B2, B3, OO, OO, C1,
                   C2, C2, C2, C2, C3, OO, OO, A1, A2, OO, OO, A1, A2, OO, OO,
                   OO, OO, A3, A4, OO, OO, A3, A4, OO, OO, B4, B5, B5, B5, B5, B6, OO, OO, C4,
                   C5, C5, C5, C5, C6, OO, OO, A3, A4, OO, OO, A3, A4, OO, OO,
                   OO, OO, A3, A4, OO, OO, A3, A4, OO, OO, B1, B2, B2, B2, B2, B3, OO, OO, C1,
                   C2, C2, C2, C2, C3, OO, OO, A3, A4, OO, OO, A3, A4, OO, OO,
                   OO, OO, A5, A6, OO, OO, A5, A6, OO, OO, B4, B5, B5, B5, B5, B6, OO, OO, C4,
                   C5, C5, C5, C5, C6, OO, OO, A5, A6, OO, OO, A5, A6, OO, OO,
                   OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, HH, B1, B2, B2, B2, B3, OO, OO, C1,
                   C2, C2, C2, C3, HH, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
                   OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, HH, B4, B5, B5, B5, B6, OO, OO, C4,
                   C5, C5, C5, C6, HH, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
                   OO, OO, A1, A2, OO, OO, A1, A2, OO, OO, B1, B2, B2, B2, B2, B3, OO, OO, C1,
                   C2, C2, C2, C2, C3, OO, OO, A1, A2, OO, OO, A1, A2, OO, OO,
                   OO, OO, A3, A4, OO, OO, A3, A4, OO, OO, B4, B5, B5, B5, B5, B6, OO, OO, C4,
                   C5, C5, C5, C5, C6, OO, OO, A3, A4, OO, OO, A3, A4, OO, OO,
                   OO, OO, A3, A4, OO, OO, A3, A4, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
                   OO, OO, OO, OO, OO, OO, OO, A3, A4, OO, OO, A3, A4, OO, OO,
                   OO, OO, A5, A6, OO, OO, A5, A6, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
                   OO, OO, OO, OO, OO, OO, OO, A5, A6, OO, OO, A5, A6, OO, OO,
                   OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, A1, A2, OO, OO, HH, HH, HH, HH, HH,
                   HH, OO, OO, A1, A2, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
                   OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, A3, A4, OO, OO, HH, HH, HH, HH, HH,
                   HH, OO, OO, A3, A4, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
                   HH, HH, OO, OO, D1, D2, D3, D4, OO, OO, A3, A4, OO, OO, E1, E2, E2, E2, E2,
                   E3, OO, OO, A3, A4, OO, OO, D1, D2, D3, D4, OO, OO, HH, HH,
                   HH, HH, OO, OO, D5, D6, D7, D8, OO, OO, A3, A4, OO, OO, E4, E5, E5, E5, E5,
                   E6, OO, OO, A3, A4, OO, OO, D5, D6, D7, D8, OO, OO, HH, HH,
                   OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, A3, A4, OO, OO, E4, E5, E5, E5, E5,
                   E6, OO, OO, A3, A4, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
                   OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, A5, A6, OO, OO, E7, E8, E8, E8, E8,
                   E9, OO, OO, A5, A6, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
                   OO, OO, A1, A2, OO, OO, A1, A2, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
                   OO, OO, OO, OO, OO, OO, OO, A1, A2, OO, OO, A1, A2, OO, OO,
                   OO, OO, A3, A4, OO, OO, A3, A4, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
                   OO, OO, OO, OO, OO, OO, OO, A3, A4, OO, OO, A3, A4, OO, OO,
                   OO, OO, A3, A4, OO, OO, A3, A4, OO, OO, A1, A2, OO, OO, D1, D2, D3, D2, D3,
                   D4, OO, OO, A1, A2, OO, OO, A3, A4, OO, OO, A3, A4, OO, OO,
                   OO, OO, A5, A6, OO, OO, A5, A6, OO, OO, A5, A6, OO, OO, D5, D6, D7, D6, D7,
                   D8, OO, OO, A5, A6, OO, OO, A5, A6, OO, OO, A5, A6, OO, OO,
                   OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
                   OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
                   OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
                   OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
                   OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, G1, G2, G3, G4,
                   OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
                   OO, OO, OO, OO, OO, OO, OO, OO, B1, B2, B2, B3, HH, OO, OO, G5, G6, G7, G8,
                   OO, OO, HH, C1, C2, C2, C3, OO, OO, OO, OO, OO, OO, OO, OO,
                   OO, OO, OO, OO, OO, OO, OO, OO, B4, B5, B5, B6, HH, OO, OO, G9, Ga, Gb, Gc,
                   OO, OO, HH, C4, C5, C5, C6, OO, OO, OO, OO, OO, OO, OO, OO
               };

int Sound = 1;
Mix_Chunk *Snd_bom, *Snd_ding, *Snd_dow;
GameState game_state;
GameEngine game_engine;
KeyState key_state;
Sprite_t player[2];
Sprite_t enemy[MAX_ENEMYS];
Sprite_t bullet[MAX_ENEMYS + 4];
Sprite_t explode[4];
int players=0;
main ()
{
	game_engine.FULLSCREEN = 0;
again:
	game_state = GAME_START;
	while (game_state != GAME_EXIT)
	{
		switch (game_state)
		{
		case GAME_START:
			game_state = GAME_MENU;
			GameEngine_Init (&game_engine);
			break;
		case GAME_MENU:
			printf("Run GameMenu\n");
			Game_Menu();
			SDL_SetEventFilter (EVENT_FILTER);	//this function must after the function Game_Menu()
			game_state = GAME_RUNNING;
			break;
		case GAME_RUNNING:
			printf ("Game running ...\n");
			Load_Data (&game_engine);
			Play_Game (&game_engine);
			break;
		case GAME_RESET:
			break;
		case GAME_WIN:
			/* It will be changed future version */
			//sleep(3);
			//ClearUp(&game_engine);
			game_engine.level++;
			if (game_engine.level > MAX_LEVELS - 1)
				game_engine.level = 0;
			game_state = GAME_RUNNING;
			break;
		case GAME_OVER:
			/* It will be changed future version */
			//sleep(3);
			//ClearUp(&game_engine);
			game_state = GAME_RUNNING;
			break;
		case GAME_EXIT:
			GameEngine_Close (&game_engine);
			break;
		}
	}
	SDL_SetEventFilter (NULL);
	goto again;
}


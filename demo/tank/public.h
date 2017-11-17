/*
        tank-sdl - tank.c
        Copyright (c) 2004 by lanche <lanche@163.com>
 ___________________________________________________________________________
        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2, or (at your option)
        any later version.
 
        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.
 
        You should have received a copy of the GNU General Public License
        along with this program; if not, write to the Free Software
        Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 
        See the file COPYING in the same directory for more information.
*/

#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <time.h>
#define WIDTH    640
#define HEIGHT   480
#define BPP      16

#define PLANE_W  34
#define PLANE_H  27
#define TILE_W   16
#define TILE_H   16


/* pic's code */
#define RV -1
#define OO 0
#define A1 1
#define A2 2
#define A3 10
#define A4 11
#define A5 19
#define A6 20
#define B1 28
#define B2 29
#define B3 30
#define B4 37
#define B5 38
#define B6 39
#define C1 31
#define C2 32
#define C3 33
#define C4 40
#define C5 41
#define C6 42
#define D1 46
#define D2 47
#define D3 48
#define D4 49
#define D5 55
#define D6 56
#define D7 57
#define D8 58
#define E1 3
#define E2 4
#define E3 5
#define E4 12
#define E5 13
#define E6 14
#define E7 21
#define E8 22
#define E9 23
#define I1 34
#define I2 35
#define I3 36
#define I4 43
#define I5 44
#define I6 45
#define G1 6
#define G2 7
#define G3 8
#define G4 9
#define G5 15
#define G6 16
#define G7 17
#define G8 18
#define G9 24
#define Ga 25
#define Gb 26
#define Gc 27
#define HH 50
#define TT 59

/* Define direct */
#define DIR_UP    0
#define DIR_LEFT  1
#define DIR_DOWN  2
#define DIR_RIGHT 3

#define MAX_ENEMYS 10
#define MAX_LEVELS 4

#define TRUE   1
#define FALSE  0



typedef enum GameState
{
    GAME_START,
    GAME_MENU,
    GAME_RUNNING,
    GAME_RESET,
    GAME_WIN,
    GAME_OVER,
    GAME_EXIT
} GameState;

typedef struct KeyState
{
	int Up[2], Down[2], Left[2], Right[2], Fire[2], Move[2], F4, Quit, AnyKey, Debug;
}
KeyState;

typedef struct GameEngine
{
	SDL_Surface *screen,
	*swapscreen,
	*bg1, *bg2,
	*tiles, *player1, *player2, *enemys, *bullets, *explodes, *bigtank;	//player1 分别为图像
	SDL_Event event;
	int FULLSCREEN;
	Uint32 last_time;
	Uint32 now_time;
	int max_enemys;
	int bore_enemy;
	int enemy_left;
	int player_left[2];	//left life
	int clock;
	int level;            	//级别
	Uint32 score[2];        //分数
	int enemy_pos[5][2];
}
GameEngine;

typedef struct Sprite_t
{
	int id;		//标记为哪个坦克
	int type;
	int armor;
	int width;
	int height;
	int x;
	int y;
	int ox;
	int oy;
	int boring;
	int active;
	int dir;
	int speed;
	int timer1;			/* use to show subframe */
	int clock;
	SDL_Surface *image;
}
Sprite_t;

extern int Sound;
extern Mix_Chunk *Snd_bom, *Snd_ding, *Snd_dow;
extern GameState game_state;
extern GameEngine game_engine;
extern KeyState key_state;
extern Sprite_t player[2];
extern Sprite_t enemy[MAX_ENEMYS];
extern Sprite_t bullet[MAX_ENEMYS + 4];
extern Sprite_t explode[4];
extern int bg[][34];
extern int players;
void GameEngine_Init (GameEngine * ge);
void GameEngine_Close (GameEngine * ge);
int Video_Init (GameEngine * ge);
void Video_Set (GameEngine * ge);
void Video_Reset (GameEngine * ge);
void Video_Close (GameEngine * ge);
void Game_Menu ();
void PlaySound (Mix_Chunk * snd);
int EVENT_FILTER (const SDL_Event * event);
void Load_Data (GameEngine * ge);
void Play_Game (GameEngine * ge);
void BorePlayer (GameEngine * ge, Sprite_t * sprite,int id);
void BoreEnemy (GameEngine * ge);
void BoreBigTank (GameEngine * ge, int type);
int MoveAble (Sprite_t * sprite, int x, int y);
int PlaceAble (Sprite_t * tank);
void DrawSprite (GameEngine * ge, Sprite_t * sprite);
void ClearSprite (GameEngine * ge, Sprite_t * sprite);
void ExplodeSprite (Sprite_t * sprite);
void ChangeDirection (Sprite_t * sprite, int direction);
void PlayerFire (GameEngine * ge, Sprite_t * bullet,int id);
void EnemyFire (GameEngine * ge, Sprite_t * bullet, int index);
void BigTankFire (GameEngine * ge, Sprite_t * bigtank, int index);
void Burst (GameEngine * ge, Sprite_t * object);
void Explode (GameEngine * ge, Sprite_t * tank);
void ExplodeHawk (GameEngine * ge);
void Disp_TankLeft (GameEngine * ge);
void Show_Score (GameEngine * ge);
void Add_Score (GameEngine * ge, int player, Sprite_t * tank);
void ClearUp (GameEngine * ge);
int HitTest (Sprite_t * obj1, Sprite_t * obj2);
int HitObject (Sprite_t * bullet, int x, int y);
void HitTile (GameEngine * ge, Sprite_t * bullet, int x, int y);
int InterSect (int x1, int y1, int x2,int  y2);
void ShowGameOver (GameEngine * ge);
void ShowRoundClear (GameEngine * ge);
Uint32 TimeLeft ();

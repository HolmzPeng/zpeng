#include "public.h"
/* ----------------------------------- */
Uint32
TimeLeft ()
{
	static Uint32 next_time = 0;
	Uint32 now;

	now = SDL_GetTicks ();
	if (next_time <= now)
	{
		next_time = now + 30;
		return (0);
	}
	return (next_time - now);
}

/* ----------------------------------- */
int
MoveAble (Sprite_t * sprite, int x, int y)
{

	static int lx[4] = { 4, 0, 4, 31 };
	static int ly[4] = { 0, 4, 31, 4 };
	static int mx[4] = { 16, 0, 16, 31 };
	static int my[4] = { 0, 16, 31, 16 };
	static int rx[4] = { 27, 0, 27, 31 };
	static int ry[4] = { 0, 27, 31, 27 };

	/*
	  static int  lx[4]={ 0, 0, 0,31 };
	  static int  ly[4]={ 0, 0,31, 0 };
	  static int  mx[4]={16, 0,16,31 };
	  static int  my[4]={ 0,16,31,16 };
	  static int  rx[4]={31, 0,31,31 };
	  static int  ry[4]={ 0,31,31,31 };
	*/

	int x1, y1, x2, y2, x3, y3;
	int row, col, dx, dy;

	/* Out off screen,return 0 */
	if (x < 0)
		return 0;
	if (y < 0)
		return 0;
	if (x > PLANE_W * 16 - 32)
		return 0;
	if (y > PLANE_H * 16 - 32)
		return 0;
	x1 = x + lx[sprite->dir];
	y1 = y + ly[sprite->dir];
	x2 = x + mx[sprite->dir];
	y2 = y + my[sprite->dir];
	x3 = x + rx[sprite->dir];
	y3 = y + ry[sprite->dir];

	/* Pass entirely */
	if (bg[y1 / 16][x1 / 16] == OO && bg[y2 / 16][x2 / 16] == OO
	        && bg[y3 / 16][x3 / 16] == OO)
		return 1;

	/* Can't move around */
	return 0;
}

/* ----------------------------------- */
int
PlaceAble (Sprite_t * tank)
{
	int i;
	for (i = 0; i < 2; i++)
	{
		if (!player[i].active)
			continue;
		if (tank == &player[i])
			continue;
		if (HitTest (tank, &player[i]))		//¿´ÊÇ·ñÊÇÅóÓÑ
			return FALSE;
	}
	for (i = 0; i < game_engine.max_enemys; i++)
	{
		if (!enemy[i].active)
			continue;
		if (tank == &enemy[i])
			continue;
		if (HitTest (tank, &enemy[i]))
			return FALSE;
	}

	return TRUE;
}
/* ----------------------------------- */
int
InterSect (int x1, int y1, int x2,int  y2)			//ÔÚ¹¥»÷·¶Î§!!!
{
	int dx, dy;
	dx = x1 - x2 > 0 ? x1 - x2 : x2 - x1;
	dy = y1 - y2 > 0 ? y1 - y2 : y2 - y1;
	if (dx < 32 && dy < 32)
		return 1;
	if (dx < 8 && dy < 8)
		return 2;
	return 0;
}

/* ----------------------------------- */
int
HitTest (Sprite_t * obj1, Sprite_t * obj2)
{
	int minX, minY, maxX, maxY;
	minX = obj1->x > obj2->x ? obj1->x : obj2->x;
	maxX =
	    obj1->x + obj1->width <
	    obj2->x + obj2->width ? obj1->x + obj1->width : obj2->x + obj2->width;
	minY = obj1->y > obj2->y ? obj1->y : obj2->y;
	maxY =
	    obj1->y + obj1->height <
	    obj2->y + obj2->height ? obj1->y + obj1->height : obj2->y + obj2->height;
	if (minX > maxX || minY > maxY)
		return 0;
	else
		return 1;
}

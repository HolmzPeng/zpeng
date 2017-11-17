#include "public.h"
/* -------------´ò¶«Î÷---------------------- */
int
HitObject (Sprite_t * bullet, int x, int y)
{
  static int lx[4] = { 0, 0, 0, 12 };
  static int ly[4] = { 0, 0, 12, 0 };
  static int rx[4] = { 12, 0, 12, 12 };
  static int ry[4] = { 0, 12, 12, 12 };
  int x1, y1, x2, y2;
  x1 = x + lx[bullet->dir];
  y1 = y + ly[bullet->dir];
  x2 = x + rx[bullet->dir];
  y2 = y + ry[bullet->dir];

  /* Out off screen then return -1 */
  if (x < 0)
    return -1;
  if (y < 0)
    return -1;
  if (x > PLANE_W * 16 - 16)
    return -1;
  if (y > PLANE_H * 16 - 14)
    return -1;

  /* Can move around */
  if (bg[y1 / 16][x1 / 16] == OO && bg[y2 / 16][x2 / 16] == OO)
    return 0;

  /* Pass over river */
  if (bg[y1 / 16][x1 / 16] == RV && bg[y2 / 16][x2 / 16] == RV)
    return 0;

  if (bg[y1 / 16][x1 / 16] == OO && bg[y2 / 16][x2 / 16] == RV)
    return 0;
  if (bg[y1 / 16][x1 / 16] == RV && bg[y2 / 16][x2 / 16] == OO)
    return 0;

  /* Hit the HH */
  if (bg[y1 / 16][x1 / 16] == HH && bg[y2 / 16][x2 / 16] == HH)
    return 9;

  /* Hit the hawk! */
  //if(x1>=16*16 && x1<18*16 && y1>24*16-8 && y1<27*16)return 8;
  if (bg[y1 / 16][x1 / 16] == G6 || bg[y2 / 16][x2 / 16] == G6)
    return 8;
  if (bg[y1 / 16][x1 / 16] == G7 || bg[y2 / 16][x2 / 16] == G7)
    return 8;
  if (bg[y1 / 16][x1 / 16] == Ga || bg[y2 / 16][x2 / 16] == Ga)
    return 8;
  if (bg[y1 / 16][x1 / 16] == Gb || bg[y2 / 16][x2 / 16] == Gb)
    return 8;

  /* Against the wall */
  return 1;
}

/* ----------------------------------- */
void
HitTile (GameEngine * ge, Sprite_t * bullet, int x, int y)
{
  SDL_Rect res_rect, des_rect;
  static int lx[4] = { 0, 0, 0, 12 };
  static int ly[4] = { 0, 0, 12, 0 };
  static int rx[4] = { 12, 0, 12, 12 };
  static int ry[4] = { 0, 12, 12, 12 };
  int x1, y1, x2, y2;
  x1 = (x + lx[bullet->dir]) / 16;
  y1 = (y + ly[bullet->dir]) / 16;
  x2 = (x + rx[bullet->dir]) / 16;
  y2 = (y + ry[bullet->dir]) / 16;
  if (bg[y1][x1] != OO)
    {
      if (bg[y1][x1] == HH)
	return;
      if (bg[y1][x1] > 63)
	bg[y1][x1] = OO;
      else
	{
	  bg[y1][x1] += 63;
	  res_rect.w = 16;
	  res_rect.h = 16;
	  res_rect.x = ((bg[y1][x1] - 1) % 9) * 16;
	  res_rect.y = ((bg[y1][x1] - 1) / 9) * 16;
	}
      des_rect.w = 16;
      des_rect.h = 16;
      des_rect.x = x1 * 16;
      des_rect.y = y1 * 16;
      SDL_BlitSurface (ge->bg1, &des_rect, ge->bg2, &des_rect);
      SDL_BlitSurface (ge->bg1, &des_rect, ge->screen, &des_rect);
      if (bg[y1][x1] != OO)
	{
	  SDL_BlitSurface (ge->tiles, &res_rect, ge->bg2, &des_rect);
	  SDL_BlitSurface (ge->tiles, &res_rect, ge->screen, &des_rect);
	}
    }
  if (x1 == x2 && y1 == y2)
    return;
  if (bg[y2][x2] != OO)
    {
      if (bg[y2][x2] == HH)
	return;
      if (bg[y2][x2] > 63)
	bg[y2][x2] = OO;
      else
	{
	  bg[y2][x2] += 63;
	  res_rect.w = 16;
	  res_rect.h = 16;
	  res_rect.x = ((bg[y2][x2] - 1) % 9) * 16;
	  res_rect.y = ((bg[y2][x2] - 1) / 9) * 16;
	}
      des_rect.w = 16;
      des_rect.h = 16;
      des_rect.x = x2 * 16;
      des_rect.y = y2 * 16;
      SDL_BlitSurface (ge->bg1, &des_rect, ge->bg2, &des_rect);
      SDL_BlitSurface (ge->bg1, &des_rect, ge->screen, &des_rect);
      if (bg[y2][x2] != OO)
	{
	  SDL_BlitSurface (ge->tiles, &res_rect, ge->bg2, &des_rect);
	  SDL_BlitSurface (ge->tiles, &res_rect, ge->screen, &des_rect);
	}
    }
}

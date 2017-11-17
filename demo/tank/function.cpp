#include "public.h"
/* ----------------------------------- */
void
ChangeDirection (Sprite_t * sprite, int direction)
{
  int dir;
  int row, col;
  int dx, dy;

  dir = direction < 0 ? (int) (4.0 * rand () / (RAND_MAX + 1.0)) : direction;
  if (dir == sprite->dir)
    return;
  sprite->dir = dir;
  row = sprite->x / TILE_W;
  col = sprite->y / TILE_H;
  dx = sprite->x % TILE_W;
  dy = sprite->y % TILE_H;
  if (dir == DIR_UP || dir == DIR_DOWN)
    {
      if (dx < 4)
	sprite->x = row * TILE_W;
      if (dx > 12)
	sprite->x = (row + 1) * TILE_W;
    }
  else if (dir == DIR_LEFT || dir == DIR_RIGHT)
    {
      if (dy < 4)
	sprite->y = col * TILE_H;
      if (dy > 12)
	sprite->y = (col + 1) * TILE_H;
    }
}





/* ----------------------------------- */
void BorePlayer (GameEngine * ge, Sprite_t * sprite,int id)
{
  if (ge->player_left[id] < 0)
    return;
  sprite->armor = 0;
  sprite->width = 32;
  sprite->height = 32;

  if(id==0)
  sprite->x = 13 * 16;
  else
  sprite->x = 19 * 16;

  sprite->y = 25 * 16;
  sprite->ox = sprite->x;
  sprite->oy = sprite->y;
  sprite->boring = 1;
  sprite->active = 1;
  sprite->dir = DIR_UP;
  sprite->speed = 3;
  sprite->type = 0;
  if(id==0)
  sprite->image = ge->player1;
  else
  sprite->image=ge->player2;
  //ge->player_left--;
  //if(ge->player_left<0)ge->player_left=4;
}

/* ----------------------------------- */
void
PlayerFire (GameEngine * ge, Sprite_t * sprite,int id)
{
  int dx[4] = { 10, 0, 10, 16 };
  int dy[4] = { 0, 10, 16, 10 };
  if (bullet[id].active == 1)
    return;
  bullet[id].width = 12;
  bullet[id].height = 12;
  bullet[id].x = sprite->x + dx[sprite->dir];
  bullet[id].y = sprite->y + dy[sprite->dir];
  bullet[id].active = 1;
  bullet[id].dir = sprite->dir;
  bullet[id].speed = 7;
  bullet[id].timer1 = 0;
  bullet[id].image = ge->bullets;
}

/* ----------------------------------- */
void
EnemyFire (GameEngine * ge, Sprite_t * sprite, int index)
{
  int dx[4] = { 10, 0, 10, 16 };
  int dy[4] = { 0, 10, 16, 10 };
  int i;
  i = index + 4;
  if (bullet[i].active == 1)
    return;
  bullet[i].width = 12;
  bullet[i].height = 12;
  bullet[i].x = sprite->x + dx[sprite->dir];
  bullet[i].y = sprite->y + dy[sprite->dir];
  bullet[i].active = 1;
  bullet[i].dir = sprite->dir;
  bullet[i].speed = 5;
  bullet[i].timer1 = 0;
  bullet[i].image = ge->bullets;
}

/* ----------------------------------- */
void
BigTankFire (GameEngine * ge, Sprite_t * bigtank, int index)
{
  int i;
  i = index + 4;
  if (bullet[i].active == 1)
    return;
  bullet[i].width = 12;
  bullet[i].height = 12;
  bullet[i].x = bigtank->x + bigtank->width / 2;
  bullet[i].y = bigtank->y + bigtank->height;
  bullet[i].active = 1;
  bullet[i].dir = DIR_DOWN;
  bullet[i].speed = 5;
  bullet[i].timer1 = 0;
  bullet[i].image = ge->bullets;
}

/* ----------------------------------- */
void
BoreEnemy (GameEngine * ge)
{
  int i;
  int pos, dir;

  if (ge->bore_enemy < 1)
    return;

  for (i = 0; i < ge->max_enemys; i++)
    {
      if (enemy[i].active != 1)
	{
	  pos = (int) (5.0 * rand () / (RAND_MAX + 1.0));
	  dir = (int) (3.0 * rand () / (RAND_MAX + 1.0));
	  enemy[i].armor = 0;
	  enemy[i].width = 32;
	  enemy[i].height = 32;
	  //enemy[i].x=pos*32*4;
	  //pos=(int)(3.0*rand()/(RAND_MAX+1.0));
	  //enemy[i].y=pos*32;
	  enemy[i].x = ge->enemy_pos[pos][0] * 16;
	  enemy[i].y = ge->enemy_pos[pos][1] * 16;
	  enemy[i].ox = enemy[i].x;
	  enemy[i].oy = enemy[i].y;
	  enemy[i].boring = 1;
	  enemy[i].active = 1;
	  enemy[i].dir = DIR_DOWN;
	  if (ge->bore_enemy < 3)
	    {
	      enemy[i].armor = 2;
	      enemy[i].type = 2;
	      enemy[i].speed = 1;
	    }
	  else if (ge->bore_enemy < 7)
	    {
	      enemy[i].type = 1;
	      enemy[i].speed = 4;
	    }
	  else
	    {
	      enemy[i].type = 0;
	      enemy[i].speed = 3;
	    }
	  enemy[i].clock = 0;
	  enemy[i].image = ge->enemys;
	  ge->bore_enemy--;
	  return;
	}
    }
}

/* ----------------------------------- */
void
BoreBigTank (GameEngine * ge, int type)
{
  int i;
  for (i = ge->max_enemys; i < ge->max_enemys + 2; i++)
    {
      enemy[i].armor = 14;
      enemy[i].width = 128;
      enemy[i].height = 54;
      enemy[i].x = 48 + (i - ge->max_enemys) * 160;
      enemy[i].y = 10 + (i - ge->max_enemys) * 64;
      enemy[i].ox = enemy[i].x;
      enemy[i].oy = enemy[i].y;
      enemy[i].boring = 0;
      enemy[i].active = 1;
      enemy[i].dir = DIR_RIGHT;
      enemy[i].type = type;
      enemy[i].speed = 2;
      enemy[i].clock = 0;
      enemy[i].timer1 = 0;
      enemy[i].image = ge->bigtank;
    }
}

/* ----------------------------------- */
void
Explode (GameEngine * ge, Sprite_t * tank)
{
  int i, x, y;
  SDL_Rect rect;
  x = tank->x - 24;
  y = tank->y - 24;
  for (i = 0; i < 4; i++)
    {
      if (explode[i].active == 1)
	continue;
      explode[i].width = 80;
      explode[i].height = 80;
      explode[i].x = x;
      explode[i].y = y;
      explode[i].ox = x;
      explode[i].oy = y;
      explode[i].active = 1;
      explode[i].timer1 = 0;
      explode[i].image = ge->explodes;
      rect.w = 80;
      rect.h = 80;
      rect.x = explode[i].x;
      rect.y = explode[i].y;
    }
}

/* ----------------------------------- */
void
ExplodeHawk (GameEngine * ge)
{
  Sprite_t hawk;
  SDL_Rect rect;
  hawk.width = hawk.height = 32;
  hawk.ox = hawk.x = 16 * 16;
  hawk.oy = hawk.y = 25 * 16;

  rect.w = rect.h = 32;
  rect.x = hawk.x;
  rect.y = hawk.y;
  SDL_BlitSurface (ge->bg1, &rect, ge->screen, &rect);
  SDL_BlitSurface (ge->bg1, &rect, ge->bg2, &rect);
  hawk.active = 1;
  Explode (ge, &hawk);

  /* Clear bg flags */
  bg[25][16] = OO;
  bg[25][17] = OO;
  bg[26][16] = OO;
  bg[26][17] = OO;
}



/* ----------------------------------- */
void
Burst (GameEngine * ge, Sprite_t * object)
{
  ClearSprite (ge, object);
  if (object->ox != object->x || object->oy != object->y)
    {
      object->ox = object->x;
      object->oy = object->y;
      ClearSprite (ge, object);
    }
  object->active = 0;
}


/* ----------------------------------- */
void
Add_Score (GameEngine * ge, int player, Sprite_t * tank)
{
  int value[3] = { 400, 600, 1000 };
  if (tank->type < 10)
    ge->score[player] += value[tank->type];
  else
    ge->score[player] += 3000;
}

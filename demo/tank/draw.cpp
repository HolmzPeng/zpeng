#include "public.h"
/* ----------------------------------- */
void
DrawTile (int x, int y)
{
	int tile;
	SDL_Rect res_rect, des_rect;
	tile = bg[y][x] - 1;
	res_rect.w = 16;
	res_rect.h = 16;
	res_rect.x = (tile % 9) * 16;
	res_rect.y = (tile / 9) * 16;
	des_rect.w = 16;
	des_rect.h = 16;
	des_rect.x = x * 16;
	des_rect.y = y * 16;
	SDL_BlitSurface (game_engine.tiles, &res_rect, game_engine.screen,
	                 &des_rect);
}
/* ----------------------------------- */
void
ShowGameOver (GameEngine * ge)
{
	SDL_Surface *tmp;
	SDL_Rect rect;
	tmp = IMG_Load ("pics/gameover.png");
	SDL_SetColorKey (tmp, SDL_SRCCOLORKEY,
	                 SDL_MapRGB (tmp->format, 255, 0, 255));
	tmp = SDL_DisplayFormat (tmp);
	rect.w = 352;
	rect.h = 32;
	rect.x = 96;
	rect.y = 160;
	SDL_BlitSurface (tmp, NULL, ge->screen, &rect);
	SDL_FreeSurface (tmp);
}

/* ----------------------------------- */
void
ShowRoundClear (GameEngine * ge)
{
	SDL_Surface *tmp;
	SDL_Rect rect;
	tmp = IMG_Load ("pics/round-clear.png");
	SDL_SetColorKey (tmp, SDL_SRCCOLORKEY,
	                 SDL_MapRGB (tmp->format, 255, 0, 255));
	tmp = SDL_DisplayFormat (tmp);
	rect.w = 288;
	rect.h = 32;
	rect.x = 96;
	rect.y = 160;
	SDL_BlitSurface (tmp, NULL, ge->screen, &rect);
	SDL_FreeSurface (tmp);
}
/* ----------------------------------- */
void
DrawSprite (GameEngine * ge, Sprite_t * sprite)
{
	int frame;
	SDL_Rect grid, pos;
	frame = sprite->timer1 / 2;
	grid.w = pos.w = sprite->width;
	grid.h = pos.h = sprite->height;
	grid.x = frame * sprite->width + sprite->type * 2 * sprite->width;
	grid.y = sprite->dir * sprite->height;
	if (sprite->type == 11)
	{
		grid.x = 0;
		grid.y = (14 - sprite->armor) / 3 * sprite->height;
	}
	if (sprite->height == 12)
		grid.y = 0;
	pos.x = sprite->x;
	pos.y = sprite->y;
	SDL_BlitSurface (sprite->image, &grid, ge->screen, &pos);
	/* Draw sprite to new position */
}

/* ----------------------------------- */
void
ClearSprite (GameEngine * ge, Sprite_t * sprite)
{
	SDL_Rect mask;
	mask.w = sprite->width;
	mask.h = sprite->height;
	//if(mask.w>32){
	//  printf("w=%d h=%d\n",mask.w,mask.h);
	//}
	mask.x = sprite->ox;
	mask.y = sprite->oy;
	SDL_BlitSurface (ge->bg2, &mask, ge->screen, &mask);
	/* Clear sprite from the position */
}

/* --------------œ‘ æ…˙√¸÷µ--------------------- */
void
Disp_TankLeft (GameEngine * ge)
{
	int i,j;
	SDL_Rect res_rect, des_rect;
	res_rect.w = res_rect.h = 16;
	des_rect.w = des_rect.h = 16;
	res_rect.x = (9 - 1) * 16;
	res_rect.y = (6 - 1) * 16;
	/* Player tank left */
	for(j=0;j<2;j++)
	{
		for (i = 0; i < 7; i++)
		{
			if(j==0)
				des_rect.x = 112 - i * 16;
			else
				des_rect.x = 112 - i * 16+400;
			des_rect.y = 434;
			SDL_BlitSurface (ge->bg1, &des_rect, ge->screen, &des_rect);
		}
		for (i = 0; i < ge->player_left[j]; i++)
		{
			if (i >= 7)
				break;
			if(j==0)
				des_rect.x = 100 - i * 16;
			else
				des_rect.x = 112 - i * 16+400;
			des_rect.y = 434;
			SDL_BlitSurface (ge->tiles, &res_rect, ge->screen, &des_rect);
		}
	}
	////////////////////////////////////////////////////////////Enemy tank left
	res_rect.x = (9 - 1) * 16;
	res_rect.y = (7 - 1) * 16;
	if (ge->bore_enemy < 0)
		return;
	for (i = 0; i < 20; i++)
	{
		des_rect.x = 546;
		des_rect.y = 2 + i * 16;
		SDL_BlitSurface (ge->bg1, &des_rect, ge->screen, &des_rect);
	}
	for (i = 0; i < ge->bore_enemy; i++)
	{
		des_rect.x = 546;
		des_rect.y = 2 + i * 16;
		SDL_BlitSurface (ge->tiles, &res_rect, ge->screen, &des_rect);
	}
}

/* ----------------------------------- */
void
Show_Score (GameEngine * ge)
{
	Uint32 score;
	int i, len, num;
	SDL_Surface *tmp;
	SDL_Rect res_rect, des_rect;
	tmp = IMG_Load ("pics/numbers.png");
	SDL_SetColorKey (tmp, SDL_SRCCOLORKEY, SDL_MapRGB (tmp->format, 0, 0, 0));
	tmp = SDL_DisplayFormat (tmp);
	res_rect.w = res_rect.h = des_rect.w = des_rect.h = 16;
	res_rect.y = 0;
	des_rect.y = 434;

	for (i = 0; i < 2; i++)
	{
		/* Now support double player */
		score = ge->score[i];
		len = 0;
		while (score >= 1)
		{
			len++;
			num = score % 10;
			//show
			res_rect.x = num * 16;
			if(i==0)
				des_rect.x = 257 - len * 16;
			else
				des_rect.x = 257 - len * 16+100;

			SDL_BlitSurface (ge->bg1, &des_rect, ge->screen, &des_rect);
			SDL_BlitSurface (tmp, &res_rect, ge->screen, &des_rect);
			score /= 10;
		}
	}

	SDL_FreeSurface (tmp);
}

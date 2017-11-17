#include "public.h"
/* ----------------------------------- */
void
GameEngine_Init (GameEngine * ge)
{
	Uint32 VideoFlags;
	int SVolume = 128;

	/* Video stuff */
	if (!Video_Init (ge))
	{
		fprintf (stderr, "Can't init video!\n");
		exit (1);
	}

	SDL_WM_SetCaption ("Tank Battle", "Tank");
	SDL_ShowCursor (1);

	ge->swapscreen =
	    SDL_CreateRGBSurface (SDL_SRCALPHA | SDL_HWSURFACE, WIDTH, HEIGHT, BPP,
	                          ge->screen->format->Rmask,
	                          ge->screen->format->Gmask,
	                          ge->screen->format->Bmask,
	                          ge->screen->format->Amask);
	if (ge->swapscreen == NULL)
		fprintf (stderr, "Can't allocate SwapScreen!\n");
	ge->swapscreen = SDL_DisplayFormat (ge->swapscreen);

	SDL_EnableKeyRepeat (0, 0);

	/* Audio stuff */
	////////////////////////////////////////////////////////////

	///-----------------------------------------------
	if (Sound)
	{
		if (Mix_OpenAudio (22050, AUDIO_S16, 1, 256) < 0)
		{
			fprintf (stderr, "Couldn't set 22050 Hz 16-bit audio :%s\n",
			         SDL_GetError ());
			Sound = 0;
		}
	}
	if (Sound)
	{
		/* Allocate 4 channels for mixing */
		//Mix_AllocateChannels (2);
		Mix_Volume (-1, SVolume);
		Snd_bom = Mix_LoadWAV ("./snd/bom.wav");
		Snd_ding = Mix_LoadWAV ("./snd/ding.wav");
		Snd_dow = Mix_LoadWAV ("./snd/dow.wav");
	}

	/* More data init */

	ge->bg1 =
	    SDL_CreateRGBSurface (SDL_SRCALPHA | SDL_HWSURFACE, WIDTH, HEIGHT, BPP,
	                          ge->screen->format->Rmask,
	                          ge->screen->format->Gmask,
	                          ge->screen->format->Bmask,
	                          ge->screen->format->Amask);

	ge->bg2 =
	    SDL_CreateRGBSurface (SDL_SRCALPHA | SDL_HWSURFACE, WIDTH, HEIGHT, BPP,
	                          ge->screen->format->Rmask,
	                          ge->screen->format->Gmask,
	                          ge->screen->format->Bmask,
	                          ge->screen->format->Amask);
	if (ge->bg1 == NULL || ge->bg2 == NULL)
	{
		fprintf (stderr, "Can't allocate SwapScreen!\n");
		Video_Close (ge);
	}

	ge->bg1 = SDL_DisplayFormat (ge->bg1);
	ge->bg2 = SDL_DisplayFormat (ge->bg2);

	ge->tiles = IMG_Load ("pics/tiles.png");
	SDL_SetColorKey (ge->tiles, SDL_SRCCOLORKEY,
	                 SDL_MapRGB (ge->tiles->format, 255, 0, 255));
	ge->tiles = SDL_DisplayFormat (ge->tiles);

	ge->player1 = IMG_Load ("pics/player1.png");
	SDL_SetColorKey (ge->player1, SDL_SRCCOLORKEY,
	                 SDL_MapRGB (ge->player1->format, 255, 0, 255));
	ge->player1 = SDL_DisplayFormat (ge->player1);

	ge->player2 = IMG_Load ("pics/player2.png");
	SDL_SetColorKey (ge->player2, SDL_SRCCOLORKEY,
	                 SDL_MapRGB (ge->player2->format, 255, 0, 255));
	ge->player2 = SDL_DisplayFormat (ge->player2);

	ge->enemys = IMG_Load ("pics/enemys.png");
	SDL_SetColorKey (ge->enemys, SDL_SRCCOLORKEY,
	                 SDL_MapRGB (ge->enemys->format, 255, 0, 255));
	ge->enemys = SDL_DisplayFormat (ge->enemys);

	ge->bullets = IMG_Load ("pics/bullets.png");
	SDL_SetColorKey (ge->bullets, SDL_SRCCOLORKEY,
	                 SDL_MapRGB (ge->bullets->format, 255, 0, 255));
	ge->bullets = SDL_DisplayFormat (ge->bullets);

	ge->explodes = IMG_Load ("pics/explodes.png");
	SDL_SetColorKey (ge->explodes, SDL_SRCCOLORKEY,
	                 SDL_MapRGB (ge->explodes->format, 255, 0, 255));
	ge->explodes = SDL_DisplayFormat (ge->explodes);

	ge->level = 0;
	ge->score[0] = 0;
	ge->score[1] = 0;
}

/* ----------------------------------- */
void
GameEngine_Close (GameEngine * ge)
{
	if (ge->swapscreen != NULL)
		SDL_FreeSurface (ge->swapscreen);
	if (ge->bg1 != NULL)
		SDL_FreeSurface (ge->bg1);
	if (ge->bg2 != NULL)
		SDL_FreeSurface (ge->bg2);
	if (ge->tiles != NULL)
		SDL_FreeSurface (ge->tiles);
	if (ge->player1 != NULL)
		SDL_FreeSurface (ge->player1);
	if (ge->player2 != NULL)
		SDL_FreeSurface (ge->player2);
	if (ge->enemys != NULL)
		SDL_FreeSurface (ge->enemys);
	if (ge->bullets != NULL)
		SDL_FreeSurface (ge->bullets);
	if (ge->explodes != NULL)
		SDL_FreeSurface (ge->explodes);
	if (ge->bigtank != NULL)
		SDL_FreeSurface (ge->bigtank);
	if (Sound)
	{
		Mix_FreeChunk (Snd_bom);
		Mix_FreeChunk (Snd_ding);
		Mix_FreeChunk (Snd_dow);
		Mix_CloseAudio ();
	}
}

/* ----------------------------------- */
int
Video_Init (GameEngine * ge)
{
	Uint32 InitFlags;
	InitFlags = SDL_INIT_VIDEO;
	if (!Sound)
		InitFlags |= SDL_INIT_AUDIO;
	if (SDL_Init (InitFlags) < 0)
	{
		fprintf (stderr, "Can't init SDL:%s\n", SDL_GetError ());
		return FALSE;
	}
	atexit (SDL_Quit);
	Video_Set (ge);
	return TRUE;
}

/* ----------------------------------- */
void
Video_Set (GameEngine * ge)
{
	Uint32 VideoFlags;

	if (ge->FULLSCREEN)
	{
		VideoFlags = SDL_SRCALPHA | SDL_SWSURFACE | SDL_FULLSCREEN;
	}
	else
		VideoFlags = SDL_SRCALPHA | SDL_SWSURFACE;

	if ((ge->screen =
	            SDL_SetVideoMode (WIDTH, HEIGHT, BPP, VideoFlags)) == NULL)
	{
		fprintf (stderr, "Couldn't set %ix%i video mode: %s\n",
		         WIDTH, HEIGHT, SDL_GetError ());
		Video_Close (ge);
		exit (2);
	}
}

/* ----------------------------------- */
void
Video_Reset (GameEngine * ge)
{
	SDL_BlitSurface (ge->screen, NULL, ge->swapscreen, NULL);
	ge->FULLSCREEN = !ge->FULLSCREEN;
	Video_Init (ge);
	SDL_BlitSurface (ge->swapscreen, NULL, ge->screen, NULL);
	SDL_UpdateRect (ge->screen, 0, 0, 0, 0);
}

/* ----------------------------------- */
void
Video_Close (GameEngine * ge)
{
	SDL_Quit ();
}

/* ----------------------------------- */
void
PlaySound (Mix_Chunk * snd)
{
	int Ch;
	if (Sound)
	{
		if (Mix_PlayChannel (-1, snd, 0) < 0)
		{
			Ch = Mix_GroupOldest (-1);
			Mix_HaltChannel (Ch);
			Mix_PlayChannel (Ch, snd, 0);
		}
	}
}

/* ----------------------------------- */
void
ClearUp (GameEngine * ge)
{
	if (ge->swapscreen != NULL)
		SDL_FreeSurface (ge->swapscreen);
	if (ge->bg1 != NULL)
		SDL_FreeSurface (ge->bg1);
	if (ge->tiles != NULL)
		SDL_FreeSurface (ge->tiles);
	if (ge->player1 != NULL)
		SDL_FreeSurface (ge->player1);
	if (ge->player2 != NULL)
		SDL_FreeSurface (ge->player2);
	if (ge->enemys != NULL)
		SDL_FreeSurface (ge->enemys);
	if (ge->bullets != NULL)
		SDL_FreeSurface (ge->bullets);
	if (ge->explodes != NULL)
		SDL_FreeSurface (ge->explodes);
}
/* ----------------------------------- */
void
Load_Data (GameEngine * ge)
{
	int i, j;
	SDL_Surface *tmp, *tiles;
	SDL_Rect res_rect, des_rect;
	//ge->level=3;
	if (ge->level == 0)
	{
		tiles = IMG_Load ("pics/tiles.png");
		SDL_BlitSurface (tiles, NULL, ge->tiles, NULL);
		SDL_FreeSurface (tiles);
		tmp = IMG_Load ("pics/map01.png");
		ge->bore_enemy = 16;
		ge->enemy_left = 16;
		ge->max_enemys = 4;
		ge->player_left[0] = 4;
		if(players==1)
			ge->player_left[1] = 4;
		else						//single player
			ge->player_left[1] = -10;

		int tmpbg[][34] =
		    {
		        OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		        OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		        OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		        OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		        OO, OO, A1, A2, OO, OO, A1, A2, OO, OO, B1, B2, B2, B2, B2, B3, OO,
		        OO, C1, C2, C2, C2, C2, C3, OO, OO, A1, A2, OO, OO, A1, A2, OO, OO,
		        OO, OO, A3, A4, OO, OO, A3, A4, OO, OO, B4, B5, B5, B5, B5, B6, OO,
		        OO, C4, C5, C5, C5, C5, C6, OO, OO, A3, A4, OO, OO, A3, A4, OO, OO,
		        OO, OO, A3, A4, OO, OO, A3, A4, OO, OO, B1, B2, B2, B2, B2, B3, OO,
		        OO, C1, C2, C2, C2, C2, C3, OO, OO, A3, A4, OO, OO, A3, A4, OO, OO,
		        OO, OO, A5, A6, OO, OO, A5, A6, OO, OO, B4, B5, B5, B5, B5, B6, OO,
		        OO, C4, C5, C5, C5, C5, C6, OO, OO, A5, A6, OO, OO, A5, A6, OO, OO,
		        OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, HH, B1, B2, B2, B2, B3, OO,
		        OO, C1, C2, C2, C2, C3, HH, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		        OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, HH, B4, B5, B5, B5, B6, OO,
		        OO, C4, C5, C5, C5, C6, HH, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		        OO, OO, A1, A2, OO, OO, A1, A2, OO, OO, B1, B2, B2, B2, B2, B3, OO,
		        OO, C1, C2, C2, C2, C2, C3, OO, OO, A1, A2, OO, OO, A1, A2, OO, OO,
		        OO, OO, A3, A4, OO, OO, A3, A4, OO, OO, B4, B5, B5, B5, B5, B6, OO,
		        OO, C4, C5, C5, C5, C5, C6, OO, OO, A3, A4, OO, OO, A3, A4, OO, OO,
		        OO, OO, A3, A4, OO, OO, A3, A4, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		        OO, OO, OO, OO, OO, OO, OO, OO, OO, A3, A4, OO, OO, A3, A4, OO, OO,
		        OO, OO, A5, A6, OO, OO, A5, A6, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		        OO, OO, OO, OO, OO, OO, OO, OO, OO, A5, A6, OO, OO, A5, A6, OO, OO,
		        OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, A1, A2, OO, OO, HH, HH, HH,
		        HH, HH, HH, OO, OO, A1, A2, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		        OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, A3, A4, OO, OO, HH, HH, HH,
		        HH, HH, HH, OO, OO, A3, A4, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		        HH, HH, OO, OO, D1, D2, D3, D4, OO, OO, A3, A4, OO, OO, E1, E2, E2,
		        E2, E2, E3, OO, OO, A3, A4, OO, OO, D1, D2, D3, D4, OO, OO, HH, HH,
		        HH, HH, OO, OO, D5, D6, D7, D8, OO, OO, A3, A4, OO, OO, E4, E5, E5,
		        E5, E5, E6, OO, OO, A3, A4, OO, OO, D5, D6, D7, D8, OO, OO, HH, HH,
		        OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, A3, A4, OO, OO, E4, E5, E5,
		        E5, E5, E6, OO, OO, A3, A4, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		        OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, A5, A6, OO, OO, E7, E8, E8,
		        E8, E8, E9, OO, OO, A5, A6, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		        OO, OO, A1, A2, OO, OO, A1, A2, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		        OO, OO, OO, OO, OO, OO, OO, OO, OO, A1, A2, OO, OO, A1, A2, OO, OO,
		        OO, OO, A3, A4, OO, OO, A3, A4, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		        OO, OO, OO, OO, OO, OO, OO, OO, OO, A3, A4, OO, OO, A3, A4, OO, OO,
		        OO, OO, A3, A4, OO, OO, A3, A4, OO, OO, A1, A2, OO, OO, D1, D2, D3,
		        D2, D3, D4, OO, OO, A1, A2, OO, OO, A3, A4, OO, OO, A3, A4, OO, OO,
		        OO, OO, A5, A6, OO, OO, A5, A6, OO, OO, A5, A6, OO, OO, D5, D6, D7,
		        D6, D7, D8, OO, OO, A5, A6, OO, OO, A5, A6, OO, OO, A5, A6, OO, OO,
		        OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		        OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		        OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		        OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		        OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, G1, G2,
		        G3, G4, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		        OO, OO, OO, OO, OO, OO, OO, OO, B1, B2, B2, B3, HH, OO, OO, G5, G6,
		        G7, G8, OO, OO, HH, C1, C2, C2, C3, OO, OO, OO, OO, OO, OO, OO, OO,
		        OO, OO, OO, OO, OO, OO, OO, OO, B4, B5, B5, B6, HH, OO, OO, G9, Ga,
		        Gb, Gc, OO, OO, HH, C4, C5, C5, C6, OO, OO, OO, OO, OO, OO, OO, OO
		    };
		int bore_pos[][2] = { {0, 0}, {4, 2}, {16, 0}, {28, 2}, {32, 0} };
		for (i = 0; i < PLANE_H; i++)
			for (j = 0; j < PLANE_W; j++)
				bg[i][j] = tmpbg[i][j];
		for (i = 0; i < 5; i++)
			for (j = 0; j < 2; j++)
				ge->enemy_pos[i][j] = bore_pos[i][j];
	}
	else if (ge->level == 1)
	{
		tmp = IMG_Load ("pics/map02.png");
		ge->bore_enemy = 16;
		ge->enemy_left = 16;
		ge->max_enemys = 4;
		//ge->player_left=3;
		int tmpbg[][34] = {
		                      I1, I2, I2, I2, I2, I2, I3, A3, A4, OO, OO, E4, E5, E5, E5, E5, E5,
		                      E5, E5, E5, E6, OO, OO, OO, OO, D1, D2, D3, D4, OO, OO, I1, I2, I2,
		                      I4, I5, I5, I5, I5, I5, I6, A3, A4, OO, OO, E7, E8, E8, E8, E8, E8,
		                      E8, E8, E8, E9, OO, OO, OO, OO, D5, D6, D7, D8, OO, OO, I4, I5, I5,
		                      I1, I2, I2, I2, I3, HH, HH, A3, A4, OO, OO, OO, OO, OO, OO, HH, HH,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, HH, HH, OO, OO, OO, OO, OO, OO,
		                      I4, I5, I5, I5, I6, HH, HH, A5, A6, OO, OO, OO, OO, OO, OO, HH, HH,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, HH, HH, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, HH, HH, HH, HH, HH, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, B1, B2, B2, B2, B3, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, HH, HH, HH, HH, HH, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, B4, B5, B5, B5, B6, OO, OO, OO, OO, OO, OO, OO, OO,
		                      C1, C2, C2, C2, C3, OO, OO, HH, HH, OO, OO, OO, OO, OO, OO, RV, RV,
		                      RV, RV, A1, A2, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      C4, C5, C5, C5, C6, OO, OO, HH, HH, OO, OO, OO, OO, OO, OO, RV, RV,
		                      RV, RV, A3, A4, OO, OO, OO, OO, OO, OO, OO, OO, HH, HH, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, RV, RV, RV, RV, RV, RV,
		                      RV, RV, A3, A4, OO, OO, OO, OO, HH, HH, HH, HH, HH, HH, HH, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, RV, RV, RV, RV, RV, RV,
		                      RV, RV, A5, A6, OO, OO, OO, OO, HH, HH, HH, HH, HH, HH, HH, OO, OO,
		                      RV, RV, RV, RV, RV, RV, RV, RV, RV, OO, OO, OO, OO, OO, OO, RV, RV,
		                      RV, RV, OO, OO, OO, OO, OO, OO, B1, B2, B2, B3, HH, B1, B2, B2, B3,
		                      RV, RV, RV, RV, RV, RV, RV, RV, RV, OO, OO, OO, OO, OO, OO, RV, RV,
		                      RV, RV, OO, OO, OO, OO, OO, OO, B4, B5, B5, B6, HH, B4, B5, B5, B6,
		                      C1, C2, C2, C3, C1, C2, C2, C2, C3, OO, OO, I1, I2, I2, I2, I2, I3,
		                      I1, I2, I2, I2, I2, I3, OO, OO, B1, B2, B2, B2, B3, B1, B2, B2, B3,
		                      C4, C5, C5, C6, C4, C5, C5, C5, C6, OO, OO, I4, I5, I5, I5, I5, I6,
		                      I4, I5, I5, I5, I5, I6, OO, OO, B4, B5, B5, B5, B6, B4, B5, B5, B6,
		                      TT, TT, TT, TT, TT, TT, TT, TT, TT, OO, OO, TT, TT, TT, TT, TT, TT,
		                      TT, TT, TT, TT, TT, TT, OO, OO, TT, TT, TT, TT, TT, TT, TT, TT, TT,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, HH, HH, HH, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, D1, D2,
		                      D3, D4, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, HH, HH, HH, OO, OO,
		                      OO, OO, TT, A1, A2, OO, OO, TT, TT, HH, HH, TT, TT, OO, OO, D5, D6,
		                      D7, D8, OO, OO, TT, TT, HH, HH, TT, TT, OO, OO, A1, A2, TT, OO, OO,
		                      OO, OO, TT, A3, A4, OO, OO, C1, C2, C2, C2, C2, C3, OO, OO, OO, OO,
		                      OO, OO, OO, OO, B1, B2, B2, B2, B2, B3, OO, OO, A3, A4, TT, OO, OO,
		                      OO, OO, TT, A3, A4, OO, OO, C4, C5, C5, C5, C5, C6, OO, OO, OO, OO,
		                      OO, OO, OO, OO, B4, B5, B5, B5, B5, B6, OO, OO, A3, A4, TT, OO, OO,
		                      OO, OO, TT, A3, A4, OO, OO, C1, C2, C2, C2, C2, C3, OO, OO, D1, D2,
		                      D3, D4, OO, OO, B1, B2, B2, B2, B2, B3, OO, OO, A3, A4, TT, OO, OO,
		                      OO, OO, TT, A3, A4, OO, OO, C4, C5, C5, C5, C5, C6, OO, OO, D5, D6,
		                      D7, D8, OO, OO, B4, B5, B5, B5, B5, B6, OO, OO, A3, A4, TT, OO, OO,
		                      OO, OO, TT, A5, A6, OO, OO, TT, TT, TT, TT, TT, TT, OO, OO, OO, OO,
		                      OO, OO, OO, OO, TT, TT, TT, TT, TT, TT, OO, OO, A5, A6, TT, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, G1, G2,
		                      G3, G4, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, A1, A2, OO, OO, OO, G5, G6,
		                      G7, G8, OO, OO, OO, A1, A2, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, A5, A6, OO, OO, OO, G9, Ga,
		                      Gb, Gc, OO, OO, OO, A5, A6, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO
		                  };
		int bore_pos[][2] = { {0, 4}, {9, 0}, {23, 0}, {29, 0}, {32, 2} };
		for (i = 0; i < PLANE_H; i++)
			for (j = 0; j < PLANE_W; j++)
				bg[i][j] = tmpbg[i][j];
		for (i = 0; i < 5; i++)
			for (j = 0; j < 2; j++)
				ge->enemy_pos[i][j] = bore_pos[i][j];
	}
	else if (ge->level == 2)
	{
		tmp = IMG_Load ("pics/map03.png");
		ge->bore_enemy = 16;
		ge->enemy_left = 16;
		ge->max_enemys = 4;
		//ge->player_left=3;
		int tmpbg[][34] = {
		                      TT, TT, TT, TT, TT, TT, TT, TT, TT, TT, OO, TT, TT, TT, TT, TT, TT,
		                      TT, TT, TT, TT, TT, TT, OO, TT, TT, TT, TT, TT, TT, TT, TT, TT, TT,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, HH, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, HH, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, HH, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, HH, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      HH, TT, TT, TT, OO, OO, OO, OO, OO, OO, OO, TT, TT, TT, HH, HH, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, E1, E2, E3, OO, OO, TT, TT, TT, HH,
		                      HH, TT, TT, TT, OO, OO, E1, E2, E3, OO, OO, TT, TT, TT, HH, HH, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, E4, E5, E6, OO, OO, TT, TT, TT, HH,
		                      OO, OO, OO, OO, OO, OO, E4, E5, E6, OO, OO, OO, OO, OO, HH, HH, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, E4, E5, E6, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, E4, E5, E6, OO, OO, OO, OO, OO, HH, HH, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, E7, E8, E9, OO, OO, OO, OO, OO, OO,
		                      TT, TT, TT, TT, OO, OO, E4, E5, E6, OO, OO, E1, E2, E2, E2, E3, OO,
		                      OO, E1, E2, E2, E2, E3, OO, OO, E1, E2, E3, OO, OO, TT, TT, TT, TT,
		                      TT, TT, TT, TT, OO, OO, E4, E5, E6, TT, TT, E4, E5, E5, E5, E6, TT,
		                      TT, E4, E5, E5, E5, E6, OO, OO, E4, E5, E6, OO, OO, TT, TT, TT, TT,
		                      TT, TT, TT, TT, OO, OO, E7, E8, E9, TT, TT, E4, E5, E5, E5, E6, TT,
		                      TT, E4, E5, E5, E5, E6, OO, OO, E4, E5, E6, OO, OO, TT, TT, TT, TT,
		                      TT, TT, TT, TT, OO, OO, OO, OO, OO, OO, OO, E4, E5, E5, E5, E6, TT,
		                      TT, E4, E5, E5, E5, E6, TT, TT, E4, E5, E6, OO, OO, TT, TT, TT, TT,
		                      HH, HH, HH, HH, OO, OO, OO, OO, OO, OO, OO, E7, E8, E8, E8, E9, OO,
		                      OO, E7, E8, E8, E8, E9, TT, TT, E4, E5, E6, OO, OO, HH, HH, HH, HH,
		                      TT, TT, TT, TT, TT, TT, TT, TT, TT, OO, OO, OO, OO, OO, OO, TT, OO,
		                      OO, TT, OO, OO, OO, OO, OO, OO, E4, E5, E6, TT, TT, TT, TT, TT, TT,
		                      TT, TT, TT, TT, TT, TT, TT, TT, TT, OO, OO, TT, TT, TT, TT, TT, TT,
		                      TT, TT, TT, TT, TT, TT, OO, OO, E7, E8, E9, TT, TT, TT, TT, TT, TT,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, HH, HH, TT, TT, TT, TT,
		                      TT, TT, TT, TT, HH, HH, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, HH, HH, TT, TT, TT, TT,
		                      TT, TT, TT, TT, HH, HH, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, TT, TT, TT, TT, OO, OO, TT, TT, TT, TT, TT, TT, TT, TT, TT,
		                      TT, TT, TT, TT, TT, TT, TT, TT, TT, OO, OO, TT, TT, TT, TT, OO, OO,
		                      OO, OO, TT, TT, TT, TT, OO, OO, TT, TT, TT, TT, TT, TT, TT, TT, TT,
		                      TT, TT, TT, TT, TT, TT, TT, TT, TT, OO, OO, TT, TT, TT, TT, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, TT, TT,
		                      TT, TT, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, TT, TT, TT, TT, TT, TT, TT, TT, TT, TT, OO, OO, OO, TT, TT,
		                      TT, TT, OO, OO, OO, TT, TT, TT, TT, TT, TT, TT, TT, TT, TT, OO, OO,
		                      OO, OO, TT, TT, TT, TT, TT, TT, TT, TT, TT, TT, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, TT, TT, TT, TT, TT, TT, TT, TT, TT, TT, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, G1, G2,
		                      G3, G4, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, TT, TT, OO, OO, OO, OO, G5, G6,
		                      G7, G8, OO, OO, OO, OO, TT, TT, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, TT, TT, OO, OO, OO, OO, G9, Ga,
		                      Gb, Gc, OO, OO, OO, OO, TT, TT, OO, OO, OO, OO, OO, OO, OO, OO, OO
		                  };
		int bore_pos[][2] = { {0, 1}, {15, 1}, {32, 1}, {0, 6}, {32, 6} };
		for (i = 0; i < PLANE_H; i++)
			for (j = 0; j < PLANE_W; j++)
				bg[i][j] = tmpbg[i][j];
		for (i = 0; i < 5; i++)
			for (j = 0; j < 2; j++)
				ge->enemy_pos[i][j] = bore_pos[i][j];
	}
	else if (ge->level == 3)
	{
		tiles = IMG_Load ("pics/tiles2.png");
		SDL_BlitSurface (tiles, NULL, ge->tiles, NULL);
		SDL_FreeSurface (tiles);
		tmp = IMG_Load ("pics/map04.png");
		ge->bore_enemy = 12;
		ge->enemy_left = 12 + 2;
		//      ge->player_left = 3;
		ge->max_enemys = 4;
		int tmpbg[][34] = {
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      RV, RV, RV, RV, RV, RV, RV, RV, RV, RV, RV, RV, RV, RV, RV, RV, RV,
		                      RV, RV, RV, RV, RV, RV, RV, RV, RV, RV, RV, RV, RV, RV, RV, RV, RV,
		                      RV, RV, RV, RV, RV, RV, RV, RV, RV, RV, RV, RV, RV, RV, RV, RV, RV,
		                      RV, RV, RV, RV, RV, RV, RV, RV, RV, RV, RV, RV, RV, RV, RV, RV, RV,
		                      OO, OO, B1, B2, A3, A4, A3, A4, A1, A3, A4, B1, B2, A3, A4, B1, B2,
		                      A3, A4, A3, A4, B1, B2, A3, A4, A3, A4, B1, B2, A3, A4, B3, OO, OO,
		                      OO, OO, B4, B5, A5, A6, A5, A6, B3, A5, A6, B4, B5, A5, A6, B4, B5,
		                      A5, A6, A5, A6, B4, B5, A5, A6, A5, A6, B4, B5, A5, A6, B3, OO, OO,
		                      OO, OO, A3, A4, OO, OO, A3, A4, B1, B2, B1, B2, B3, OO, OO, A3, A4,
		                      B1, B2, OO, OO, B3, A3, A4, B1, B2, A3, A4, OO, OO, B1, B2, OO, OO,
		                      OO, OO, A5, A6, OO, OO, A5, A6, B4, B5, B4, B5, A1, OO, OO, A5, A6,
		                      B4, B5, OO, OO, A1, A5, A6, B4, B5, A5, A6, OO, OO, B4, B5, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, B1, B2, B3, OO, OO, A3, A4,
		                      B1, B2, OO, OO, B3, B1, B2, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, B4, B5, B3, OO, OO, A5, A6,
		                      B4, B5, OO, OO, B3, B4, B5, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, A3, A4, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, A3, A4, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, A5, A6, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, A5, A6, OO, OO, OO, OO, OO, OO,
		                      HH, A1, B3, B3, OO, OO, B3, A1, OO, OO, A1, B3, B1, B2, A1, B3, B1,
		                      B2, A1, B3, B1, B2, A3, A4, OO, OO, A1, B3, OO, OO, B3, B3, A1, HH,
		                      A3, A4, B1, B2, OO, OO, OO, OO, OO, OO, A3, A4, B4, B5, A3, A4, B4,
		                      B5, A3, A4, B4, B5, A5, A6, OO, OO, OO, OO, OO, OO, B1, B2, A3, A4,
		                      A5, A6, B4, B5, OO, OO, OO, OO, OO, OO, A5, A6, B3, B3, A5, A6, A1,
		                      B3, A5, A6, B3, B3, A1, B3, OO, OO, OO, OO, OO, OO, B4, B5, A5, A6,
		                      B1, B2, A3, A4, B1, B2, A3, A4, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, A3, A4, B1, B2, A3, A4, B1, B2,
		                      B4, B5, A5, A6, B4, B5, A5, A6, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, A5, A6, B4, B5, A5, A6, B4, B5,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, G1, G2,
		                      G3, G4, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, A3, A4, B3, OO, OO, G5, G6,
		                      G7, G8, OO, OO, B3, A3, A4, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO,
		                      OO, OO, OO, OO, OO, OO, OO, OO, OO, OO, A5, A6, B3, OO, OO, G9, Ga,
		                      Gb, Gc, OO, OO, B3, A3, A4, OO, OO, OO, OO, OO, OO, OO, OO, OO, OO
		                  };
		int bore_pos[][2] = { {0, 11}, {32, 11}, {0, 13}, {32, 13}, {32, 15} };
		for (i = 0; i < PLANE_H; i++)
			for (j = 0; j < PLANE_W; j++)
				bg[i][j] = tmpbg[i][j];
		for (i = 0; i < 5; i++)
			for (j = 0; j < 2; j++)
				ge->enemy_pos[i][j] = bore_pos[i][j];
		ge->bigtank = IMG_Load ("pics/bigtanks.png");
		SDL_SetColorKey (ge->bigtank, SDL_SRCCOLORKEY,
		                 SDL_MapRGB (ge->bigtank->format, 255, 0, 255));
		ge->bigtank = SDL_DisplayFormat (ge->bigtank);
	}

	SDL_BlitSurface (tmp, NULL, ge->bg1, NULL);
	SDL_FreeSurface (tmp);

	/* Draw background */
	SDL_BlitSurface (ge->bg1, NULL, ge->screen, NULL);
	res_rect.w = res_rect.h = 16;
	des_rect.w = res_rect.h = 16;
	for (i = 0; i < PLANE_H; i++)
	{
		for (j = 0; j < PLANE_W; j++)
		{
			if (bg[i][j] == OO || bg[i][j] == RV)
				continue;
			des_rect.x = j * 16;
			des_rect.y = i * 16;
			res_rect.x = ((bg[i][j] - 1) % 9) * 16;
			res_rect.y = ((bg[i][j] - 1) / 9) * 16;
			SDL_BlitSurface (ge->tiles, &res_rect, ge->screen, &des_rect);
		}
	}
	SDL_BlitSurface (ge->screen, NULL, ge->bg2, NULL);
	SDL_UpdateRect (ge->screen, 0, 0, 0, 0);

	srand (time (NULL));

	/* Init key state */
	key_state.Up[0] = 0;
	key_state.Up[1] = 0;

	key_state.Down[0] = 0;
	key_state.Down[1] = 0;

	key_state.Left[0] = 0;
	key_state.Left[1] = 0;

	key_state.Right[0] = 0;
	key_state.Right[1] = 0;

	key_state.Fire[0] = 0;
	key_state.Fire[1] = 0;

	key_state.Move[0] = 0;
	key_state.Move[1] = 0;

	key_state.F4 = 0;
	key_state.Quit = 0;
	key_state.AnyKey = 0;
	key_state.Debug = 0;

	for (i = 0; i < 2; i++)
		player[i].active = 0;
	for (i = 0; i < MAX_ENEMYS; i++)
		enemy[i].active = 0;
	for (i = 0; i < MAX_ENEMYS + 4; i++)
		bullet[i].active = 0;
	for (i = 0; i < 4; i++)
		explode[i].active = 0;
	if (ge->level == 3)
		BoreBigTank (ge, 11);

}
/* ----------------------------------- */
void
Game_Menu ()
{
	//SDL_Event event;
	SDL_Surface *tmp,*tmp2;
	SDL_Rect des_rect,rect1,rect2;
	tmp = IMG_Load ("pics/menu.png");
	tmp2=IMG_Load("pics/bullets.png");
	//SDL_SetColorKey(tmp,SDL_SRCCOLORKEY,SDL_MapRGB(tmp->format,0,0,0));
	tmp = SDL_DisplayFormat (tmp);
	des_rect.w = 576;
	des_rect.h = 448;
	des_rect.x = 32;
	des_rect.y = 16;
	rect1.w=rect2.w=rect1.h=rect2.h=15;
	rect1.x=230; rect1.y=395;		//µãµÄ×ø±ê
	rect2.x=230;rect2.y=430;
	SDL_BlitSurface (tmp, NULL, game_engine.screen, &des_rect);
	SDL_BlitSurface(tmp2,NULL,game_engine.screen,&rect1);
	SDL_UpdateRect (game_engine.screen, 0, 0, 0, 0);
	SDL_FreeSurface (tmp);
	//sleep(3);
	while (game_state == GAME_MENU)
	{
		while(SDL_PollEvent (&game_engine.event))
		{
			switch(game_engine.event.type)
			{
			case SDL_KEYDOWN:
				switch(game_engine.event.key.keysym.sym)
				{
				case SDLK_UP:
					printf("one player\n");
					if(players!=0)
					{
					players=0;
					//clear dot 2
					SDL_BlitSurface(game_engine.screen,&rect1,game_engine.screen, &rect2);
					SDL_BlitSurface(tmp2,NULL,game_engine.screen, &rect1);
					SDL_UpdateRect (game_engine.screen, 0, 0, 0, 0);
					}
					break;
				case SDLK_DOWN:
					printf("two player\n");
					if(players!=1)
					{
					players=1;
						//clear dot 1
					SDL_BlitSurface(game_engine.screen,&rect2,game_engine.screen, &rect1);
					SDL_BlitSurface(tmp2,NULL,game_engine.screen, &rect2);
					SDL_UpdateRect (game_engine.screen, 0, 0, 0, 0);
					}
					break;
				case SDLK_RETURN:
					game_state=GAME_RUNNING;
					printf("begin \n");
					break;
				case SDLK_F4:
					Video_Reset (&game_engine);
					break;
				case SDLK_ESCAPE:
					SDL_Quit();
					exit(0);
					break;
				}
				break;
			}
		}
	}
}


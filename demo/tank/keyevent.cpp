#include "public.h"
int
EVENT_FILTER (const SDL_Event * event)
{
	switch (event->type)
	{
	case SDL_KEYDOWN:
		key_state.AnyKey = 1;
		switch (event->key.keysym.sym)
		{
		case SDLK_w:
			key_state.Up[0] = 1;
			key_state.Move[0] = 1;
			player[0].dir = DIR_UP;
			break;
		case SDLK_a:
			key_state.Left[0] = 1;
			key_state.Move[0] = 1;
			player[0].dir = DIR_LEFT;
			break;
		case SDLK_s:
			key_state.Down[0] = 1;
			key_state.Move[0] = 1;
			player[0].dir = DIR_DOWN;
			break;
		case SDLK_d:
			key_state.Right[0] = 1;
			key_state.Move[0] = 1;
			player[0].dir = DIR_RIGHT;
			break;
		case SDLK_UP:
			key_state.Up[1] = 1;
			key_state.Move[1] = 1;
			player[1].dir = DIR_UP;
			//ClearSprite(&game_engine,&player[0]);
			//ChangeDirection(&player[0],DIR_UP);
			//DrawSprite(&game_engine,&player[0]);
			break;
		case SDLK_LEFT:
			key_state.Left[1] = 1;
			key_state.Move[1] = 1;
			player[1].dir = DIR_LEFT;
			//ClearSprite(&game_engine,&player[0]);
			//ChangeDirection(&player[0],DIR_LEFT);
			//DrawSprite(&game_engine,&player[0]);
			break;
		case SDLK_DOWN:
			key_state.Down[1] = 1;
			key_state.Move[1] = 1;
			player[1].dir = DIR_DOWN;
			//ClearSprite(&game_engine,&player[0]);
			//ChangeDirection(&player[0],DIR_DOWN);
			//DrawSprite(&game_engine,&player[0]);
			break;
		case SDLK_RIGHT:
			key_state.Right[1] = 1;
			key_state.Move[1] = 1;
			player[1].dir = DIR_RIGHT;
			//ClearSprite(&game_engine,&player[0]);
			//ChangeDirection(&player[0],DIR_RIGHT);
			//DrawSprite(&game_engine,&player[0]);
			break;
		case SDLK_j:
			key_state.Fire[0] = 1;
			break;
		case SDLK_KP_PERIOD:
			key_state.Fire[1] = 1;
			break;
		case SDLK_F4:
			key_state.F4 = 1;
			Video_Reset (&game_engine);
			break;
		case SDLK_ESCAPE:
			key_state.Quit = 1;
			break;
		}
		break;
	case SDL_KEYUP:
		key_state.AnyKey = 0;
		switch (event->key.keysym.sym)
		{
		case SDLK_w:
			key_state.Up[0] = 0;
			key_state.Move[0] = 0;
			break;
		case SDLK_a:
			key_state.Left[0] = 0;
			key_state.Move[0] = 0;
			break;
		case SDLK_s:
			key_state.Down[0] = 0;
			key_state.Move[0] = 0;
			break;
		case SDLK_d:
			key_state.Right[0] = 0;
			key_state.Move[0] = 0;
			break;
		case SDLK_UP:
			key_state.Up[1] = 0;
			key_state.Move[1] = 0;
			break;
		case SDLK_LEFT:
			key_state.Left[1] = 0;
			key_state.Move[1] = 0;
			break;
		case SDLK_DOWN:
			key_state.Down[1] = 0;
			key_state.Move[1] = 0;
			break;
		case SDLK_RIGHT:
			key_state.Right[1] = 0;
			key_state.Move[1] = 0;
			break;
		case SDLK_j:
			key_state.Fire[0] = 0;
			break;
		case SDLK_KP_PERIOD:
			key_state.Fire[1] = 0;
			break;
		case SDLK_ESCAPE:
			key_state.Quit = 0;
			break;
		}
		break;
	default:
		return (1);
	}
	return (0);
}

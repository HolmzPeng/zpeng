#include "kyo.h"


void pic_load(SDL_Surface **optimizedImage, char *pic_name)
{
    SDL_Surface *loadedImage = NULL;
    *optimizedImage = NULL;
    loadedImage = IMG_Load(pic_name);
    if (loadedImage != NULL)
    {
        *optimizedImage = SDL_DisplayFormat(loadedImage);
        SDL_FreeSurface(loadedImage);
        SDL_SetColorKey(*optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB((*optimizedImage)->format, 255, 255, 255));
    }
    else
    {
        GAME_ERROR("IMG_Load %s", pic_name);
        SDL_Quit();
    }
}

BOOL apply_surface(int x, int y, SDL_Surface *src, SDL_Surface *dst)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    if (SDL_BlitSurface(src, NULL, dst, &offset) != 0)
    {
        return FALSE;
    }

    return TRUE;
}



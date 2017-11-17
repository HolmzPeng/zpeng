#include <box.h>

static void video_close(void)
{
    SDL_Quit();
}


static int video_set(GameEngine *ge)
{
    Uint32 videoflags;
    
    if (ge->fullscreen)
        videoflags = SDL_SRCALPHA | SDL_SWSURFACE | SDL_FULLSCREEN;
    else
        videoflags = SDL_SRCALPHA | SDL_SWSURFACE;

    if ((ge->screen = SDL_SetVideoMode(WIDTH, HEIGHT, BPP, videoflags)) == NULL)
    {
        fprintf(stderr, "Couldn't set %ix%i video mode : %s\n", WIDTH, HEIGHT, SDL_GetError()); 
        video_close();
        return FALSE;
    }

    return TRUE;
}

static int video_init(GameEngine *ge)
{
    Uint32 initflags = SDL_INIT_VIDEO;

    if (ge->sound)
        initflags |= SDL_INIT_AUDIO;

    if (SDL_Init(initflags) < 0)
    {
        fprintf(stderr, "Can't init SDL:%s\n", SDL_GetError()); 
        return FALSE;
    }
    atexit(SDL_Quit);

    return video_set(ge);
}


static void video_reset(GameEngine *ge)
{
    /*
     *SDL_BlitSurface(ge->screen, NULL, ge->swapscreen, NULL);
     *ge->fullscreen = !ge->fullscreen;
     *video_init(ge);
     *SDL_BlitSurface(ge->swapscreen, NULL, ge->screen, NULL);
     *SDL_UpdateRect(ge->screen, 0, 0, 0, 0);
     */
}

int box_init(GameEngine *ge)
{
    memset(ge, 0, sizeof(GameEngine));

    ERR(video_init(ge) == FALSE, "Can't init video!\n", return FALSE);

    TTF_Init();
    ge->pFont = TTF_OpenFont("simfang.ttf", 18);
    ERR(ge->pFont == NULL, "TTF_OpenFont failed!\n", return FALSE);

    box_show_bg(ge);

    SDL_WM_SetCaption("Box", "Box");

    srand(time(NULL));

    return 0;
}

int box_exit(GameEngine *ge)
{
    video_close();
    TTF_CloseFont(ge->pFont);
    TTF_Quit();
    SDL_KillThread(ge->th);
}

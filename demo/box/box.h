#ifndef __BOX_H__
#define __BOX_H__


#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <res.h>

#define WIDTH       640
#define HEIGHT      480
#define BPP         32

#define BOX_SIZE            4

#define START_X     25
#define START_Y     20

#define ACT_W       420
#define ACT_H       440
#define ACT_BG      0x999999

#define NEXT_W      140
#define NEXT_H      120

#define DATA_W      140
#define DATA_H      290

#define VIRT_W      21
#define VIRT_H      22

#define PIXEL       20

#define LEVEL       10

#define BG          0xffffff

#define TRUE        1
#define FALSE       0

#define ERR(val, string, ret)       do {                                    \
                                            if (val)                        \
                                            {                               \
                                                fprintf(stderr, string);    \
                                                ret;                        \
                                            }                               \
                                       } while (0)

typedef struct {
    int x;
    int y;
    int type;
    int dir;
    int color;
}BOX;

typedef struct {
    unsigned int left:1;
    unsigned int right:1;
    unsigned int down:1;
    unsigned int rotate:1;
    unsigned int pause:1;
    unsigned int quit:1;
}KEYSTATE;

typedef struct {
    int x;
    int y;
    int score;
    int level;
    int pause;
    int sound;
    int fullscreen;
    int game_over;
    BOX curr_box;
    BOX next_box;
    int box_buf[VIRT_H][VIRT_W];
    SDL_Event event;
    SDL_Surface *screen;
    SDL_Thread *th;
    TTF_Font *pFont;
}GameEngine;

extern int box_init(GameEngine *box);
extern int key_handle(void *data);
extern int event_filter(const SDL_Event *event);
extern int play_game(GameEngine *box);
extern int box_exit(GameEngine *box);
extern int box_show(GameEngine *box);
extern void box_show_bg(GameEngine *box);
extern void box_show_data(GameEngine *box);

//extern KEYSTATE key_state;  

#endif /* __BOX_H__ */

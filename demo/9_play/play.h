#ifndef __KYO_PLAY_H__
#define __KYO_PLAY_H__

#include <views.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>

#define CTL_PATH        "/tmp/play_ctl"
#define MES_PATH        "/tmp/play_mes"

#define LRC_FG_COLOR    0xFF0000
#define LRC_BG_COLOR    0xFFFFFF

#define FONT_SIZE       30

struct lrc_node_t {
    int ms;
    char *content;
    SDL_Surface *bg;
    SDL_Surface *fg;
    struct lrc_node_t *next;
    struct lrc_node_t *prev;
};

struct play_t {
    int ctl;
    int mes;
    int pid;
    int pause;
    int lrc_quit;
    int time_length;
    int fg_color;
    int bg_color;
    struct lrc_node_t lrc_head;
    TTF_Font *font;
    SDL_mutex *mutex;
    SDL_Thread *lrc_thread;
    char filepath[PATHMAX];
    char tmpbuf[PATHMAX];
    struct views_t *views;
    struct global_t global;
    struct sdl_t *sdl;
};

struct play_t *play_init(const char *path);

void play_exit(struct play_t *play);

void play_run(struct play_t *play);


#endif /* __KYO_PLAY_H__ */

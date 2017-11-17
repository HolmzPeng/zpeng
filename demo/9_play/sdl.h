#ifndef __KYO_SDL_H__
#define __KYO_SDL_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

#define DEBUG       0

#define DEBUG0(...)
#define DEBUG1(...)     printf(__VA_ARGS__)
#ifdef DEBUG
    #define DEBUG2(...)     fprintf(stderr, __VA_ARGS__)
#else
    #define DEBUG2(...)
#endif

#define SDL_ERRP(con, ret, flag, ...)   do      \
            {                                   \
                if (con)                        \
                {                               \
                    DEBUG##flag(__VA_ARGS__);   \
                    ret;                        \
                }                               \
            } while (0)

struct sdl_t;

typedef void (sdl_key_handle_t)(int key, int state, void *);
typedef void (sdl_mouse_handle_t)(int button, int state, int x, int y, void *);

typedef void (sdl_quit_handle_t)(struct sdl_t *, void *data);

typedef struct {
    sdl_key_handle_t *handle;
    void *data;
}sdl_key_t;

struct mouse_node_t;

typedef struct mouse_node_t {
    SDL_Rect rect;
    sdl_mouse_handle_t *handle;
    void *data;
    struct mouse_node_t *next;
    struct mouse_node_t *prev;
}sdl_mouse_t;

struct sdl_t {
    int w;
    int h;
    unsigned int bpp:8;
    unsigned int ttf:1;
    unsigned int img:1;
    unsigned int audio:1;
    unsigned int quit:1;
    unsigned int copy:1;
    SDL_Surface *screen;
    sdl_key_t key[2][SDLK_LAST];   //0:keydown 1:keyup
    sdl_mouse_t mouse[2][SDL_BUTTON_X2];   //0:keydown 1:keyup
    sdl_quit_handle_t *quit_handle;
    void *quit_data;
};

#define KYO_QUIT        ((void *)-100)
#define KYO_NULL        ((void *)0)

#define KYO_VIDEO       0
#define KYO_IMG         1
#define KYO_TTF         2
#define KYO_CHUNK       4
#define KYO_MUSIC       8
#define KYO_FULLSCREEN  0x10

#define KYO_KEYDOWN     0
#define KYO_KEYUP       1

extern struct sdl_t *sdl_init(int w, int h, int bpp, int flag);
extern void sdl_exit(struct sdl_t *);
extern struct sdl_t *sdl_copy(struct sdl_t *sdl);
extern int sdl_run(struct sdl_t *);
extern void sdl_register_quit(struct sdl_t *, sdl_quit_handle_t *handle, void *data);
extern int sdl_register_key(struct sdl_t *, int key, int state, sdl_key_handle_t *handle, void *data);
extern int sdl_register_mouse(struct sdl_t *, int button, int state, SDL_Rect *rect, sdl_mouse_handle_t *handle, void *data);
extern void draw_pixel(SDL_Surface *dst, int x, int y, unsigned int color);
extern void draw_vline(SDL_Surface *dst, int x, int y, int l, unsigned int color);
extern void draw_hline(SDL_Surface *dst, int x, int y, int l, unsigned int color);
extern void draw_rect(SDL_Surface *dst, int x, int y, int w, int h, unsigned int color);
extern void draw_box(SDL_Surface *dst, int x, int y, int w, int h, int border, unsigned int color, unsigned int border_color);
extern void draw_circle(SDL_Surface *dst, int x, int y, int r, unsigned int color);
extern SDL_Surface *pic_load(const char *file);
extern SDL_Color kyo_color(unsigned int color);
extern SDL_Rect kyo_rect(int x, int y, int w, int h);
extern int show_txt(SDL_Surface *dst, TTF_Font *font, const char *content, int x, int y, int w, int h, unsigned int color);

#endif /* __KYO_SDL_H__ */

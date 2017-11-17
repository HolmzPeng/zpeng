#ifndef __KYO_SDL_H__
#define __KYO_SDL_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

typedef void (key_handle_t)(int key, void *);
typedef void (mouse_handle_t)(int button, int x, int y, void *);

struct mouse_node_t {
    int button;
    mouse_handle_t *handle;
    void *data;
    struct mouse_node_t *next;
    struct mouse_node_t *prev;
};

struct key_node_t {
    int key;
    key_handle_t *handle;
    void *data;
    struct key_node_t *next;
    struct key_node_t *prev;
};

struct sdl_t {
    unsigned int ttf:1;
    unsigned int audio:1;
    SDL_Surface *screen;    
    struct key_node_t key_head;
    struct mouse_node_t mouse_head;
};

#define KYO_VIDEO       0x0
#define KYO_TTF         0x1
#define KYO_MUSIC       0x2
#define KYO_CHUNK       0x4


#define DEBUG0(...)
#define DEBUG1(...)     fprintf(stderr, __VA_ARGS__);

#define KYO_ERRP(con, ret, flag, ...)   do  \
        {                                   \
            if (con)                        \
            {                               \
                DEBUG##flag(__VA_ARGS__)    \
                ret;                        \
            }                               \
        } while (0)

#define KYO_QUIT    ((key_handle_t *)(-1))

struct sdl_t *sdl_init(int width, int height, int bpp, int flag);
int register_key(struct sdl_t *sdl, int key, key_handle_t *handle, void *data);
int register_mouse(struct sdl_t *sdl, int button, mouse_handle_t *handle, void *data);
int run_key(struct sdl_t *sdl);
void sdl_exit(struct sdl_t *sdl);
SDL_Surface *pic_load(const char *file);
void show_txt(TTF_Font *font, const char *text, int color, SDL_Surface *dst, int x, int flagx, int y, int flagy);
void draw_pixel(SDL_Surface *dst, int x, int y, int color);
void draw_rect(SDL_Surface *dst, int x, int y, int w, int h, int color);
void draw_box(SDL_Surface *dst, int x, int y, int w, int h, int border, int b_color, int color);
void draw_circle(SDL_Surface *dst, int x, int y, int r, int color);

#endif /* __KYO_SDL_H__ */

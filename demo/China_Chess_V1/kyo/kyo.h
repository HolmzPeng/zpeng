#ifndef __KYO_CHESS_H_
#define __KYO_CHESS_H_

#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <chess.h>

#define GAME_ERROR(s, arg...)       fprintf(stderr, s" error!\n", ##arg)

#define TRUE        0
#define FALSE       1

#define CH_NUM      14

extern void pic_load(SDL_Surface **optimizedImage, char *pic_name);

extern BOOL apply_surface(int x, int y, SDL_Surface *src, SDL_Surface *dst);



#endif /* __KYO_CHESS_H_ */

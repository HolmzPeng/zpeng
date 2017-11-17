#include <box.h>

static void draw_rect(SDL_Surface *dst, int x, int y, int w, int h, int color)
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;
    offset.w = w; 
    offset.h = h;

    SDL_FillRect(dst, &offset, SDL_MapRGB(dst->format, (char)(color >> 16), (char)(color >> 8), (char)color));
}

static void draw_pixel(SDL_Surface *dst, int x, int y, int size, int color)
{
    
    /*draw_rect(dst, x, y, size, size, 0xffffff);*/
    draw_rect(dst, x, y, size, size, color);
    /*draw_rect(dst, x + 2, y + 2, size - 4, size - 4, color);*/

}

static void draw_vline(SDL_Surface *dst, int x, int y, int len, int color)
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;
    offset.w = 1; 
    offset.h = len;

    SDL_FillRect(dst, &offset, SDL_MapRGB(dst->format, (char)(color >> 16), (char)(color >> 8), (char)color));
}

static void draw_hline(SDL_Surface *dst, int x, int y, int len, int color)
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;
    offset.w = len; 
    offset.h = 1;

    SDL_FillRect(dst, &offset, SDL_MapRGB(dst->format, (char)(color >> 16), (char)(color >> 8), (char)color));
}


static void draw_box(SDL_Surface *dst, int x, int y, int w, int h, int color)
{
    draw_hline(dst, x, y, w, color);
    draw_vline(dst, x + w, y, h, color);
    draw_hline(dst, x, y + h, w, color);
    draw_vline(dst, x, y, h, color);
}

static void draw_txt(SDL_Surface *dst, TTF_Font *pFont, int x, int y, const char *content, int color)
{
    SDL_Surface *txt = NULL;
    SDL_Rect offset = {x, y};
    SDL_Color fc = {(char)(color >> 16), (char)(color >> 8), (char)color};
    
    txt = TTF_RenderUTF8_Blended(pFont, content, fc); 

    SDL_BlitSurface(txt, NULL, dst, &offset);
    SDL_FreeSurface(txt);
}

static void draw_square(SDL_Surface *dst, int x, int y, BOX *box)
{
    int i, j;

    for (i = 0; i < BOX_SIZE; i++)
    {
        for (j = 0; j < BOX_SIZE; j++) 
        {
            if (square[box->type][box->dir][i][j]) 
                draw_pixel(dst, x + j * PIXEL, y + i * PIXEL, PIXEL, box->color);
        }
    }
}

static void draw_box_buf(GameEngine *ge)
{
    int i, j;

    for (i = 0; i < VIRT_H; i++)
    {
        for (j = 0; j < VIRT_W; j++) 
        {
            if (ge->box_buf[i][j])
                draw_pixel(ge->screen, START_X + j * PIXEL, START_Y + i * PIXEL, PIXEL, ge->box_buf[i][j]); 
        }
    }
}

void box_show_bg(GameEngine *ge)
{
    draw_rect(ge->screen, 0, 0, WIDTH, HEIGHT, 0xcccccc);       //清背景

    draw_rect(ge->screen, START_X + ACT_W + 25, START_Y - 5, NEXT_W + 10, NEXT_H + 10, 0x0);  
    draw_rect(ge->screen, START_X + ACT_W + 30, START_Y, NEXT_W, NEXT_H, ACT_BG); 

    draw_rect(ge->screen, START_X + ACT_W + 25, START_Y + 150 - 5, DATA_W + 10, DATA_H + 10, 0x0); 
    draw_rect(ge->screen, START_X + ACT_W + 30, START_Y + 150, DATA_W, DATA_H, ACT_BG); 

}

void box_show_next(GameEngine *ge)
{
    draw_rect(ge->screen, START_X + ACT_W + 25, START_Y - 5, NEXT_W + 10, NEXT_H + 10, 0x0);  
    draw_rect(ge->screen, START_X + ACT_W + 30, START_Y, NEXT_W, NEXT_H, ACT_BG); 
    draw_square(ge->screen, START_X + ACT_W + 60, START_Y + 30, &ge->next_box);
}

void box_show_data(GameEngine *ge)
{
    int sx = START_X + ACT_W + 30;
    int sy = START_Y + 150;
    char score[128], level[128];

    snprintf(score, sizeof(score) - 1, "%d", ge->score);
    snprintf(level, sizeof(level) - 1, "%d", ge->level);

    draw_rect(ge->screen, sx - 5, sy - 5, DATA_W + 10, DATA_H + 10, 0x0); 
    draw_rect(ge->screen, sx, sy, DATA_W, DATA_H, ACT_BG); 
    draw_txt(ge->screen, ge->pFont, sx + 10, sy + 100, "Score :", 0x0);
    draw_txt(ge->screen, ge->pFont, sx + 75, sy + 101, score, 0x0);
    draw_txt(ge->screen, ge->pFont, sx + 10, sy + 140, "Level :", 0x0);
    draw_txt(ge->screen, ge->pFont, sx + 75, sy + 141, level, 0x0);
}

int box_show(GameEngine *ge)
{
    draw_rect(ge->screen, START_X - 5, START_Y - 5, ACT_W + 10, ACT_H + 10, 0x0);  //画主框
    draw_rect(ge->screen, START_X, START_Y, ACT_W, ACT_H, ACT_BG); //清主框背景

    draw_box_buf(ge);

    SDL_Flip(ge->screen);

    return 0;
}

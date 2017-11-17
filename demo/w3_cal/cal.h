#ifndef __CAL_H__
#define __CAL_H__

#include <sdl.h>

#define WIDTH       480
#define HEIGHT      272
#define BPP         32

#define ROW         6
#define COL         7

#define BASE_YEAR   1970

struct cal_t {
    int year; 
    int month;
    int day;
    char data[ROW][COL]; 
    TTF_Font *font;
    Mix_Music *music;
    int play;
    struct sdl_t *sdl;
};

extern int now_year(void);
extern int now_month(void);
extern int now_day(void);
extern void get_data(int year, int month, char data[][COL]);

#endif /* __CAL_H__ */

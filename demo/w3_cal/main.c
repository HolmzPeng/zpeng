#include <cal.h>

void show_cal(struct cal_t *cal)
{
    int i, j;
    char buf[1024];

    draw_rect(cal->sdl->screen, 0, 0, WIDTH, HEIGHT, 0x0);

    snprintf(buf, sizeof(buf), "%4d 年 %02d 月",  cal->year, cal->month);

    show_txt(cal->font, buf, 0xFFFFFF, cal->sdl->screen, 145, 0, 0, 0);

    snprintf(buf, sizeof(buf), "一 二 三 四 五");

    show_txt(cal->font, buf, 0xFFFFFF, cal->sdl->screen, 110, 0, 30, 0);

    snprintf(buf, sizeof(buf), "六 日");

    show_txt(cal->font, buf, 0xFF0000, cal->sdl->screen, 283, 0, 30, 0);

    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++) 
        {
            snprintf(buf, sizeof(buf), "%2d", cal->data[i][j]);
            if ((i == 0 && cal->data[i][j] > 7) || (i >= ROW - 2 && cal->data[i][j] < 14))
                show_txt(cal->font, buf, 0x969696, cal->sdl->screen, 110 + j * 34, 0, 60 + i * 24, 0);
            else if (j == 6 || j == 7)
                show_txt(cal->font, buf, 0xFF0000, cal->sdl->screen, 110 + j * 34, 0, 60 + i * 24, 0);
            else
                show_txt(cal->font, buf, 0xFFFFFF, cal->sdl->screen, 110 + j * 34, 0, 60 + i * 24, 0);
        }
    }

    SDL_Flip(cal->sdl->screen);
}

void key_handle(int key, void *data)
{
    struct cal_t *cal = (struct cal_t *)data;
    
    switch (key)
    {
        case SDLK_UP:
            cal->year--;
            break;
        case SDLK_DOWN:
            cal->year++;
            break;
        case SDLK_LEFT:
            if (cal->month == 1)
            {
                cal->year--; 
                cal->month = 12;
            }
            else
                cal->month--;
            break;
        case SDLK_RIGHT:
            if (cal->month == 12)
            {
                cal->year++;
                cal->month = 1; 
            }
            else
                cal->month++;
            break;
        case SDLK_SPACE:
            cal->year = now_year();
            cal->month = now_month();
            cal->day = now_day();
            break;
        case SDLK_RETURN:
            if (!cal->play)
                Mix_PausedMusic();
            else
                Mix_ResumeMusic();
            cal->play = !cal->play;
            return;
        default:
            break;
    }
    get_data(cal->year, cal->month, cal->data);
    show_cal(cal);
}

int cal_init(struct cal_t *cal)
{
    memset(cal, 0, sizeof(struct cal_t));

    cal->year = now_year();
    cal->month = now_month();
    cal->day = now_day();

    get_data(cal->year, cal->month, cal->data);

    cal->sdl = sdl_init(WIDTH, HEIGHT, BPP, KYO_VIDEO | KYO_TTF | KYO_MUSIC);
    KYO_ERRP(NULL == cal->sdl, goto ERR1, 0);

    cal->font = TTF_OpenFont("../simfang.ttf", 20);
    KYO_ERRP(NULL == cal->font, goto ERR2, 0);

    TTF_SetFontStyle(cal->font, TTF_STYLE_BOLD);

    cal->music = Mix_LoadMUS("../God.mp3");
    KYO_ERRP(NULL == cal->font, goto ERR3, 0);

    Mix_PlayMusic(cal->music, -1);

    show_cal(cal);

    register_key(cal->sdl, SDLK_UP, key_handle, cal);
    register_key(cal->sdl, SDLK_DOWN, key_handle, cal);
    register_key(cal->sdl, SDLK_LEFT, key_handle, cal);
    register_key(cal->sdl, SDLK_RIGHT, key_handle, cal);
    register_key(cal->sdl, SDLK_SPACE, key_handle, cal);
    register_key(cal->sdl, SDLK_RETURN, key_handle, cal);

    return 0;
ERR3:
    TTF_CloseFont(cal->font);
ERR2:
    sdl_exit(cal->sdl);
ERR1:
    return -1;
}

void cal_exit(struct cal_t *cal)
{
    TTF_CloseFont(cal->font);
    Mix_FreeMusic(cal->music);
    sdl_exit(cal->sdl);
}


int main(void)
{
    struct cal_t cal;

    if (cal_init(&cal))
        return 0;

    run_key(cal.sdl);

    cal_exit(&cal);

    return 0;
}

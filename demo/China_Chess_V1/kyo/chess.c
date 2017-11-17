#include "kyo.h"

BOOL turn = 0;    //描述当前该谁走,0为红走，1为绿走


/*
 *
 *  ROW  对 y
 *  COL  对 x
 *
 *
 *
 *
 *
 */

const int SCREEN_WIDTH = 457;
const int SCREEN_HEIGHT = 500;
const int SCREEN_BPP = 16;

struct pos_t cur_key;

struct st_chess val;

INT flag = 0;

SDL_Surface *chess[2][CH_NUM];

SDL_Surface *screen = NULL;
SDL_Surface *background = NULL;
SDL_Surface *current = NULL;

#define SPACE_X     49 
#define SPACE_Y     49

#define PHY_S_X     11
#define PHY_S_Y     8

#define CHESS_W     40
#define CHESS_H     40

#define RED         8
#define GREEN       1

void game_init(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        GAME_ERROR("SDL_Init"); 
        SDL_Quit();
    }

    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
    if (screen == NULL)
    {
        GAME_ERROR("SDL_SetVideoMode"); 
        SDL_Quit();
    }

    SDL_WM_SetCaption("China Chess", NULL);
}

void load_data(void)
{
    int i, j;
    char filename[100];

    pic_load(&background, "pics/board.jpg");
    pic_load(&current, "pics/box.jpg");

    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < CH_NUM; j++)
        {
            snprintf(filename, sizeof(filename), "pics/%d_%d.jpg", j + 1, i + 1);
            pic_load(&(chess[i][j]), filename);
        }
    }
    cur_key.x = 4;
    cur_key.y = 9;
    val.name = R_MARSHAL;
    val.src_pos = cur_key;
    flag = 1;
}

BOOL phy_to_virt(struct pos_t *index, int x, int y)
{
    if ((x >= 0 && x <= SCREEN_WIDTH) && (y >= 0 && y <= SCREEN_HEIGHT) && \
        ((x - PHY_S_X) / SPACE_X * SPACE_X + CHESS_W + 5 >= x) && \
        ((y - PHY_S_Y) / SPACE_Y * SPACE_Y + CHESS_H + 5 >= y))
            //实际坐标减去实际起始坐标除以间距，即得到对应数据下标，再乘以间距加上棋子宽度如果大于等于实际坐标即选中实际棋子范围，加上5只是为了扩大一些范围好点击
    {
        index->x = (x - PHY_S_X) / SPACE_X;
        index->y = (y - PHY_S_Y) / SPACE_Y;   
        
        return TRUE;
    }
    else
       return FALSE;
       
}


void show_board(struct pos_t *index)
{
    int i, j, x, y;

    apply_surface(0, 0, background, screen);

    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++)
        {
                printf("%5d", chessboard[i][j]);
            if (chessboard[i][j])
            {
                if (index && index->y == i && index->x == j)
                    apply_surface(j * SPACE_X + PHY_S_X, i * SPACE_Y + PHY_S_Y, \
                    chess[1][chessboard[i][j] - 1], screen);     //i和j对应窗口y和x
                else
                    apply_surface(j * SPACE_X + PHY_S_X, i * SPACE_Y + PHY_S_Y,  \
                    chess[0][chessboard[i][j] - 1], screen);
            }
        }
        printf("\n");
    }

    if (SDL_Flip(screen) != 0)
    {
        GAME_ERROR("SDL_Flip!");
    }
}

void chess_handle(int x, int y)
{
    struct pos_t index;
    int ret;

    index.x = x;
    index.y = y;

    if (!flag)
    {
        if (!chessboard[index.y][index.x])
            return;
        val.name = chessboard[index.y][index.x];
    //    printf("name = %d, phy_x = %d, phy_y = %d, index.x = %d, index.y = %d\n", val.name, x, y, index.x, index.y);
 
        if ((!turn && val.name < 8) || (turn && val.name > 7))
                return;
/*        
        if (!turn)
        {
            if (val.name < 8)
                return;
        }
        else
        {
            if (val.name > 7)
                return;
        }
*/
        val.src_pos = index; 
        show_board(&index);
        flag = 1;        
        return;
    }
    else
    {
        if (chessboard[index.y][index.x] && ((!turn && chessboard[index.y][index.x] > 7) ||  \
            (turn && chessboard[index.y][index.x] != 0 && chessboard[index.y][index.x] < 8)))
        {
            val.name = chessboard[index.y][index.x];
            cur_key = index;
            val.src_pos = index; 
            show_board(&index);
            flag = 1;
            return;
        }

        val.dst_pos = index;

   //     printf("src.x = %d, src.y = %d, dst.x = %d, dst.y = %d\n", val.src_pos.x, val.src_pos.y, val.dst_pos.x, val.dst_pos.y);
        ret = analysis(&val);
    
        printf("ret = %d\n", ret);

        switch(ret)
        {
            case SUCCESS:
                printf("Success\n");
                show_board(&index);
                apply_surface(val.src_pos.x * SPACE_X + PHY_S_X, val.src_pos.y * SPACE_Y + PHY_S_Y, current, screen);
                SDL_Flip(screen);

                turn = ~turn;
                break;

            case GAME_OVER:
                show_board(NULL);
                printf("GAME_OVER\n");
                break;

            case NUL:
            default:
                return;
        }
       
        flag = 0;
    }
}

void key_handle(SDLKey sym)
{
    int ret;
    switch(sym)
    {
        case SDLK_UP:
            if (cur_key.y > 0)
                cur_key.y--;
            break;

        case SDLK_DOWN:
             if (cur_key.y < ROW - 1)
                cur_key.y++;
            break;

        case SDLK_LEFT:
             if (cur_key.x > 0)
                cur_key.x--;
            break;

        case SDLK_RIGHT:
             if (cur_key.x < COL - 1)
                cur_key.x++;
            break;

        case SDLK_SPACE:
            chess_handle(cur_key.x, cur_key.y);
            return;

        default:
            break;
    }
    show_board(NULL);
    apply_surface(cur_key.x * SPACE_X + PHY_S_X, cur_key.y * SPACE_Y + PHY_S_Y, current, screen);
    SDL_Flip(screen); 
}

void pressESCtoQuit_while()
{
    int gameOver = 0;
    struct pos_t index;
    SDL_Event gameEvent;
    while(gameOver == 0)
    {
        while(SDL_PollEvent(&gameEvent) != 0)
        {
            if (gameEvent.type == SDL_QUIT)
            {
                gameOver = 1;
            }
            if (gameEvent.type == SDL_MOUSEBUTTONDOWN)
            {
                if (phy_to_virt(&index, gameEvent.motion.x, gameEvent.motion.y) == FALSE)
                    break; 
                chess_handle(index.x, index.y);
            }
            if (gameEvent.type == SDL_KEYUP)
            {

                if (gameEvent.key.keysym.sym == SDLK_ESCAPE)
                {
                    gameOver = 1;
                    break; 
                }
                key_handle(gameEvent.key.keysym.sym);
            }
        }
                SDL_Delay(10);
    }
}

int main(void)
{
    game_init();
    load_data();

    show_board(&cur_key);

    atexit(SDL_Quit);
    
    pressESCtoQuit_while();

    SDL_Quit();

    return 0;
}

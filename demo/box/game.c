#include <box.h>


void list_map(GameEngine *ge)
{
    int i, j; 

    for (i = 0; i < VIRT_H; i++)
    {
        for (j = 0; j < VIRT_W; j++) 
        {
            if (ge->box_buf[i][j]) 
                printf(" 1 ");
            else
                printf(" 0 ");
        }
        printf("\n");
    }
    printf("=================================\n");
}


static void new_box(BOX *handle)
{
    /*handle->type = rand() % sizeof(square) / (4 * 4 * 4);*/
    /*handle->type = rand() % 4;*/
    handle->type = rand() % 2;
    handle->dir = rand() % 4;
    handle->color = rand() % 0xffff00 + 0xff;
}

static void move_box(BOX *handle, int x, int y)
{
    handle->x = x;
    handle->y = y;
}

static int box_pixel(BOX *handle, int x, int y)
{
    int ret;
    int type = handle->type;
    int dir = handle->dir;

    if (type == -1)
        return 0;
    return square[type][dir][x][y];
}


void remove_box(GameEngine *ge)
{
    int i, j, x, y;

    for (i = 0; i < BOX_SIZE; i++)
    {
        for (j = 0; j < BOX_SIZE; j++) 
        {
            x = ge->curr_box.x + j; 
            y = ge->curr_box.y + i; 
            if (x < 0 || x >= VIRT_W || y < 0 || y >= VIRT_H)
                continue;
            if (box_pixel(&ge->curr_box, i, j))
                ge->box_buf[y][x] = 0;
        }
    }
}

int put_box(GameEngine *ge)
{
    int i, j, x, y;

    for (i = 0; i < BOX_SIZE; i++)
    {
        for (j = 0; j < BOX_SIZE; j++) 
        {
            if(!box_pixel(&ge->curr_box, i, j))
                continue;
            x = ge->curr_box.x + j;
            y = ge->curr_box.y + i;

            if (x < 0 || x >= VIRT_W || y >= VIRT_H)
                return -1;
            if (y < 0)
                continue;
            if (ge->box_buf[y][x] != 0)
                return -1;
        }
    }

    for (i = 0; i < BOX_SIZE; i++)
    {
        for (j = 0; j < BOX_SIZE; j++) 
        {
            x = ge->curr_box.x + j; 
            y = ge->curr_box.y + i; 
            if (x < 0 || x >= VIRT_W || y < 0 || y >= VIRT_H)
                continue;
            if (box_pixel(&ge->curr_box, i, j))
                ge->box_buf[y][x] = ge->curr_box.color;
        }
    }

    return 0;
}

int box_left(GameEngine *ge)
{
    remove_box(ge);
    ge->curr_box.x--;
    if (put_box(ge))
    {
        ge->curr_box.x++; 
        put_box(ge);
    }
    box_show(ge);
    return 0;
}

int box_right(GameEngine *ge)
{
    remove_box(ge);
    ge->curr_box.x++;
    if (put_box(ge))
    {
        ge->curr_box.x--;
        put_box(ge);
    }
    box_show(ge);
    return 0;
}

int box_rotate(GameEngine *ge)
{
    remove_box(ge);
    ge->curr_box.dir = (ge->curr_box.dir + 1) % 4;
    if (put_box(ge))
    {
        ge->curr_box.dir = (ge->curr_box.dir + 3) % 4;
        put_box(ge);
    }
    box_show(ge);

    return 0;
}

int box_down(GameEngine *ge)
{
    int i, j, c, l = 0;

    if (ge->game_over)
        return -1;
    remove_box(ge);
    ge->curr_box.y++;
    if (put_box(ge))
    {
        ge->curr_box.y--; 
        put_box(ge);

        for (i = 0; i < VIRT_W; i++)
        {
            if (ge->box_buf[0][i] != 0) 
            {
                ge->game_over = 1;
                return -1;
            }
        }

        for (i = VIRT_H - 1; i > 0; i--)
        {
            c = 0;
            for (j = 0; j < VIRT_W; j++) 
            {
                if (ge->box_buf[i][j] != 0) 
                    c++;
            }
            if (c == VIRT_W)
            {
                for (j = 0; j < VIRT_W; j++)
                {
                    ge->box_buf[i][j] = 0;
                    box_show(ge);
                    usleep(80 * 1000);
                }
                for (j = i; j > 0; j--)
                {
                    memmove(ge->box_buf + j, ge->box_buf + j - 1, 4 * VIRT_W); 
                }
                i++;
                ge->score += 100;

                /*if (ge->score == ge->level * 2000)*/
                    ge->level++; 

                box_show_data(ge);
            }
        }

        ge->curr_box = ge->next_box;
        new_box(&ge->next_box);
        move_box(&ge->curr_box, VIRT_W / 2 - 2, -3);
        
        put_box(ge);
        box_show_next(ge);
    }

    box_show(ge);

    return 0;
}

void box_clear(GameEngine *ge)
{
    /*int i, j;*/
    memset(ge->box_buf, 0, sizeof(ge->box_buf));
    /*
     *for (i = 0; i < VIRT_H; i++)
     *{
     *    for (j = 0; j < VIRT_W; j++) 
     *    {
     *        ge->box_buf[j][i] = 0;
     *    }
     *}
     */
}
#if 0
int key_handle(void *data)
{
    GameEngine *ge = (GameEngine *)data;

    while (!ge->game_over)
    {
        SDL_PollEvent(&ge->event);

        if (key_state.quit)
            ge->game_over = 1;

        if (!key_state.pause)
        {
            if (key_state.left)
                box_left(ge);
            else if (key_state.right)
                box_right(ge);
            else if (key_state.rotate)
                box_rotate(ge);
            else if (key_state.down)
                box_down(ge);
        }
        /*usleep(100 * 1000);*/
        SDL_Delay(80);
    }

    return 0;
}
#else

struct key_t {
    KEYSTATE key;
    GameEngine *ge;
};

int key_state_handle(void *data)
{
    struct key_t *k = (struct key_t *)data;

    while (!k->key.quit)
    {
        if (!k->key.pause)
        {
            if (k->key.left)    
                box_left(k->ge);
            else if (k->key.right)
                box_right(k->ge);
            else if (k->key.down)
                box_down(k->ge);
        }
        SDL_Delay(80);
    }

    return 0; 
}

int key_handle(void *data)
{
    GameEngine *ge = (GameEngine *)data;
    struct key_t key_data;
    SDL_Thread *th = NULL;

    memset(&key_data, 0, sizeof(key_data));
    key_data.ge = ge;

    th = SDL_CreateThread(key_state_handle, &key_data);

    while (!ge->game_over)
    {
        if (SDL_WaitEvent(&ge->event))
        {
            switch (ge->event.type)
            {
                case SDL_QUIT:
                    ge->game_over = 1;
                    key_data.key.quit = 1;
                    break;
                case SDL_KEYDOWN:
                    switch (ge->event.key.keysym.sym)
                    {
                        case SDLK_LEFT:
                            key_data.key.left = 1;
                            /*box_left(ge);*/
                            break;
                        case SDLK_RIGHT:
                            key_data.key.right = 1;
                            /*box_right(ge);*/
                            break;
                        case SDLK_UP:
                            box_rotate(ge);
                            break;
                        case SDLK_DOWN:
                            key_data.key.down = 1;
                            /*box_down(ge);*/
                            break;
                        case SDLK_ESCAPE:
                            key_data.key.quit = 1;
                            ge->game_over = 1;
                            break;
                        case SDLK_SPACE:
                            if (ge->pause)
                                ge->pause = 0;
                            else
                                ge->pause = 1;
                            key_data.key.pause = ge->pause;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (ge->event.key.keysym.sym)
                    {
                        case SDLK_LEFT:
                            key_data.key.left = 0;
                            break;
                        case SDLK_RIGHT:
                            key_data.key.right = 0;
                            /*box_right(ge);*/
                            break;
                        case SDLK_DOWN:
                            key_data.key.down = 0;
                            /*box_down(ge);*/
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
    }

    SDL_KillThread(th);

    return 0;
}
#endif
void game_start(GameEngine *ge)
{
    new_box(&ge->curr_box);
    new_box(&ge->next_box);
    move_box(&ge->curr_box, VIRT_W / 2 - 2, 0);
    box_clear(ge);
    ge->level = 1;
    put_box(ge);
    box_show(ge);
    box_show_next(ge);
    box_show_data(ge);
    ge->game_over = 0;
    ge->th = SDL_CreateThread(key_handle, ge);
}


int play_game(GameEngine *ge)
{
    game_start(ge);

    while (!ge->game_over)
    {
        usleep((LEVEL - ge->level) * 50 * 1000);
        if (!ge->pause)
            box_down(ge);
    }
    
    return 0;
}

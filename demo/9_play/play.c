#include <play.h>

int mplayer_init(struct play_t *play);
void mplayer_exit(struct play_t *play);
void mplayer_play(struct play_t *play);
void mplayer_clean(int mes);
int mplayer_send_cmd(struct play_t *play, const char *cmd);
char *mplayer_recv(struct play_t *play);
void show_play(struct play_t *play);
int lrc_add_node(const char *content, struct play_t *play);
void lrc_destroy(struct play_t *play);

void file_select(int key, int state, void *data)
{
    struct play_t *play = (struct play_t *)data;

    views_load(play->views);

    views_run(play->views);
    
    strcpy(play->filepath, views_select(play->views));

    /*printf("filepath = %s\n", play->filepath);*/

    /*mplayer_play(play);*/
    play_load(play);

    /*printf("length = %d\n", play->time_length);*/

    show_play(play);
}

int lrc_add_node(const char *content, struct play_t *play)
{
    char *front = NULL, *end = NULL, *sec = NULL, *ms = NULL;
    int clen = 0;
    struct lrc_node_t *new = NULL;

    if ((front = strchr(content, '[')) == NULL ||
        (sec = strchr(front + 1, ':')) == NULL ||
        (ms = strchr(sec + 1, '.')) == NULL ||
        (end = strchr(content, ']')) == NULL ||
        (clen = strlen(end + 1)) < 1)
        goto ERR1;

    new = (struct lrc_node_t *)malloc(sizeof(struct lrc_node_t));
    SDL_ERRP(NULL == new, goto ERR1, 2, "malloc node!\n");

    new->ms = atoi(front + 1) * 60000 + atoi(sec + 1) * 1000 + atoi(ms + 1) * 10;
    new->content = (char *)malloc(clen + 1);
    SDL_ERRP(NULL == new->content, goto ERR2, 2, "malloc content!\n");
    strcpy(new->content, end + 1);
    printf("ms = %d, content = %s\n", new->ms, new->content);

    new->fg = TTF_RenderUTF8_Blended(play->font, new->content, kyo_color(play->fg_color));
    SDL_ERRP(NULL == new->fg, goto ERR3, 2, "TTF_RenderUTF8_Blended fg!\n");

    new->bg = TTF_RenderUTF8_Blended(play->font, new->content, kyo_color(play->bg_color));
    SDL_ERRP(NULL == new->bg, goto ERR4, 2, "TTF_RenderUTF8_Blended bg!\n");

    new->next = &play->lrc_head;
    new->prev = play->lrc_head.prev;
    play->lrc_head.prev->next = new;
    play->lrc_head.prev = new;

    return 0;
ERR4:
    SDL_FreeSurface(new->fg);
ERR3:
    free(new->content);
ERR2:
    free(new);
ERR1:
    return -1;
}

void lrc_destroy(struct play_t *play)
{
    struct lrc_node_t *tail = play->lrc_head.next, *save = NULL;

    while (tail != &play->lrc_head)
    {
        save = tail->next;
        SDL_FreeSurface(tail->fg);
        SDL_FreeSurface(tail->bg);
        free(tail->content);
        free(tail);
        tail = save;
    }
    play->lrc_head.next = &play->lrc_head;
    play->lrc_head.prev = &play->lrc_head;
}


int load_lrc(struct play_t *play)
{
    FILE *fp = NULL;
    int len;

    fp = fopen(play->tmpbuf, "r");
    SDL_ERRP(NULL == fp, goto ERR1, 2, "%s error!\n", play->tmpbuf);

    while (fgets(play->tmpbuf, PATHMAX, fp) != NULL)
    {
        len = strlen(play->tmpbuf);
        if (play->tmpbuf[len - 1] == '\n')
            play->tmpbuf[len - 1] = '\0';

        lrc_add_node(play->tmpbuf, play);          
    }

    fclose(fp);

    return 0;
ERR1:
    return -1;
}

static int _get_time_pos(struct play_t *play) 
{
    int ret;
    char *p = NULL;

    SDL_mutexP(play->mutex);
    while (1)
    {
        ret = mplayer_send_cmd(play, "get_time_pos\n");
        /*printf("send = %d\n", ret);*/
        if (ret <= 0)
            continue;
        p = mplayer_recv(play);
        if (p != NULL)
        {
            /*printf("recv p = %s\n", p);*/
            ret = atof(p) * 1000;
            break;
        }
    }
    SDL_mutexV(play->mutex);

    return ret;
}

struct lrc_node_t *find_time_node(int cur_time, struct play_t *play)
{
    struct lrc_node_t *tail = NULL;

    printf("cur_time = %d\n", cur_time);

    for (tail = play->lrc_head.next; tail != &play->lrc_head; tail = tail->next)
    {
        if (cur_time < tail->ms 
                || tail->next == &play->lrc_head 
                || (cur_time >= tail->ms 
                        && cur_time < tail->next->ms))
        {
            /*printf("tail->ms = %d\n", tail->ms);*/
            return tail;
        }
    }

    return play->lrc_head.next;
}

int lrc_handle(void *data)
{
    struct play_t *play = (struct play_t *)data;
    struct lrc_node_t *cur = NULL, *save = NULL;
    SDL_Rect offset = {0, 0};
    int i, ms, cur_time;

    SDL_Delay(play->lrc_head.next->ms);

    while (!play->lrc_quit)
    {
        cur_time = _get_time_pos(play);
        cur = find_time_node(cur_time, play);
        if (cur == save)
        { 
            SDL_Delay(1);
            continue;
        }

        draw_rect(play->sdl->screen, 0, 0, play->sdl->w, FONT_SIZE, 0x0);
        ms = (cur->next->ms - cur_time) / cur->bg->w;

        offset.w = (cur_time - cur->ms) / (cur->next->ms - cur->ms) * cur->bg->w;
        offset.h = cur->bg->h;
        while (offset.w < cur->bg->w)
        {
            SDL_BlitSurface(cur->bg, NULL, play->sdl->screen, NULL);
            offset.w++;
            SDL_BlitSurface(cur->fg, &offset, play->sdl->screen, NULL);
        
            SDL_Flip(play->sdl->screen);
            SDL_Delay(ms);
        }

        save = cur;
        /*printf("delay = %d, cur->next->ms = %d, cur->ms = %d\n", cur->next->ms - cur->ms, cur->next->ms, cur->ms);*/
        /*SDL_Delay(cur->next->ms - cur->ms);*/
    }

    return 0;
}

int play_load(struct play_t *play)
{
    char *p = NULL;

    strcpy(play->tmpbuf, play->filepath);
    p = strrchr(play->tmpbuf, '.');
    SDL_ERRP(p == NULL, return -1, 2, "path error!\n");
    strncpy(p, ".lrc", 5);

    /*printf("lrcpath = %s\n", play->tmpbuf);*/

    if (load_lrc(play) == 0)
    {
        play->lrc_thread = SDL_CreateThread(lrc_handle, play);
        SDL_ERRP(play->lrc_thread == NULL, goto ERR1, 2, "SDL_CreateThread!\n");
    }

    mplayer_play(play);

    return 0;
ERR1:
    return -1;
}

void play_run(struct play_t *play)
{
    sdl_run(play->sdl);
}

void show_play(struct play_t *play)
{
    draw_rect(play->sdl->screen, 0, 0, WIDTH, HEIGHT, 0x0);
    SDL_Flip(play->sdl->screen);
}

struct play_t *play_init(const char *path)
{
    struct play_t *play = NULL;
    int ret;

    play = (struct play_t *)malloc(sizeof(struct play_t));
    SDL_ERRP(NULL == play, goto ERR1, 2, "malloc play failed!\n");
    memset(play, 0, sizeof(struct play_t));

    play->sdl = sdl_init(WIDTH, HEIGHT, BPP, KYO_VIDEO | KYO_TTF | KYO_IMG);
    SDL_ERRP(NULL == play->sdl, goto ERR2, 2, "sdl_init!\n");

    play->fg_color = LRC_FG_COLOR;
    play->bg_color = LRC_BG_COLOR;

    play->font = TTF_OpenFont("./res/simfang.ttf", FONT_SIZE);
    SDL_ERRP(NULL == play->font, goto ERR2, 2, "TTF_OpenFont!\n");

    TTF_SetFontStyle(play->font, TTF_STYLE_BOLD);

    play->lrc_head.next = &play->lrc_head;
    play->lrc_head.prev = &play->lrc_head;

    ret = mplayer_init(play);
    SDL_ERRP(-1 == ret, goto ERR2, 2, "mplayer_init!\n");

    play->global.sdl = sdl_copy(play->sdl);

    play->views = views_init("/kyo", MUSIC, &play->global);
    SDL_ERRP(NULL == play->views, goto ERR2, 2, "views_init!\n");
    views_save(play->views);

    play->mutex = SDL_CreateMutex();
    SDL_ERRP(play->mutex == NULL, goto ERR2, 2, "SDL_CreateMutex!\n");

    if (path != NULL)
    {
        strcpy(play->filepath, path);
        play_load(play);
    }

    sdl_register_key(play->sdl, SDLK_o, KYO_KEYDOWN, file_select, play);

    show_play(play);

    return play;
ERR2:
    play_exit(play);
ERR1:
    return NULL;
}

int mplayer_init(struct play_t *play)
{
    int ret;

    unlink(CTL_PATH);
    ret = mkfifo(CTL_PATH, 0777);
    SDL_ERRP(-1 == ret, goto ERR1, 2, "mkfifo CTL!\n");

    unlink(MES_PATH);
    ret = mkfifo(MES_PATH, 0777);
    SDL_ERRP(-1 == ret, goto ERR2, 2, "mkfifo MES!\n");

    play->ctl = open(CTL_PATH, O_RDWR);
    SDL_ERRP(-1 == play->ctl, goto ERR2, 2, "open CTL!\n");

    play->mes = open(MES_PATH, O_RDWR);
    SDL_ERRP(-1 == play->mes, goto ERR2, 2, "open MES!\n");
    return 0;
ERR2:
    mplayer_exit(play);
ERR1:
    return -1;
}

void mplayer_play(struct play_t *play)
{
    char *p = NULL;
    int ret;

    if (play->pid != 0)
        kill(play->pid, SIGTERM);

    if ((play->pid = fork()) == 0)
    {
        close(2);
        dup2(play->mes, 1);
        exit(execlp("mplayer", "mplayer", play->filepath, "-quiet", "-slave", "-input", "file="CTL_PATH, NULL)); 
    }
    usleep(500000);
    SDL_mutexP(play->mutex);
    while (1)
    {
        ret = mplayer_send_cmd(play, "get_time_length\n");
        if (ret <= 0)
            continue;

        p = mplayer_recv(play);
        if (p != NULL)
        {
            play->time_length = atof(p) * 1000;
            break;
        }
    }
    printf("time_length = %d\n", play->time_length);
    SDL_mutexV(play->mutex);
}

int mplayer_send_cmd(struct play_t *play, const char *cmd)
{
    mplayer_clean(play->mes);
    return write(play->ctl, cmd, strlen(cmd));
}

void mplayer_clean(int mes)
{
    int flag;
    char buf[1024];

    flag = fcntl(mes, F_GETFL);
    fcntl(mes, F_SETFL, flag | O_NONBLOCK);

    while (read(mes, buf, sizeof(buf)) > 0)
        ;

    fcntl(mes, F_SETFL, flag);
}

char *mplayer_recv(struct play_t *play)
{
    int ret;
    char *p = NULL;

    ret = read(play->mes, play->tmpbuf, PATHMAX);
    /*printf("read = %d\n", ret);*/
    if (ret <= 0)
        return NULL;
    play->tmpbuf[ret] = '\0';
    /*printf("buf = %s", play->tmpbuf);*/
    /*fflush(stdout);*/
    p = strrchr(play->tmpbuf, '=');
    if (p == NULL || *(p + 1) < '0' || *(p + 1) > '9')
        return NULL;
    return p + 1;
}

void mplayer_exit(struct play_t *play)
{
    if (play->ctl != 0)
        close(play->ctl);
    if (play->mes != 0)
        close(play->mes);
    unlink(CTL_PATH);
    unlink(MES_PATH);
    if (play->pid != 0)
        kill(play->pid, SIGTERM);
}

void play_exit(struct play_t *play)
{
    if (play->lrc_thread != NULL)
    {
        play->lrc_quit = 1;
        SDL_WaitThread(play->lrc_thread, NULL);
    }
    SDL_DestroyMutex(play->mutex);
    lrc_destroy(play);
    TTF_CloseFont(play->font);
    views_exit(play->views);
    sdl_exit(play->sdl);
    mplayer_exit(play);
    free(play);
}

#include <views.h>

int key_handle(void *data)
{
    struct views_t *views = (struct views_t *)data;

    while (!views->thread_quit)
    {
        if (views->key_up)
        {
            if (views->cur->prev == &views->list)
                continue;

            if (views->page_count > 0)
                views->page_count--;
            else
                views->start = views->start->prev;

            views->cur = views->cur->prev;

            show_views(views);
        }
        else if (views->key_down)
        {
            if (views->cur->next == &views->list)
                continue;

            if (views->page_count < LIST_MAX - 1)
                views->page_count++;
            else
                views->start = views->start->next;
            views->cur = views->cur->next;

            show_views(views);
        }

        SDL_Delay(100);
    }

    return 0;
}

void views_move(int key, int state, void *data)
{
    struct views_t *views = (struct views_t *)data;

    if (state == KYO_KEYDOWN)
    {
        if (key == SDLK_UP)
            views->key_up = 1;
        else
            views->key_down = 1;
    }
    else
    {
        if (key == SDLK_UP)
            views->key_up = 0;
        else
            views->key_down = 0;
    }
}

void views_return(int key, int state, void *data)
{
    struct views_t *views = (struct views_t *)data;
    char *p = NULL;

    if (!strcmp(views->rootpath, views->curpath))
        return;

    p = strrchr(views->curpath, '/');
    if (p == NULL)
        return;

    *p = '\0';

    list_destroy(views);
    list_load(views);
    show_views(views);
}

void views_enter(int key, int state, void *data)
{
    struct views_t *views = (struct views_t *)data;
    char *filename = views->cur->filename;

    if (views->cur->type == KDIR)
    {
        strcat(views->curpath, "/");
        strcat(views->curpath, views->cur->filename);
        /*printf("curpath = %s\n", views->curpath);*/
        list_destroy(views);
        list_load(views);
        show_views(views);
    }
    else if (views->cur->type == UNKNOWN)
    {
    
    }
    else
    {
        snprintf(views->tmpbuf, PATHMAX, "%s/%s.%s", views->curpath, filename, filename + strlen(filename) + 1);
        views->sdl->quit = 1;
    }
#if 0
    else if (views->cur->type == EBOOK)
    {
        if (fork() == 0)
        {
            strcat(views->curpath, "/");
            views->cur->filename[strlen(views->cur->filename)] = '.';
            strcat(views->curpath, views->cur->filename);
            printf("views curpath = %s\n", views->curpath);
            exit(execlp("gvim", "gvim", views->curpath, NULL));
        }
    }
    else if (views->cur->type == MUSIC || views->cur->type == VIDEO)
    {
        if (fork() == 0)
        {
            strcat(views->curpath, "/");
            views->cur->filename[strlen(views->cur->filename)] = '.';
            strcat(views->curpath, views->cur->filename);
            printf("views curpath = %s\n", views->curpath);
            exit(execlp("mplayer", "mplayer", views->curpath, NULL));
        }
    }
#endif
}

char *views_select(struct views_t *views)
{
    return views->tmpbuf;
}

static void show_mes(struct views_t *views)
{
    SDL_Surface *txt = NULL;
    SDL_Rect offset = {MES_X, MES_Y};
    char *filename = views->cur->filename;
    int type = views->cur->type;

    SDL_BlitSurface(views->res.big[type], NULL, views->sdl->screen, NULL);

    if ((type == KDIR || type == UNKNOWN))
        snprintf(views->tmpbuf, PATHMAX, "文件名称: %s", filename);
    else
        snprintf(views->tmpbuf, PATHMAX, "文件名称: %s.%s", filename, filename + strlen(filename) + 1);

    txt = TTF_RenderUTF8_Blended(views->res.font9, views->tmpbuf, views->fontcolor[views->cur->type]);

    SDL_BlitSurface(txt, NULL, views->sdl->screen, &offset);
    SDL_FreeSurface(txt);

    snprintf(views->tmpbuf, PATHMAX, "文件大小: %ld", views->cur->mes.st_size);
    txt = TTF_RenderUTF8_Blended(views->res.font9, views->tmpbuf, views->fontcolor[views->cur->type]);
    offset.y += 20;
    SDL_BlitSurface(txt, NULL, views->sdl->screen, &offset);
    SDL_FreeSurface(txt);

    strftime(views->tmpbuf, PATHMAX, "修改时间: %F %H:%M", localtime(&views->cur->mes.st_mtime));
    txt = TTF_RenderUTF8_Blended(views->res.font9, views->tmpbuf, views->fontcolor[views->cur->type]);
    offset.y += 20;
    SDL_BlitSurface(txt, NULL, views->sdl->screen, &offset);
    SDL_FreeSurface(txt);
}

void show_views(struct views_t *views)
{
    struct res_t *res = &views->res;
    SDL_Surface *screen = views->sdl->screen;
    SDL_Rect offset = {LIST_X, LIST_Y};
    SDL_Rect txt_offset = {LIST_X + 36, LIST_Y};
    struct node_t *tail = views->start;
    int i;

    SDL_SoftStretch(res->bg, NULL, screen, NULL);

    /*printf("num == %d\n", views->list_num);*/
    if (views->list_num != 0)
    {
        show_mes(views);
        txt_offset.y = LIST_Y + (LIST_H - tail->txt->h) / 2;
    }

    for (i = 0; i < LIST_MAX; i++)
    {
        offset.y = LIST_Y + i * LIST_H;
        if (views->cur == tail && views->list_num != 0)
            SDL_BlitSurface(res->fcs, NULL, screen, &offset);
        else
            SDL_BlitSurface(res->ufcs, NULL, screen, &offset);

        if (i >= views->list_num)
            continue;

        SDL_BlitSurface(res->small[tail->type], NULL, screen, &offset);
        SDL_BlitSurface(tail->txt, NULL, screen, &txt_offset);

        txt_offset.y += LIST_H;
        tail = tail->next;
    }

    SDL_Flip(screen);
}

struct views_t *views_init(const char *root, int filter, struct global_t *global)
{
    struct views_t *views = NULL;
    int ret;

    views = (struct views_t *)malloc(sizeof(struct views_t));
    SDL_ERRP(NULL == views, goto ERR1, 2, "malloc views failed!\n");

    bzero(views, sizeof(struct views_t));

    views->fontcolor[KDIR] = (SDL_Color){0, 0, 255};
    
    views->filter = filter;

    views->rootpath = (char *)malloc(strlen(root) + 1);
    SDL_ERRP(NULL == views->rootpath, goto ERR2, 2, "malloc rootpath failed!\n");
    strcpy(views->rootpath, root);
    strcpy(views->curpath, root);

    views->list.next = &views->list;
    views->list.prev = &views->list;

    if (global == NULL || global->sdl == NULL)
    {
        views->sdl = sdl_init(WIDTH, HEIGHT, BPP, KYO_VIDEO | KYO_TTF);
        SDL_ERRP(NULL == views->sdl, goto ERR2, 2, "sdl_init failed!\n");
    }
    else
        views->sdl = global->sdl;

    views->save_screen = SDL_CreateRGBSurface(SDL_SWSURFACE, views->sdl->w, views->sdl->h, views->sdl->bpp, 0, 0, 0, 0);
    SDL_ERRP(NULL == views->save_screen, goto ERR2, 2, "SDL_CreateRGBSurface failed!\n");

    ret = res_load(&views->res);
    SDL_ERRP(-1 == ret, goto ERR2, 2, "res_load failed!\n");

    ret = list_load(views);
    SDL_ERRP(ret == -1, goto ERR2, 2, "list_load failed!\n");

    sdl_register_key(views->sdl, SDLK_UP, KYO_KEYDOWN, views_move, views);
    sdl_register_key(views->sdl, SDLK_DOWN, KYO_KEYDOWN, views_move, views);
    sdl_register_key(views->sdl, SDLK_UP, KYO_KEYUP, views_move, views);
    sdl_register_key(views->sdl, SDLK_DOWN, KYO_KEYUP, views_move, views);


    sdl_register_key(views->sdl, SDLK_LEFT, KYO_KEYDOWN, views_return, views);
    sdl_register_key(views->sdl, SDLK_RIGHT, KYO_KEYDOWN, views_enter, views);
    sdl_register_key(views->sdl, SDLK_RETURN, KYO_KEYDOWN, views_enter, views);
    sdl_register_key(views->sdl, SDLK_BACKSPACE, KYO_KEYDOWN, views_return, views);

    chdir(views->rootpath);

    views->th = SDL_CreateThread(key_handle, views);
    SDL_ERRP(NULL == views->th, goto ERR2, 2, "SDL_CreateThread failed!\n");

    show_views(views);

    return views;
ERR2:
    views_exit(views);
ERR1:
    return NULL;
}

void views_load(struct views_t *views)
{
    views->sdl->quit = 0;
    SDL_BlitSurface(views->save_screen, NULL, views->sdl->screen, NULL);
    SDL_Flip(views->sdl->screen);
}

void views_save(struct views_t *views)
{
    SDL_BlitSurface(views->sdl->screen, NULL, views->save_screen, NULL);
}

void views_run(struct views_t *views)
{
    sdl_run(views->sdl);
    views_save(views);
}

void views_exit(struct views_t *views)
{
    views->thread_quit = 1;
    SDL_WaitThread(views->th, NULL);
    res_destroy(&views->res);
    if (views->sdl)
        sdl_exit(views->sdl);
    SDL_FreeSurface(views->save_screen);
    list_destroy(views);
    free(views->rootpath);
    free(views);
}

int res_load(struct res_t *res)
{
    int i, index = TYPEMAX * 2;

    for (i = 0; i < TYPEMAX; i++)
    {
        res->big[i] = pic_load(resbmp[i]);
        SDL_ERRP(NULL == res->big[i], goto ERR1, 2, "pic_load %s\n", resbmp[i]);
        res->small[i] = pic_load(resbmp[i + TYPEMAX]);
        SDL_ERRP(NULL == res->small[i], goto ERR1, 2, "pic_load %s\n", resbmp[i]);
    }
    res->bg = pic_load(resbmp[index]);
    SDL_ERRP(NULL == res->bg, goto ERR1, 2, "pic_load %s\n", resbmp[index]);
    res->fcs = pic_load(resbmp[index + 1]);
    SDL_ERRP(NULL == res->fcs, goto ERR1, 2, "pic_load %s\n", resbmp[index + 1]);
    res->ufcs = pic_load(resbmp[index + 2]);
    SDL_ERRP(NULL == res->ufcs, goto ERR1, 2, "pic_load %s\n", resbmp[index + 2]);

    res->font12 = TTF_OpenFont(resttf[0], 16);
    SDL_ERRP(NULL == res->font12, goto ERR1, 2, "TTF_OpenFont 12 %s\n", resttf[0]);

    TTF_SetFontStyle(res->font12, TTF_STYLE_BOLD);

    res->font9 = TTF_OpenFont(resttf[0], 9);
    SDL_ERRP(NULL == res->font9, goto ERR1, 2, "TTF_OpenFont 9 %s\n", resttf[0]);
    TTF_SetFontStyle(res->font9, TTF_STYLE_BOLD);

    return 0;
ERR1:
    res_destroy(res);
    return -1;
}

void res_destroy(struct res_t *res)
{
    int i;

    for (i = 0; i < TYPEMAX; i++)
    {
        SDL_FreeSurface(res->big[i]); 
        SDL_FreeSurface(res->small[i]); 
    }
    SDL_FreeSurface(res->bg);
    SDL_FreeSurface(res->ufcs);
    SDL_FreeSurface(res->fcs);
    TTF_CloseFont(res->font12);
    TTF_CloseFont(res->font9);
}


static int get_file_type(char *filename)
{
    char *p = NULL;
    int i;
    int suffix_max = TYPEMAX - 2;
    char buf[128];

    p = strrchr(filename, '.');
    if (p == NULL)
        return UNKNOWN;
    *p++ = '\0';

    snprintf(buf, sizeof(buf), "|%s|", p);

    for (i = 0; i < suffix_max; i++)
    {
        if (strstr(typesuffix[i], buf)) 
            return i + 1;
    }

    *(p - 1) = '.';

    return UNKNOWN;
}

static int add_file(const char *filename, struct views_t *views)
{
    struct node_t *new = NULL;
    struct stat s;
    char *name = NULL;
    int type, ret;

    snprintf(views->tmpbuf, PATHMAX, "%s/%s", views->curpath, filename);
    ret = lstat(views->tmpbuf, &s);
    SDL_ERRP(-1 == ret, goto ERR1, 2, "lstat %s failed!\n", views->tmpbuf);

    name = (char *)malloc(strlen(filename) + 1);
    SDL_ERRP(NULL == name, goto ERR1, 2, "malloc filename %s\n", filename);
    strcpy(name, filename);

    if (S_ISDIR(s.st_mode))
        type = KDIR;
    else
    {
        type = get_file_type(name);
        if (views->filter != 0 && type != views->filter)
        {
            free(name); 
            return 0;
        }
    }

    new = (struct node_t *)malloc(sizeof(struct node_t));
    SDL_ERRP(NULL == new, goto ERR2, 2, "malloc node_t %s\n", filename);

    new->mes = s;
    new->filename = name;
    new->type = type;

    new->txt = TTF_RenderUTF8_Blended(views->res.font12, new->filename, views->fontcolor[new->type]);
    SDL_ERRP(NULL == new->txt, goto ERR3, 2, "TTF_RenderUTF8_Blended %s\n", filename);

    new->next = &views->list;
    new->prev = views->list.prev;
    views->list.prev->next = new;
    views->list.prev = new;

    views->list_num++;

    return 0;
ERR3:
    free(new);
ERR2:
    free(name);
ERR1:
    return -1;
}

int list_load(struct views_t *views)
{
    DIR *dp = NULL;
    struct dirent *dirp = NULL;

    dp = opendir(views->curpath);
    SDL_ERRP(dp == NULL, goto ERR1, 2, "opendir %s failed!\n", views->curpath);

    while ((dirp = readdir(dp)) != NULL)
    {
        if (dirp->d_name[0] == '.')
            continue;

        add_file(dirp->d_name, views);
    }

    views->cur = views->list.next;
    views->start = views->list.next;
    views->page_count = 0;

    closedir(dp);

    return 0;
ERR1:
    return -1;
}

void list_destroy(struct views_t *views)
{
    struct node_t *tail = views->list.next, *save = NULL;  
    while (tail != &views->list)
    {
        save = tail->next; 

        SDL_FreeSurface(tail->txt);
        free(tail->filename);
        free(tail);

        tail = save;
    }

    views->list.next = &views->list;
    views->list.prev = &views->list;
    views->list_num = 0;
}

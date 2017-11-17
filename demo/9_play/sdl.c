#include <sdl.h>

static void mouse_head_init(sdl_mouse_t (*mouse)[SDL_BUTTON_X2])
{
    int i, j;

    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < SDL_BUTTON_X2; j++)
        {
            mouse[i][j].next = &mouse[i][j];
            mouse[i][j].prev = &mouse[i][j];
        }
    }
}

struct sdl_t *sdl_init(int w, int h, int bpp, int flag)
{
    int ret;
    int m = SDL_INIT_VIDEO;
    struct sdl_t *sdl = NULL;

    if (flag & KYO_MUSIC || flag & KYO_CHUNK)
        m |= SDL_INIT_AUDIO;

    ret = SDL_Init(m);
    SDL_ERRP(-1 == ret, goto ERR1, 2, "SDL_Init: %s\n", SDL_GetError());

    sdl = (struct sdl_t *)malloc(sizeof(struct sdl_t));
    SDL_ERRP(NULL == sdl, goto ERR2, 2, "malloc sdl_t failed\n");

    memset(sdl, 0, sizeof(struct sdl_t));

    sdl->w = w;
    sdl->h = h;
    sdl->bpp = bpp;

    m = SDL_SWSURFACE;

    if (flag & KYO_FULLSCREEN)
        m |= SDL_FULLSCREEN;

    sdl->screen = SDL_SetVideoMode(w, h, bpp, m);
    SDL_ERRP(sdl->screen == NULL, goto ERR3, 2, "SDL_SetVideoMode: %s\n", SDL_GetError());

    if (flag & KYO_IMG)
    {
        sdl->img = 1;
        ret = IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF);
        SDL_ERRP(-1 == ret, goto ERR3, 2, "IMG_Init failed!\n");
    }

    if (flag & KYO_TTF)
    {
        sdl->ttf = 1;
        ret = TTF_Init();
        SDL_ERRP(-1 == ret, goto ERR3, 2, "TTF_Init failed!\n");
    }

    if (flag & KYO_MUSIC || flag & KYO_CHUNK)
    {
        sdl->audio = 1;
        if (flag & KYO_MUSIC)
            ret = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 512);
        else
            ret = Mix_OpenAudio(22050, AUDIO_S16, 1, 256);
        SDL_ERRP(-1 == ret, goto ERR3, 2, "Mix_OpenAudio failed!\n");
    }

    mouse_head_init(sdl->mouse);

    sdl_register_key(sdl, SDLK_ESCAPE, KYO_KEYDOWN, KYO_QUIT, NULL);

    return sdl;
ERR3:
    sdl_exit(sdl);
ERR2:
    SDL_Quit();
ERR1:
    return NULL;
}

struct sdl_t *sdl_copy(struct sdl_t *sdl)
{
    struct sdl_t *new_sdl = NULL;

    new_sdl = (struct sdl_t *)malloc(sizeof(struct sdl_t));
    SDL_ERRP(new_sdl == NULL, return NULL, 2, "malloc new_sdl failed!\n");
    *new_sdl = *sdl;

    new_sdl->copy = 1;

    memset(new_sdl->key, 0, sizeof(new_sdl->key));

    mouse_head_init(new_sdl->mouse);

    return new_sdl;
}

void sdl_mouse_destroy(sdl_mouse_t (*mouse)[SDL_BUTTON_X2])
{
    int i, j;
    sdl_mouse_t *tail = NULL, *save = NULL;

    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < SDL_BUTTON_X2; j++)
        {
            tail = mouse[i][j].next;
            while (tail != &mouse[i][j])
            {
                printf("hello\n");
                save = tail->next;
                free(tail);
                tail = save;
            }
        }
    }

}

void sdl_exit(struct sdl_t *sdl)
{
    sdl_mouse_destroy(sdl->mouse);

    if(sdl->copy)
    {
        free(sdl);
        return;
    }

    if (sdl->ttf)
        TTF_Quit();
    if (sdl->img)
        IMG_Quit();
    if (sdl->audio)
        Mix_CloseAudio();
    SDL_FreeSurface(sdl->screen);
    free(sdl);
    SDL_Quit();
}

int sdl_register_key(struct sdl_t *sdl, int key, int state, sdl_key_handle_t *handle, void *data)
{
    SDL_ERRP(key < SDLK_UNKNOWN || key > SDLK_LAST
                || state < KYO_KEYDOWN
                || state > KYO_KEYUP, return -1, 2, "register key failed!\n");

    sdl->key[state][key].handle = handle;
    sdl->key[state][key].data = data;

    return 0;
}

int sdl_register_mouse(struct sdl_t *sdl, int button, int state, SDL_Rect *rect, sdl_mouse_handle_t *handle, void *data)
{
    sdl_mouse_t *tail = NULL;

    SDL_ERRP(button < SDL_BUTTON_LEFT
                || button > SDL_BUTTON_X2
                || state < KYO_KEYDOWN
                || state > KYO_KEYUP, return -1, 2, "register key failed!\n");

    for (tail = sdl->mouse[state][button].next; tail != &sdl->mouse[state][button]; tail = tail->next)
    {
        if (!memcmp(&tail->rect, rect, sizeof(SDL_Rect)))
        {
            tail->handle = handle;
            tail->data = data;
            return 0;
        }
    }

    tail = (sdl_mouse_t *)malloc(sizeof(sdl_mouse_t));
    SDL_ERRP(NULL == tail, return -1, 2, "malloc mouse_node_t failed!\n");

    if (rect != NULL)
        tail->rect = *rect;
    else
        tail->rect.x = -1;

    tail->handle = handle;
    tail->data = data;

    tail->next = &sdl->mouse[state][button];
    tail->prev = sdl->mouse[state][button].prev;
    sdl->mouse[state][button].prev->next = tail;
    sdl->mouse[state][button].prev = tail;

    return 0;
}

void sdl_register_quit(struct sdl_t *sdl, sdl_quit_handle_t *handle, void *data)
{
    sdl->quit_handle = handle;
    sdl->quit_data = data;
}

static int _sdl_mouse_run(sdl_mouse_t *mouse, int type, SDL_Event *event)
{
    sdl_mouse_t *tail = NULL;

    for (tail = mouse->next; tail != mouse; tail = tail->next)
    {
        if (tail->handle == NULL)
            continue;

        if (tail->rect.x != -1)
        {
            if (event->button.x >= tail->rect.x
                && event->button.x <= tail->rect.x + tail->rect.w
                && event->button.y >= tail->rect.y
                && event->button.y <= tail->rect.y + tail->rect.h)
                if (tail->handle == KYO_QUIT)
                    return 1;
                else
                    tail->handle(event->button.button, type, event->button.x, event->button.y, tail->data);
        }
        else
        {
            if (tail->handle == KYO_QUIT)
                return 1;
            else
                tail->handle(event->button.button, type, event->button.x, event->button.y, tail->data);
        }
    }

    return 0;
}

int sdl_run(struct sdl_t *sdl)
{
    int i, type;
    SDL_Event event;
    sdl_key_t *key = NULL;
    sdl_mouse_t *mouse = NULL;

    while (!sdl->quit)
    {
        if (SDL_WaitEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                if (sdl->quit_handle)
                    sdl->quit_handle(sdl, sdl->quit_data);
                else
                    break;
            }
            else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
            {
                type = event.type - SDL_KEYDOWN;
                key = &sdl->key[type][event.key.keysym.sym];
                if (key->handle == KYO_QUIT)
                    break;
                else if (key->handle)
                    key->handle(event.key.keysym.sym, type, key->data);
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
            {
                type = event.type - SDL_MOUSEBUTTONDOWN;
                mouse = &sdl->mouse[type][event.button.button];
                if (_sdl_mouse_run(mouse, type, &event))
                    break;
            }
        }
    }

    return 0;
}

void draw_pixel(SDL_Surface *dst, int x, int y, unsigned int color)
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;
    offset.w = 1;
    offset.h = 1;

    SDL_FillRect(dst, &offset, SDL_MapRGB(dst->format,
        (char)(color >> 16), (char)(color >> 8), (char)color));
}

void draw_vline(SDL_Surface *dst, int x, int y, int l, unsigned int color)
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;
    offset.w = 1;
    offset.h = l;

    SDL_FillRect(dst, &offset, SDL_MapRGB(dst->format,
        (char)(color >> 16), (char)(color >> 8), (char)color));
}

void draw_hline(SDL_Surface *dst, int x, int y, int l, unsigned int color)
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;
    offset.w = l;
    offset.h = 1;

    SDL_FillRect(dst, &offset, SDL_MapRGB(dst->format,
        (char)(color >> 16), (char)(color >> 8), (char)color));
}

void draw_rect(SDL_Surface *dst, int x, int y, int w, int h, unsigned int color)
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;
    offset.w = w;
    offset.h = h;

    SDL_FillRect(dst, &offset, SDL_MapRGB(dst->format,
        (char)(color >> 16), (char)(color >> 8), (char)color));
}

void draw_box(SDL_Surface *dst, int x, int y, int w, int h, int border, unsigned int color, unsigned int border_color)
{
    draw_rect(dst, x, y, w, h, border_color);
    draw_rect(dst, x + border, y + border, w - 2 * border, h - 2 * border, color);
}

void draw_circle(SDL_Surface *dst, int x, int y, int r, unsigned int color)
{
    int i, j;

    for (i = x - r; i <= x + r; i++)
    {
        for (j = y - r; j <= y + r; j++)
        {
            if ((i - x) * (i - x) +  (j - y) * (j - y) <= r * r)
                draw_pixel(dst, i, j, color);
        }
    }
}

SDL_Surface *pic_load(const char *file)
{
    SDL_Surface *op = NULL, *img = NULL;

    img = IMG_Load(file);
    if (img == NULL)
        return NULL;

    op = SDL_DisplayFormat(img);
    if (op != NULL)
    {
        SDL_FreeSurface(img);
        img = op;
    }

    return img;
}

SDL_Color kyo_color(unsigned int color)
{
    SDL_Color c;

    c.r = (char)(color >> 16);
    c.g = (char)(color >> 8);
    c.b = (char)color;

    return c;
}

SDL_Rect kyo_rect(int x, int y, int w, int h)
{
    SDL_Rect r;

    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;

    return r;
}

int show_txt(SDL_Surface *dst, TTF_Font *font, const char *content, int x, int y, int w, int h, unsigned int color)
{
    SDL_Surface *txt = NULL;
    SDL_Rect rect;

    txt = TTF_RenderUTF8_Blended(font, content, kyo_color(color));
    SDL_ERRP(NULL == txt, return -1, 2, "TTF_RenderUTF8_Blended failed!\n");

    /*printf("show txt: y = %d, h = %d, txt->h = %d\n", y, h, txt->h);*/
    if (w > txt->w)
        x += (w - txt->w) / 2;
    if (h > txt->h)
        y += (h - txt->h) / 2;
    rect = kyo_rect(x, y, 0, 0);
    SDL_BlitSurface(txt, NULL, dst, &rect);

    SDL_UpdateRect(dst, x, y, txt->w, txt->h);

    SDL_FreeSurface(txt);

    return 0;
}


#include <sdl.h>

struct sdl_t *sdl_init(int width, int height, int bpp, int flag)
{
    struct sdl_t *sdl = NULL;
    unsigned long t = SDL_INIT_VIDEO;
    int ret;

    sdl = (struct sdl_t *)malloc(sizeof(struct sdl_t));
    KYO_ERRP(NULL == sdl, goto ERR1, 0);

    memset(sdl, 0, sizeof(struct sdl_t));

    sdl->key_head.next = &sdl->key_head;
    sdl->key_head.prev = &sdl->key_head;

    sdl->mouse_head.next = &sdl->mouse_head;
    sdl->mouse_head.prev = &sdl->mouse_head;

    if ((flag & KYO_MUSIC) || (flag & KYO_CHUNK))
    {
        sdl->audio = 1;
        t |= SDL_INIT_AUDIO;
    }

    ret = SDL_Init(t);
    KYO_ERRP(ret == -1, goto ERR2, 0);

    t = SDL_SWSURFACE;

    sdl->screen = SDL_SetVideoMode(width, height, bpp, t);
    KYO_ERRP(NULL == sdl->screen, goto ERR3, 0);

    if (flag & KYO_TTF)
    {
        sdl->ttf = 1;
        ret = TTF_Init();
        KYO_ERRP(ret == -1, goto ERR3, 0);
    }
    if (flag & KYO_MUSIC) 
    {
        ret = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 512);
        KYO_ERRP(ret == -1, goto ERR3, 0);
    }
    if (flag & KYO_CHUNK) 
    { 
        ret = Mix_OpenAudio(22050, AUDIO_S16, 1, 256);
        KYO_ERRP(ret == -1, goto ERR3, 0);
    }

    register_key(sdl, SDLK_ESCAPE, KYO_QUIT, NULL);

    return sdl;
ERR3:
    sdl_exit(sdl);
ERR2:
    free(sdl);
ERR1:
    return NULL;
}

int register_key(struct sdl_t *sdl, int key, key_handle_t *handle, void *data)
{
    struct key_node_t *new = NULL;

    for (new = sdl->key_head.next; new != &sdl->key_head; new = new->next)
    {
        if (key == new->key)
        {
            new->handle = handle; 
            new->data = data;
            return 0;
        }
    }

    new = (struct key_node_t *)malloc(sizeof(struct key_node_t));
    KYO_ERRP(new == NULL, goto ERR1, 0);

    new->key = key;
    new->handle = handle;
    new->data = data;

    new->next = &sdl->key_head;
    new->prev = sdl->key_head.prev;
    sdl->key_head.prev->next = new;
    sdl->key_head.prev = new;

    return 0;
ERR1:
    return -1;
}

int register_mouse(struct sdl_t *sdl, int button, mouse_handle_t *handle, void *data)
{
    struct mouse_node_t *new = NULL;

    new = (struct mouse_node_t *)malloc(sizeof(struct mouse_node_t));
    KYO_ERRP(new == NULL, goto ERR1, 0);

    new->button = button;
    new->handle = handle;
    new->data = data;

    new->next = &sdl->mouse_head;
    new->prev = sdl->mouse_head.prev;
    sdl->mouse_head.prev->next = new;
    sdl->mouse_head.prev = new;

    return 0;
ERR1:
    return -1;
}

int run_key(struct sdl_t *sdl)
{
    SDL_Event event;
    struct key_node_t *key_tail = NULL;
    struct mouse_node_t *mouse_tail = NULL;
    
    while (1)
    {
        if (SDL_WaitEvent(&event))
        {
            if (event.type == SDL_QUIT)
                break;
            if (event.type == SDL_KEYDOWN)
            {
                for (key_tail = sdl->key_head.next; key_tail != &sdl->key_head; key_tail = key_tail->next)
                {
                    if (event.key.keysym.sym == key_tail->key)
                    {
                        if (key_tail->handle == KYO_QUIT)
                            goto EXIT;
                        key_tail->handle(event.key.keysym.sym, key_tail->data);        
                    }
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                for (mouse_tail = sdl->mouse_head.next; mouse_tail != &sdl->mouse_head; mouse_tail = mouse_tail->next)
                {
                    if (event.button.button == mouse_tail->button)
                        mouse_tail->handle(event.button.button, event.button.x, 
                                            event.button.y, mouse_tail->data);        
                }
            }
        }
    }
EXIT:
    return 0;
}

void sdl_exit(struct sdl_t *sdl)
{
    struct key_node_t *key_tail = sdl->key_head.next, *key_save = NULL;
    struct mouse_node_t *mouse_tail = sdl->mouse_head.next, *mouse_save = NULL;

    while (mouse_tail != &sdl->mouse_head)
    {
        mouse_save = mouse_tail->next; 
        free(mouse_tail);
        mouse_tail = mouse_save;
    }

    while (key_tail != &sdl->key_head)
    {
        key_save = key_tail->next; 
        free(key_tail);
        key_tail = key_save;
    }

    if (sdl->ttf)
        TTF_Quit();
    if (sdl->audio)
        Mix_CloseAudio();
    SDL_FreeSurface(sdl->screen);
    SDL_Quit();
}

void draw_pixel(SDL_Surface *dst, int x, int y, int color)
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;
    offset.w = 1;
    offset.h = 1;

    SDL_FillRect(dst, &offset, SDL_MapRGB(dst->format, (char)(color >> 16), (char)(color >> 8), (char)color));
}

void draw_rect(SDL_Surface *dst, int x, int y, int w, int h, int color)
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;
    offset.w = w;
    offset.h = h;

    SDL_FillRect(dst, &offset, SDL_MapRGB(dst->format, (char)(color >> 16), (char)(color >> 8), (char)color));
}

void draw_box(SDL_Surface *dst, int x, int y, int w, int h, int border, int b_color, int color)
{
    draw_rect(dst, x, y, w, h, b_color);
    draw_rect(dst, x + border, y + border, w - 2 * border, h - 2 * border, color);
}

void draw_circle(SDL_Surface *dst, int x, int y, int r, int color)
{
    int i, j;

    for (i = x - r; i <= x + r; i++)
    {
        for (j = y - r; j <= y + r; j++) 
        {
            if ((i - x) * (i - x) + (j - y) * (j - y) <= r * r)
            {
                draw_pixel(dst, i, j, color);            
            }
        }
    }
}


SDL_Surface *pic_load(const char *file)
{
    SDL_Surface *img = NULL, *op = NULL;

    op = IMG_Load(file);
    if (op != NULL)
    {
        img = SDL_DisplayFormat(op); 
        SDL_FreeSurface(op);
    }
    return img;
}

void show_txt(TTF_Font *font, const char *text, int color, SDL_Surface *dst, int x, int flagx, int y, int flagy)
{
    SDL_Color c; 
    SDL_Surface *txt = NULL;
    SDL_Rect offset;

    c.r = (char)(color >> 16);
    c.g = (char)(color >> 8);
    c.b = (char)color;

    txt = TTF_RenderUTF8_Blended(font, text, c);

    if (flagx)
        offset.x = (x - txt->w) / 2;
    else
        offset.x = x;
    if (flagy)
        offset.y = (y - txt->h) / 2;
    else
        offset.y = y;

    SDL_BlitSurface(txt, NULL, dst, &offset);

    SDL_FreeSurface(txt);
}

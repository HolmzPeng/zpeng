#ifndef __KYO_VIEWS_H__
#define __KYO_VIEWS_H__

#include <sdl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <unistd.h>

struct global_t {
    struct sdl_t *sdl;
};

#define WIDTH       480
#define HEIGHT      272
#define BPP         32

#define LIST_X      134
#define LIST_Y      0
#define LIST_H      34
#define MES_X       2
#define MES_Y       120 

#define LIST_MAX    8

typedef enum {
    UNKNOWN,
    EBOOK,
    PICS,
    MUSIC,
    VIDEO,
    GAME,
    KDIR
}fileType;

#define TYPEMAX             7
#define PATHMAX             1048576

struct res_t {
    SDL_Surface *big[TYPEMAX];
    SDL_Surface *small[TYPEMAX];
    SDL_Surface *bg;
    SDL_Surface *ufcs;
    SDL_Surface *fcs;
    TTF_Font *font12;
    TTF_Font *font9;
};

struct node_t {
    char *path;
    char *filename;    
    fileType type;
    struct stat mes;
    SDL_Surface *txt;
    struct node_t *next;
    struct node_t *prev;
};

struct views_t {
    struct node_t list;     //保存数据列表
    int list_num;           //数据列表个数
    int filter;             //过滤器 0 全部 1 电子书 ...
    int load_mode;          //加载模式 0 有目录架构 1 无目录架构
    struct node_t *cur;     //保存光标选中文件
    int page_count;         //本屏计数器
    SDL_Thread *th;         //按键处理线程标识
    int thread_quit;        //线程退出标识
    int key_up;             //上键标识
    int key_down;           //下键标识
    struct node_t *start;   //保存列表显示的起始文件
    char *rootpath;         //根目录 起始目录
    char curpath[PATHMAX];  //当前绝对路径
    struct res_t res;       //图片字体资源
    SDL_Color fontcolor[TYPEMAX];
    struct sdl_t *sdl;      
    SDL_Surface *save_screen;
    char tmpbuf[PATHMAX];
};

extern const char *typesuffix[];
extern const char *resbmp[];
extern const char *resttf[];

struct views_t *views_init(const char *root, int filter, struct global_t *global);

void views_exit(struct views_t *);

char *views_select(struct views_t *);

void views_run(struct views_t *);

int res_load(struct res_t *);
void res_destroy(struct res_t *);

int list_load(struct views_t *views);
void list_destroy(struct views_t *views);

void show_views(struct views_t *views);

void views_save(struct views_t *views);
void views_load(struct views_t *views);

#endif /* __KYO_VIEWS_H__ */

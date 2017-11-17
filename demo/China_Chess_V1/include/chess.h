#ifndef __CHESS_H_
#define __CHESS_H_

/*
 *
 * 游戏操作
 * 游戏逻辑
 *
 *
 * 最终目标：ARM板运行的网络对战中国象棋（重视可移植和可扩展）
 *      可移植要求：遵守C89标准
 *      可扩展要求：支持跟电脑对战，悔棋，记分，背景音乐，音效，可观战，支持聊天
 *
 * 1阶段：电脑单机手动对战
 *      kyo:
 *      fyu:
 *
 * 2阶段：电脑网络对战
 *
 * 3阶段：ARM板网络对战
 *
 * 4阶段：扩展功能
 *
 *
 */


typedef int BOOL;
typedef int INT;
typedef char CHAR;

#define ROW     10
#define COL     9

#define NUL 0
#define SUCCESS 1
#define GAME_OVER 2



enum chess_name{
    G_MARSHAL = 1,
    G_SERGEANCY,
    G_ELEPHANT,
    G_HORSE,
    G_GHARRY,
    G_CANNON,
    G_SOLDIER,

    R_MARSHAL,
    R_SERGEANCY,
    R_ELEPHANT,
    R_HORSE,
    R_GHARRY,
    R_CANNON,
    R_SOLDIER,
};

struct pos_t{
    int x;
    int y;
};


struct st_chess{
    enum chess_name name;
    struct pos_t src_pos;
    struct pos_t dst_pos;
};


extern INT chessboard[ROW][COL];

extern INT analysis(struct st_chess *);

#endif /* __CHESS_H_ */

#!/usr/bin/env python3

from gui import *

def checkWin(chess, sx, sy):
    """
    五子棋输赢判断算法
    """
    old = -1
    y = chess['r'] - sy * 4
    x = chess['c'] - sx * 4

    for i in range(9):
        if (not (y < 0 or x < 0
                       or y >= chess['ui']['row']
                       or x >= chess['ui']['col'])):
            if old == chess['board'][y][x]:
                count += 1
            else:
                count = 1
                old = chess['board'][y][x]

            if old and count == 5:
                return True
        y += sy
        x += sx

    return False

def winHandle(chess):
    showChess(chess)
    if chess['turn']:
        print("黑方羸!!!")
    else:
        print("白方羸!!!")

def tieHandle(chess):
    showChess(chess)
    print("和棋!!!!")

def chessHandle(chess):
    if chess['board'][chess['r']][chess['c']]:
        return

    chess['board'][chess['r']][chess['c']] = int(chess['turn']) + 1

    #  输赢判断
    if (checkWin(chess, 1, 0) or checkWin(chess, 0, 1)
         or checkWin(chess, 1, 1) or checkWin(chess, 1, -1)):
        return winHandle(chess)

    #  和棋判断
    for row in chess['board']:
        if 0 in row:
            chess['turn'] = not chess['turn']
            return showChess(chess)

    return tieHandle(chess)

def keyHandle(key, chess):
    if key == K_UP and chess['r'] > 0:
        chess['r'] -= 1
    elif key == K_DOWN and chess['r'] < chess['ui']['row'] - 1:
        chess['r'] += 1
    elif key == K_LEFT and chess['c'] > 0:
        chess['c'] -= 1
    elif key == K_RIGHT and chess['c'] < chess['ui']['col'] - 1:
        chess['c'] += 1
    elif key == K_SPACE:
        return chessHandle(chess)

    showChess(chess)

def virtToPhy(button, pos, chess):
    ui = chess['ui']
    ux, uy = pos
    vx = (ux - (ui['board'][0] - ui['r'])) // ui['cell']
    vy = (uy - (ui['board'][1] - ui['r'])) // ui['cell']

    x = vx * ui['cell'] + ui['board'][0]
    y = vy * ui['cell'] + ui['board'][1]

    if (x - ui['r'] <= ux <= x + ui['r']
            and y - ui['r'] <= uy <= y + ui['r']):
        chess['r'] = vy
        chess['c'] = vx
        return chessHandle(chess)

def chessWinInit(chess):
    ui = chess['ui']

    #  创建五子棋窗口
    w = win.create(w=ui['width'], h=ui['height'], color=0xE98200)
    ui['screen'] = win.screen()

    #  注册键盘事件
    win.key(w, (K_UP, K_DOWN, K_LEFT, K_RIGHT, K_SPACE), keyHandle, chess)

    #  注册鼠标事件
    boardRect = (ui['board'][0] - ui['r'], ui['board'][1] - ui['r'],
                 ui['board'][2] + 2 * ui['r'], ui['board'][3] + 2 * ui['r'])
    win.mouse(w, virtToPhy, boardRect, chess)

    return w

def chessInit(ui):
    chess = {}
    chess['ui'] = ui
    chess['board'] = [[0] * ui['col'] for x in range(ui['row'])]
    chess['r'] = ui['row'] // 2
    chess['c'] = ui['col'] // 2
    chess['turn'] = True

    chess['win'] = chessWinInit(chess)

    showChess(chess)

    return chess

if __name__ == "__main__":
    chess = chessInit(getUi(11, 11, 20))
    win.loop(chess['win'])

#!/usr/bin/env python3

import win
from pygame import *

def getUi(row=9, col=11, r=18):
    """
    根据棋盘容量自动调整棋盘GUI数据
    """
    ui = {}
    ui['r'] = r
    ui['row'] = row
    ui['col'] = col
    ui['color'] = {}
    ui['color']['white'] = win.toColor(0xFFFFFF)
    ui['color']['black'] = win.toColor(0x0)
    ui['color']['line'] = win.toColor(0x0)
    ui['cell'] = r * 2 + 2
    ui['board'] = (ui['cell'], ui['cell'],
                    (col - 1) * ui['cell'], (row - 1) * ui['cell'])
    ui['info'] = ui['cell'] * 2
    ui['width'] = ui['board'][2] + 2 * ui['cell'] + ui['info']
    ui['height'] = ui['board'][3] + 2 * ui['cell']
    ui['screen'] = None

    return ui

def drawBoard(ui):
    """
    绘制棋盘
    """
    #  获取屏幕表面对象
    screen = win.screen() if ui['screen'] == None else ui['screen']

    #  执行第一次保存绘制图像, 再次绘制时直接贴图
    if 'save' in ui and ui['save']:
        screen.blit(ui['save'], (0, 0))
        return

    #  展开UI成员变量, 方便使用
    color = ui['color']['line']
    BOARD_X, BOARD_Y, BOARD_W, BOARD_H = ui['board']
    ROW, COL, CELL = ui['row'], ui['col'], ui['cell']

    #  绘制棋盘外框
    draw.rect(screen, color, ui['board'], 1)
    #  定义棋盘外框边框宽度像素
    BOARD_SPACE = 3
    #  绘制棋盘双线效果外框
    draw.rect(screen, color,
                (BOARD_X - BOARD_SPACE, BOARD_Y - BOARD_SPACE,
                 BOARD_W + BOARD_SPACE * 2,
                 BOARD_H + BOARD_SPACE * 2), 1)

    #  绘制棋盘竖线
    for i in range(1, ROW - 1):
        s = BOARD_Y + i * CELL
        draw.line(screen, color, (BOARD_X, s), (BOARD_X + BOARD_W, s))

    #  绘制棋盘横线
    for i in range(1, COL - 1):
        s = BOARD_X + i * CELL
        draw.line(screen, color, (s, BOARD_Y), (s, BOARD_Y + BOARD_H))

    #  取棋子半径5/1长度作为棋盘定点大小, 最小为2个像素
    r = ui['r'] // 5
    r = 2 if r <= 2 else r

    #  绘制5个棋盘定点
    draw.circle(screen, color,
                (BOARD_X + 2 * CELL, BOARD_Y + 2 * CELL), r)
    draw.circle(screen, color, (BOARD_X + (COL - 3) * CELL,
                                    BOARD_Y + (ROW - 3) * CELL), r)
    draw.circle(screen, color, (BOARD_X + (COL // 2) * CELL,
                                    BOARD_Y + (ROW // 2) * CELL), r)
    draw.circle(screen, color, (BOARD_X + 2 * CELL,
                                    BOARD_Y + (ROW - 3) * CELL), r)
    draw.circle(screen, color, (BOARD_X + (COL - 3) * CELL,
                                    BOARD_Y + 2 * CELL), r)

    #  保存绘制棋盘表面对象, 为了下次不再绘制,直接贴图
    ui['save'] = screen.copy()

def drawCursor(r, c, ui):
    """
    绘制当前选中光标
    """
    #  获取屏幕表面对象
    screen = win.screen() if ui['screen'] == None else ui['screen']

    x = c * ui['cell'] + ui['board'][1]
    y = r * ui['cell'] + ui['board'][0]
    color = ui['color']['line']
    l = ui['r'] // 3

    #  |-
    draw.line(screen, color, (x - ui['r'], y - ui['r']),
                             (x - ui['r'], y - ui['r'] + l))
    draw.line(screen, color, (x - ui['r'], y - ui['r']),
                             (x - ui['r'] + l, y - ui['r']))
    #  |_
    draw.line(screen, color, (x - ui['r'], y + ui['r'] - l),
                             (x - ui['r'], y + ui['r']))
    draw.line(screen, color, (x - ui['r'], y + ui['r']),
                             (x - ui['r'] + l, y + ui['r']))
    #  -|
    draw.line(screen, color, (x + ui['r'] - l, y - ui['r']),
                             (x + ui['r'], y - ui['r']))
    draw.line(screen, color, (x + ui['r'], y - ui['r'] + l),
                             (x + ui['r'], y - ui['r']))
    #  _|
    draw.line(screen, color, (x + ui['r'] - l, y + ui['r']),
                             (x + ui['r'], y + ui['r']))
    draw.line(screen, color, (x + ui['r'], y + ui['r'] - l),
                             (x + ui['r'], y + ui['r']))

def drawChess(r, c, ui, color):
    """
    绘制棋子
    """
    if not color:
        return

    #  获取屏幕表面对象
    screen = win.screen() if ui['screen'] == None else ui['screen']

    x = c * ui['cell'] + ui['board'][1]
    y = r * ui['cell'] + ui['board'][0]
    draw.circle(screen, ui['color'][color], (x, y), ui['r'])

def drawInfo(ui, color):
    """
    绘制右边提示信息
    """
    x = (ui['col'] + 1) * ui['cell'] + ui['r']
    y = ui['height'] // 2
    draw.circle(ui['screen'], ui['color'][color], (x, y), ui['r'])

def showChess(chess):
    """
    显示刷新棋盘
    """
    chessColor = (None, 'white', 'black')
    ui = chess['ui']
    board = chess['board']

    drawBoard(ui)

    for i in range(ui['row']):
        for j in range(ui['col']):
            if chess['r'] == i and chess['c'] == j:
                drawCursor(i, j, ui)
            drawChess(i, j, ui, chessColor[board[i][j]])

    drawInfo(ui, chessColor[chess['turn'] + 1])

    win.flip()



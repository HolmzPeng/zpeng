#!/usr/bin/env python3

import gt
import sys
import os
from random import randint

def createBall(win, r=None, c=None, r_inc=None, c_inc=None,
                color=None, ch=None):
    """
    创建一个弹球对象并且初始化数据
    """
    ball = {}
    ball['r'] = r if r is not None else randint(2, win['height'] - 1)
    ball['c'] = c if c is not None else randint(2, win['width'] - 1)
    ball['r_inc'] = r_inc if r_inc is not None else (-1, 1)[randint(0, 1)]
    ball['c_inc'] = c_inc if c_inc is not None else (-1, 1)[randint(0, 1)]
    ball['color'] = color if color is not None else randint(30, 37)
    ball['ch'] = ch if ch is not None else "@*$%ABCDEF"[randint(0, 9)]

    return ball


def addBall(win, num=1, balls=None):
    """
    添加多个球, 返回保存所有球数据的列表
    """
    if balls is None:
        balls = []

    for i in range(num):
        balls.append(createBall(win))

    return balls


def addObstacles(win, num=1, obstacles=None):
    """
    添加多个障碍物对象及初始化数据
    """
    if obstacles is None:
        obstacles = []

    for i in range(num):
        obs = {}
        obs['r'] = randint(2, win['height'] - 1)
        obs['c'] = randint(2, win['width'] - 1)
        obs['color'] = randint(30, 37)
        obs['ch'] = '#'
        obs['count'] = 0
        obs['total'] = randint(10, 50)

        obstacles.append(obs)

    return obstacles


def ballExit(win):
    """
    弹球程序退出处理函数
    """
    gt.exit()
    print("\033[?25h\033[%d;1H" % (win['height'] + 1))
    os._exit(0)


def ballInit(w=30, h=10, ballNum=1, obsNum=1):
    """
    弹球程序的初始化
    """
    gt.init()
    print("\033[?25l\033[2J\033[1;1H", end="", flush=True)

    ballPro = {}
    ballPro['win'] = {'width': w, 'height': h}
    ballPro['balls'] = addBall(ballPro['win'], ballNum)
    ballPro['obstacles'] = addObstacles(ballPro['win'], obsNum)

    return ballPro


def showBg(width, height):
    """
    输出弹球程序画布
    """
    print("\033[1;1H", end="", flush=True)
    for i in range(height):
        for j in range(width):
            if i == 0 or j == 0 or i == height - 1 or j == width - 1:
                print("#", end="")
            else:
                print(" ", end="")
        print()


def showObj(objs):
    """
    打印障碍物或弹球
    """
    for o in objs:
        print("\033[%d;%dH\033[%dm%s\033[0m"
               % (o['r'], o['c'], o['color'], o['ch']), end="", flush=True)


def moveBall(win, ball):
    """
    移动弹球
    """
    if (ball['r'] + ball['r_inc'] < 1
            or ball['r'] + ball['r_inc'] > win['height']):
        ball['r_inc'] = -ball['r_inc']

    if (ball['c'] + ball['c_inc'] < 1
            or ball['c'] + ball['c_inc'] > win['width']):
        ball['c_inc'] = -ball['c_inc']

    ball['r'] += ball['r_inc']
    ball['c'] += ball['c_inc']


def ballRun():
    """
    弹球程序动画函数
    """
    global ballPro

    while True:
        showBg(**ballPro['win'])
        showObj(ballPro['obstacles'])
        showObj(ballPro['balls'])

        for ball in ballPro['balls']:
            moveBall(ballPro['win'], ball)

        gt.sleep(0.1)


def ballStart(ballPro):
    """
    弹球程序开始启动
    """
    gt.run(ballRun)

    while True:
        ch = gt.get()
        if ch == 'q':
            break

    ballExit(ballPro['win'])


if __name__ == "__main__":
    #  print(sys.argv)
    w = int(sys.argv[1]) if len(sys.argv) > 1 else 20
    h = int(sys.argv[2]) if len(sys.argv) > 2 else 10
    ballNum = int(sys.argv[3]) if len(sys.argv) > 3 else 1
    obsNum = int(sys.argv[4]) if len(sys.argv) > 4 else 1

    ballPro = ballInit(w, h, ballNum, obsNum)
    ballStart(ballPro)


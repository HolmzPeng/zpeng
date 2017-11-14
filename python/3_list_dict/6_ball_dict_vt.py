#!/usr/bin/env python3

import gt
import sys
import os
import vt
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
    ball['color'] = color if color is not None else vt.getColor()
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

def randObstacle(win, obs=None):
    """
    随机一个障碍物
    """
    if obs is None:
        obs = {}
    obs['r'] = randint(2, win['height'] - 1)
    obs['c'] = randint(2, win['width'] - 1)
    obs['color'] = vt.getColor()
    obs['ch'] = '#'
    obs['count'] = 0
    obs['total'] = randint(10, 50)

    return obs

def addObstacles(win, num=1, obstacles=None):
    """
    添加多个障碍物对象及初始化数据
    """
    if obstacles is None:
        obstacles = []

    for i in range(num):
        obstacles.append(randObstacle(win))

    return obstacles


def ballExit(win):
    """
    弹球程序退出处理函数
    """
    gt.exit()
    vt.run([vt.SHOW, vt.goto(win['height'] + 1)])
    os._exit(0)


def ballInit(w=30, h=10, ballNum=1, obsNum=1):
    """
    弹球程序的初始化
    """
    gt.init()
    vt.run([vt.HIDE, vt.SCREEN, vt.goto()])

    ballPro = {}
    ballPro['pause'] = False
    ballPro['win'] = {'width': w, 'height': h}
    ballPro['balls'] = addBall(ballPro['win'], ballNum)
    ballPro['obstacles'] = addObstacles(ballPro['win'], obsNum)

    return ballPro


def showBg(width, height):
    """
    输出弹球程序画布
    """
    vt.goto(back=False)
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
        vt.run([vt.goto(o['r'], o['c']), vt.color(o['ch'], o['color'])])


def moveBall(win, ball, obstacles):
    """
    移动弹球
    """
    if (ball['r'] + ball['r_inc'] < 1
            or ball['r'] + ball['r_inc'] > win['height']):
        ball['r_inc'] = -ball['r_inc']

    if (ball['c'] + ball['c_inc'] < 1
            or ball['c'] + ball['c_inc'] > win['width']):
        ball['c_inc'] = -ball['c_inc']

    for o in obstacles:
        if ball['r'] == o['r'] and ball['c'] == o['c']:
            ball['r_inc'] = -ball['r_inc']
            ball['c_inc'] = -ball['c_inc']
            #  ballExit(win)

    ball['r'] += ball['r_inc']
    ball['c'] += ball['c_inc']


def moveObstacles(win, obs):
    """
    障碍物随机
    """
    obs['count'] += 1
    if obs['count'] == obs['total']:
        randObstacle(win, obs)


def ballRun(ballPro):
    """
    弹球程序动画函数
    """
    while True:
        if not ballPro['pause']:
            showBg(**ballPro['win'])
            showObj(ballPro['obstacles'])
            showObj(ballPro['balls'])

            for obs in ballPro['obstacles']:
                moveObstacles(ballPro['win'], obs)

            for ball in ballPro['balls']:
                moveBall(ballPro['win'], ball, ballPro['obstacles'])

        gt.sleep(0.1)


def ballStart(ballPro):
    """
    弹球程序开始启动
    """
    gt.run(ballRun, args=(ballPro, ))

    while True:
        ch = gt.get()
        if ch == 'q':
            break
        elif ch == chr(10):
            ballPro['pause'] = not ballPro['pause']
        elif ch == ' ':
            for b in ballPro['balls']:
                b['color'] = vt.getColor()
        elif ch == 'w':
            for b in ballPro['balls']:
                b['r_inc'] = -1
        elif ch == 's':
            for b in ballPro['balls']:
                b['r_inc'] = 1
        elif ch == 'a':
            for b in ballPro['balls']:
                b['c_inc'] = -1
        elif ch == 'd':
            for b in ballPro['balls']:
                b['c_inc'] = 1
        elif ch == 'g':
            for b in ballPro['balls']:
                b['r_inc'] = [-1, 1][randint(0, 1)]
                b['c_inc'] = [-1, 1][randint(0, 1)]

    ballExit(ballPro['win'])


if __name__ == "__main__":
    #  print(sys.argv)
    w = int(sys.argv[1]) if len(sys.argv) > 1 else 20
    h = int(sys.argv[2]) if len(sys.argv) > 2 else 10
    ballNum = int(sys.argv[3]) if len(sys.argv) > 3 else 1
    obsNum = int(sys.argv[4]) if len(sys.argv) > 4 else 1

    ballPro = ballInit(w, h, ballNum, obsNum)
    ballStart(ballPro)


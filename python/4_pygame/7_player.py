#!/usr/bin/env python3

import gt
import sys
import os
import win as gui
import pygame
from pygame.draw import ellipse
from random import randint

def createBall(win, r=None, c=None, w=None, h=None, r_inc=None, c_inc=None,
                color=None, ch=None):
    """
    创建一个弹球对象并且初始化数据
    """
    ball = {}
    ball['r'] = r if r is not None else randint(1, win['height'] - 2)
    ball['c'] = c if c is not None else randint(1, win['width'] - 2)
    ball['w'] = w if w is not None else randint(1, 5)
    ball['h'] = h if h is not None else randint(1, 5)
    ball['r_inc'] = r_inc if r_inc is not None else (-1, 1)[randint(0, 1)]
    ball['c_inc'] = c_inc if c_inc is not None else (-1, 1)[randint(0, 1)]
    ball['color'] = color if color is not None else gui.getColor()
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
    obs['r'] = randint(1, win['height'] - 2)
    obs['c'] = randint(1, win['width'] - 2)
    obs['w'] = randint(1, 5)
    obs['h'] = randint(1, 5)
    obs['color'] = gui.getColor()
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


def ballExit(ballPro):
    """
    弹球程序退出处理函数
    """
    ballPro['quit'] = True
    ballPro['thread'].join()
    os._exit(0)


def ballInit(w=30, h=10, ballNum=1, px=10, obsNum=1):
    """
    弹球程序的初始化
    """

    win = {}
    win['width'] = w
    win['height'] = h
    win['convas'] = [[' '] * w for x in range(h)]

    ballPro = {}
    ballPro['px'] = px
    ballPro['quit'] = False
    ballPro['pause'] = False
    ballPro['win'] = win
    ballPro['balls'] = addBall(ballPro['win'], ballNum)
    ballPro['obstacles'] = addObstacles(ballPro['win'], obsNum)

    ballPro['gui'] = gui.create(w=w * px, h=h * px)
    ballPro['bg'] = pygame.image.load("./backGround.bmp")
    ballPro['bg'] = pygame.transform.scale(ballPro['bg'], (w * px, h * px))
    ballPro['img'] = pygame.image.load("./player.bmp")
    ballPro['img'].set_colorkey((247, 0, 255))

    gui.key(ballPro['gui'],
                [pygame.K_RETURN, pygame.K_SPACE, pygame.K_w,
                 pygame.K_s, pygame.K_a, pygame.K_d, pygame.K_g,
                 pygame.K_q],
                keyHandle,
                ballPro)

    return ballPro


def convasInit(width, height, convas):
    """
    画布初始化
    """
    for i in range(height):
        for j in range(width):
            if i == 0 or j == 0 or i == height - 1 or j == width - 1:
                convas[i][j] = '#'
            else:
                convas[i][j] = ' '

    #  showBall(convas)   #测试使用


def drawObj(win, objs):
    """
    画障碍物或弹球
    """
    def _drawObj(win, o):
        convas = win['convas']

        for i in range(o['h']):
            for j in range(o['w']):
                r, c = o['r'] + i, o['c'] + j

                if (r < 0 or r > win['height'] - 1
                        or c < 0 or c > win['width'] - 1):
                    continue

                convas[r][c] = o

    for o in objs:
        _drawObj(win, o)


def moveBall(win, ball, obstacles):
    """
    移动弹球
    """
    if (ball['r'] + ball['r_inc'] < 0
            or ball['r'] + ball['r_inc'] + ball['h'] >= win['height']):
        ball['r_inc'] = -ball['r_inc']

    if (ball['c'] + ball['c_inc'] < 0
            or ball['c'] + ball['c_inc'] + ball['w'] >= win['width']):
        ball['c_inc'] = -ball['c_inc']

    for o in obstacles:
        if (o['r'] <= ball['r'] <= o['r'] + o['h']
                and o['c'] <= ball['c'] <= o['c'] + o['w']):
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


def getGuiRect(o, px):
    """
    转换坐标
    """
    #  print(o)
    return o['c'] * px, o['r'] * px, o['w'] * px, o['h'] * px


def showBall(ballPro):
    """
    刷新画布数据到屏幕
    """
    convas = ballPro['win']['convas']
    screen = gui.screen()
    y = dict(zip("@*$%ABCDEF", [48 * x for x in range(11)]))

    screen.blit(ballPro['bg'], (0, 0))
    for r in convas:
        for o in r:
            if type(o) == dict:
                #  ellipse(screen, o['color'], getGuiRect(o, ballPro['px']))
                screen.blit(ballPro['img'],
                                getGuiRect(o, ballPro['px']),
                                (0, y[o['ch']], 48, 48))

    gui.flip()


def ballRun(ballPro):
    """
    弹球程序动画函数
    """
    while not ballPro['quit']:
        if not ballPro['pause']:
            convasInit(**ballPro['win'])
            drawObj(ballPro['win'], ballPro['obstacles'])
            drawObj(ballPro['win'], ballPro['balls'])

            #  for obs in ballPro['obstacles']:
                #  moveObstacles(ballPro['win'], obs)

            for ball in ballPro['balls']:
                moveBall(ballPro['win'], ball, ballPro['obstacles'])

            try:
                showBall(ballPro)
            except:
                pass

        gt.sleep(0.1)


def keyHandle(key, ballPro):
    if key == pygame.K_q:
        return True
    elif key == pygame.K_RETURN:
        ballPro['pause'] = not ballPro['pause']
    elif key == pygame.K_SPACE:
        for b in ballPro['balls']:
            b['color'] = gui.getColor()
    elif key == pygame.K_w:
        for b in ballPro['balls']:
            b['r_inc'] = -1
    elif key == pygame.K_s:
        for b in ballPro['balls']:
            b['r_inc'] = 1
    elif key == pygame.K_a:
        for b in ballPro['balls']:
            b['c_inc'] = -1
    elif key == pygame.K_d:
        for b in ballPro['balls']:
            b['c_inc'] = 1
    elif key == pygame.K_g:
        for b in ballPro['balls']:
            b['r_inc'] = [-1, 1][randint(0, 1)]
            b['c_inc'] = [-1, 1][randint(0, 1)]


def ballStart(ballPro):
    """
    弹球程序开始启动
    """
    ballPro['thread'] = gt.run(ballRun, args=(ballPro, ))

    gui.loop(ballPro['gui'])

    ballExit(ballPro)


if __name__ == "__main__":
    #  print(sys.argv)
    w = int(sys.argv[1]) if len(sys.argv) > 1 else 20
    h = int(sys.argv[2]) if len(sys.argv) > 2 else 10
    ballNum = int(sys.argv[3]) if len(sys.argv) > 3 else 1
    px = int(sys.argv[4]) if len(sys.argv) > 4 else 10
    obsNum = int(sys.argv[5]) if len(sys.argv) > 5 else 1

    ballPro = ballInit(w, h, ballNum, px, obsNum)
    ballStart(ballPro)


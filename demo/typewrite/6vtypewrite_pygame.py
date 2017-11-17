#!/usr/bin/env python3

import random
import os
import pygame
import sys
import gt

'''
全局变量
'''
X = 600
Y = 600
x, y, color , c = None, None, None, None
R = 0
N = 0



def word_init():
    '''
    初始化窗口
    '''
    pygame.init()


def word_exit():
    '''
    结束离开还原
    '''
    os._exit(0)
    pygame.exit()


def manyword():
    '''
    创建所有单词光标的属性
    '''
    global x, y, color, c
    x = list(range(wordNum))
    y = list(range(wordNum))
    color = list(range(wordNum))
    c = list(range(wordNum))
    for i in range(wordNum):
        x[i] = random.randint(5, 595)
        y[i] = 0
        color[i] = (random.randint(5, 255), random.randint(5, 255),
        random.randint(5, 255))
        c[i] = random.choice("ABCDEFGHIJKLMNOPQRSTUVWXYZ")


def workmove(i):
    '''
    一个单词的移动
    '''
    global x, y, color, c, N
    my_font = pygame.font.SysFont("arplumingtw", 40)
    text = c[i]
    font_surface = my_font.render(text, True, color[i])
    screen.blit(font_surface,(x[i], y[i]))

    if y[i] == 595:
        y[i] = 0
        x[i] = random.randint(5, 595)
        c[i] = random.choice('ABCDEFGHIJKLMNOPQRSTUVWSYZ')
        color[i] = (random.randint(5, 255), random.randint(5, 255),
        random.randint(5, 255))
        N += 1
    y[i] += 10


def manyWorkmove():
    '''
    全部单词的移动
    '''
    for i in range(wordNum):
        workmove(i)
    pygame.display.update()


def wordDoing():
    '''
    单词移动的刷新过程
    '''
    background = pygame.image.load('1.jpeg')
    screen.blit(background, (0, 0))
    #  screen.fill((0, 0, 0))
    Rwshow()
    manyWorkmove()
    gt.run(wordDoing, 0.1,)


def wordcontrol(ch, a):
    '''
    键盘输入26个字母后处理的规则
    '''
    global x, y, c, color, R, N

    for i, o in enumerate(ch):
        P = False
        if a == o:
            c[i] = random.choice("ABCDEFGHIJKLMNOPQRSTUVWSYZ")
            y[i] = 5
            color[i] = (random.randint(100, 255),
                    random.randint(100, 255),
                    random.randint(100, 255))
            x[i] = random.randint(5, 595)
            R += 1
            P = True
            break
    if not P:
        N += 1


def control():
    '''
    键盘输入控制
    '''
    global x, y, c, color, R, N
    bom = {'A': pygame.K_a, 'B':pygame.K_b, 'C':pygame.K_c, 'D':pygame.K_d,
            'E':pygame.K_e, 'F':pygame.K_f, 'G':pygame.K_g, 'H':pygame.K_h,
            'I':pygame.K_i, 'J':pygame.K_j, 'K':pygame.K_k, 'L':pygame.K_l,
            'M':pygame.K_m, 'N':pygame.K_n, 'O':pygame.K_o, 'P':pygame.K_p,
            'Q':pygame.K_q, 'R':pygame.K_r, 'S':pygame.K_s, 'T':pygame.K_t,
            'U':pygame.K_u, 'V':pygame.K_v, 'W':pygame.K_w, 'X':pygame.K_x,
            'Y':pygame.K_y, 'Z':pygame.K_z}
    '''
    定义输入的光标的值和对应的输入按钮的关系
    '''

    while True:
        e = pygame.event.wait()
        if e.type == pygame.QUIT:
            break
        elif e.type == pygame.KEYDOWN:
            if e.key == pygame.K_ESCAPE:
                break
            for a, k in bom.items():
                if e.key == k:
                    wordcontrol(c, a)

                    #  for i, o in enumerate(c):
                        #  P = False
                        #  if a == o:
                            #  c[i] = random.choice("ABCDEFGHIJKLMNOPQRSTUVWSYZ")
                            #  y[i] = 5
                            #  color[i] = (random.randint(100, 255),
                                    #  random.randint(100, 255),
                                    #  random.randint(100, 255))
                            #  x[i] = random.randint(5, 595)
                            #  R += 1
                            #  P = True
                            #  break
                    #  if not P:
                        #  N += 1


def Rwshow():
    '''
    正确和错误次数的显示
    '''
    my_font = pygame.font.SysFont("arplumingtw", 16)
    text = "正确的次数是:" + str(R)
    font_surface = my_font.render(text, True, (0, 0, 255))
    screen.blit(font_surface,(10, 30))

    my_font = pygame.font.SysFont("arplumingtw", 16)
    text = "错误的次数是:" + str(N)
    font_surface = my_font.render(text, True, (255, 0, 0))
    screen.blit(font_surface,(10, 60))



if __name__ == '__main__':

    wordNum = 3 if len(sys.argv) < 2 else int(sys.argv[1])
    word_init()
    screen = pygame.display.set_mode((X, Y))
    manyword()
    gt.run(wordDoing, 0.1)
    control()
    word_exit()





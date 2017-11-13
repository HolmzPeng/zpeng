#!/usr/bin/env python3

import pygame
from pygame.locals import *

def toColor(color):
    """
    十六进制转换成元组颜色
    """
    if type(color) == tuple:
        return color
    return color >> 16 & 0xFF, color >> 8 & 0xFF, color & 0xFF

def create(**karg):
    """
    创建窗口
    """
    pygame.init()
    pg = {'w': 640, 'h': 480, 'color': (0, 0, 0)}
    pg['quitCall'] = None
    pg['quitCallData'] = None
    pg['quitKey'] = K_ESCAPE
    pg.update(karg)

    pg['key'] = {}
    pg['mouse'] = []
    pg['screen'] = pygame.display.set_mode((pg['w'], pg['h']))
    pg['screen'].fill(pg['color'])

    return pg

def screen():
    """
    获取Surface对象
    """
    return pygame.display.get_surface()

def flip():
    """
    刷新屏幕
    """
    pygame.display.update()

def set(pg, name, value):
    """
    设置属性
    """
    pg[name] = value

def get(pg, name):
    """
    获取属性
    """
    return pg[name] if name in pg else None

def destroy(pg):
    """
    销毁窗口
    """
    if callable(pg['quitCall']):
        pg['quitCall'](pg['quitCallData'])
    pygame.quit()

def key(pg, keys, call, data=None):
    """
    注册按键
    """
    if type(keys) == tuple or type(keys) == list:
        for k in keys:
            key(pg, k, call, data)
        return
    pg['key'][keys] = {}
    pg['key'][keys]['call'] = call
    pg['key'][keys]['data'] = data

def mouse(pg, call, rect=None, data=None, button=1):
    """
    注册鼠标
    """
    m = {}
    m['button'] = button
    if not rect:
        rect = (0, 0, pg['w'], pg['h'])
    m['rect'] = rect
    m['call'] = call
    m['data'] = data
    pg['mouse'].append(m)

def inRect(pos, rect):
    """
    判断一个坐标是否在一个范围内
    """
    return (rect[0] <= pos[0] <= rect[0] + rect[2]
                and rect[1] <= pos[1] <= rect[1] + rect[3])

def mouseLoop(pg, e):
    """
    轮询鼠标事件
    """
    for m in pg['mouse']:
        if m['button'] == e.button and inRect(e.pos, m['rect']):
            return m['call'](e.button, e.pos, m['data'])

def keyLoop(pg, e):
    """
    轮询键盘事件
    """
    if e.key == pg['quitKey']:
        return True
    elif e.key in pg['key']:
        return pg['key'][e.key]['call'](e.key, pg['key'][e.key]['data'])

def loop(pg):
    """
    事件轮询
    """
    while True:
        e = pygame.event.wait()
        if e.type == QUIT:
            break
        elif e.type == MOUSEBUTTONDOWN and mouseLoop(pg, e):
            break
        elif e.type == KEYDOWN and keyLoop(pg, e):
            break

    destroy(pg)


if __name__ == "__main__":
    def run(k, data):
        print("Run k = ", k, ", data = ", data)

    pg = create(quitKey=K_q)
    #  set(pg, 'quitKey', K_q)
    key(pg, K_ESCAPE, lambda k, d: True)
    key(pg, (K_SPACE, K_a, K_b), run, [1, 2, 3, 4, 5])
    mouse(pg, lambda b, p, d: True, (0, 0, 100, 100))
    loop(pg)

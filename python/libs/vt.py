#!/usr/bin/env python3

import random

#  VT码命令前缀
PR = '\033['

#  光标操作定义
SAVE, LOAD, HIDE, SHOW = 's', 'u', '?25l', '?25h'

#  清除操作定义
SCREEN, LINE, END = '2J', '2K', 'K'

#  颜色定义
BLACK, RED, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE, DEFAULT = range(9)

#  方向定义
UP, DOWN, LEFT, RIGHT = "ABDC"


def run(cmd, back=True):
    if type(cmd) == list:
        cmd = ["%s%s" % (['', PR][PR not in x], x) for x in cmd]
        print(''.join(cmd), end='', flush=True)
    else:
        if back:
            return PR + cmd
        print("%s%s" % (['', PR][PR not in cmd], cmd), end='', flush=True)

#  绝对定位
def goto(r=1, c=1, back=True):
    return run("%d;%dH" % (r, c), back)

#  相对定位
def move(direction=RIGHT, step=1, back=True):
    return run("%d%s" % (step, direction), back)

def left(step=1, back=True):
    return move(LEFT, step, back)

def right(step=1, back=True):
    return move(RIGHT, step, back)

def up(step=1, back=True):
    return move(UP, step, back)

def down(step=1, back=True):
    return move(DOWN, step, back)

def save(back=True):
    return run(SAVE, back)

def load(back=True):
    return run(LOAD, back)

def hide(back=True):
    return run(HIDE, back)

def show(back=True):
    return run(SHOW, back)

#  清除操作
def clear(back=True):
    return run(SCREEN, back)

def clearLine(back=True):
    return run(LINE, back)

def clearLineEnd(back=True):
    return run(END, back)

#  颜色输出
def setColor(fg=DEFAULT, bg=DEFAULT, bold=False, back=True):
    if fg == DEFAULT and bg == DEFAULT:
        return run("0m", back)
    fgstr = [str(fg + 30)+';', ''][fg == DEFAULT]
    return run("%s%s%dm" % (['', '1;'][bold], fgstr, bg + 40), back)

def color(s, fg, bg=DEFAULT, bold=False, back=True):
    return run("%s%s%s" % (setColor(fg, bg, bold), s, setColor()), back)

def getColor():
    #  return random.randint(BLACK, DEFAULT)
    return random.choice([BLACK, RED, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE])


#  vt测试
if __name__ == "__main__":
    #  clear(False)
    #  goto(back=False)
    #  run(HIDE, False)

    color("hello", DEFAULT, BLACK, back=False)
    color("hello", DEFAULT, BLACK, True, back=False)
    color("hello", RED, back=False)
    color("hello", RED, bold=True, back=False)
    print()
    exit(0)

    run([SCREEN, goto(1, 1), HIDE,
            down(3), right(10), color("0hello world\n", RED)])

    setColor(BLUE, back=False)
    print("1hello world")
    setColor(back=False)
    print("2hello world")
    color("3hello world", YELLOW, BLACK, True, False)
    input()
    run([SHOW])


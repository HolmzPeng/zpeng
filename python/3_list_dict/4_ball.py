#!/usr/bin/env python3

import gt
import os
from random import randint

W, H = 20, 10
r, c = 3, 3
fr, fc, fn, ft = randint(2, H - 1), randint(2, W - 1), 1, randint(10, 50)
r_inc, c_inc = 1, 1
color = 31
quit = False

def ballExit():
    global quit

    quit = True
    gt.exit()
    print("\033[?25h\033[%d;1H" % (H + 2))
    os._exit(0)

def ball():
    global r, c, r_inc, c_inc, color, fc, fr, fn, ft, quit

    while not quit:
        print("\033[1;1H", end="", flush=True)
        for i in range(H):
            for j in range(W):
                if i == 0 or j == 0 or i == H - 1 or j == W - 1:
                    print("#", end="")
                else:
                    print(" ", end="")
            print()

        fn += 1
        #  if fn == ft:
            #  fr, fc, fn, ft = (randint(2, H - 1), randint(2, W - 1),
                                #  1, randint(10, 50))
        print("\033[%d;%dH#" % (fr, fc), end="", flush=True)

        print("\033[%d;%dH\033[%dm@\033[0m" % (r, c, color),
                    end="", flush=True)

        if r == fr and c == fc:
            break

        if r + r_inc < 1 or r + r_inc > H:
            r_inc = -r_inc

        if c + c_inc < 1 or c + c_inc > W:
            c_inc = -c_inc

        #  color = randint(30, 37)

        r += r_inc
        c += c_inc

        gt.sleep(0.1)

    ballExit()


def main():
    global quit, color, r_inc, c_inc

    gt.init()

    print("\033[?25l\033[2J\033[1;1H", end="", flush=True)

    gt.run(ball)

    while not quit:
        ch = gt.get()
        if ch == 'q':
            break
        elif ch == ' ':
            color = randint(30, 37)
        elif ch == 'w':
            r_inc = -1
        elif ch == 's':
            r_inc = 1
        elif ch == 'a':
            c_inc = -1
        elif ch == 'd':
            c_inc = 1

    ballExit()

if __name__ == "__main__":
    main()

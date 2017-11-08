#!/usr/bin/env python3

import gt

W, H = 60, 20
r, c = 3, 3
quit = False

def ball():
    global r, c

    while not quit:
        print("\033[1;1H", end="", flush=True)
        for i in range(H):
            for j in range(W):
                if i == 0 or j == 0 or i == H - 1 or j == W - 1:
                    print("#", end="")
                else:
                    print(" ", end="")
            print()

        print("\033[%d;%dH@" % (r, c), end="", flush=True)

        if r == H:
            r -= 1
        else:
            r += 1

        gt.sleep(0.1)


def main():
    global quit

    gt.init()

    print("\033[?25l\033[2J\033[1;1H", end="", flush=True)

    gt.run(ball)

    while True:
        ch = gt.get()
        if ch == 'q':
            break

    quit = True

    print("\033[?25h")
    gt.exit()

if __name__ == "__main__":
    main()

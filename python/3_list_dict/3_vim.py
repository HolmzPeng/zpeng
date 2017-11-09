#!/usr/bin/env python3

import random
import gt

def main():
    gt.init()

    while True:
        ch = gt.get()

        if ch == 'q' or ch == chr(27):
            break
        elif ch == 'w':
            print("\033[1A", end="", flush=True)
        elif ch == 's':
            print("\033[1B", end="", flush=True)
        elif ch == 'a':
            print("\033[1D", end="", flush=True)
        elif ch == 'd':
            print("\033[1C", end="", flush=True)
        elif ch == ' ':
            print("\033[%dm" % (random.randint(30, 37)), end="", flush=True)
        else:
            print("%s" % (ch), end="")

    gt.exit()


if __name__ == "__main__":
    main()

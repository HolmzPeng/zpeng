#!/usr/bin/env python3

def main():
    print("\033[?25l", end="")
    for i in range(8):
        j = i + 1
        j %= 8
        print("\033[%dmhello\033[0m " % (i + 30), end="")
        print("\033[%d;1mhello\033[0m " % (i + 30), end="")
        print("\033[%dmhello\033[0m " % (i + 40), end="")
        print("\033[1;%d;%dmhello\033[0m " % (i + 40, j + 30), end="")
        print("\033[1;%d;%dmhello\033[0m " % (j + 30, i + 40))

    print("\033[s", end="")

    print("\033[1;1H+++++++++++++++++++++++++++++++")
    print("\033[10;50H+++++++++++++++++++++++++++++++", end="")

    print("\033[1A&&&&&&&&&&&&&&&", end="")
    print("\033[30D%%%%%%%%%%", end="")
    print("\033[3B**********", end="")
    print("\033[u最后打印的内容", end="")
    input()
    print("\033[?25h")

if __name__ == "__main__":
    main()

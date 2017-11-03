#!/usr/bin/env python3

import kyo

def ball():
    """
    一个袋子里有3个红球， 3个绿球，6个黄球， 一次从袋子里取6个球
    列出所有可能的颜色组合
    """
    r = 0
    y = 0
    while r < 4:
        g = 0
        while g < 4:
            y = 6 - r - g
            print("红球: %d, 绿球: %d, 黄球: %d" % (r, g, y))
            g += 1
        r += 1

def mul9x9():
    """
    9x9乘法
    """
    i = 1
    while i < 10:
        j = 1
        while j <= i:
            if i % 2 == 0:
                print("%d X %d = %2d " % (i, j, i * j), end='')
            else:
                print("\033[32;40m%d X %d = %2d\033[0m " % (i, j, i * j), end='')
            j += 1
        print()
        i += 1


def blackFri(year):
    """
    已知1900年一月一号是星期一, 问今年有多少个黑色星期五(黑色星期五每个月13号是星期五)
    """
    days = kyo.countYearDays(1900, year) + 13

    m = 1
    while m <= 12:
        if m == 3:
            days += 28 + kyo.isleap(year)
        elif m == 5 or m == 7 or m == 10 or m == 12:
            days += 30
        elif m != 1:
            days += 31

        if days % 7 == 5:
            print("%d 年 %d 月 13号是黑色星期五!" % (year, m))
        m += 1

ball()
mul9x9()
blackFri(2017)


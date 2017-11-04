#!/usr/bin/env python3

import time

def progress(ms=0.1):
    '''
    实现文本进度条功能
    ms      动画间隔时间，默认为0.1秒
    '''
    i = 0
    while i < 51:
        print("\033[s<", "=" * i, "> ", i * 2, "%\033[u",
              sep='', end='', flush=True)
        i += 1
        time.sleep(ms)
    print()


def waitIco(count=100, ms=0.1):
    """
    实现加载等待特效
    count   旋转次数, 默认值为100次
    ms      动画间隔时间, 默认为0.1秒
    """
    i = 0
    print("\033[s")
    while  i < count:
        print('\033[u-', end='', flush=True)
        time.sleep(ms)
        print('\033[u/', end='', flush=True)
        time.sleep(ms)
        print('\033[u|', end='', flush=True)
        time.sleep(ms)
        print('\033[u\\', end='', flush=True)
        time.sleep(ms)
        i += 1

def isleap(year):
    """
    判断输入年是否闰年
    是闰年返回True, 不是返回False
    """
    return 1 if year % 400 == 0 or year % 4 == 0 and year % 100 != 0 else 0

def countYearDays(sy, ey):
    """
    计算两个年之间天数
    """
    days = 0
    while sy < ey:
        days += 365 + isleap(sy)
        sy += 1

    return days


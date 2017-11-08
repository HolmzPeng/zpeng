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


def countMonthDays(year, month):
    """
    计算1月到指定月的天数
    """
    #  days = (month - 1) * 30  + month // 2
    #  days += 1 if month == 9 or month == 11 else 0
    #  return days - 2 + isleap(year) if month > 2 else days

    M = ((31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31),
         (31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31))[isleap(year)]

    days = 0

    while month > 1:
        days += M[month - 2]
        month -= 1

    return days


def checkDate(year, month=1, day=1):
    """
    检测日期合法性
    checkDate(2017, 1, 1)
    checkDate((2017, 1, 1))
    checkDate([2017, 1, 1])
    checkDate("2017 1 1")
    checkDate(2017)
    checkDate(2017, 12)
    checkDate(2017, day=12)
    """
    if type(year) == tuple or type(year) == list:
        year, month, day = [int(x) for x in year]
    elif type(year) == str:
        year, month, day = [int(x) for x in year.split()]

    #  print(year, month, day)

    if (year < 1900 or year > 2100
            or month < 1 or month > 12
            or day < 1
            or ((month == 2 and day > 28 + isleap(year)) or
                (month in (4, 6, 9, 11) and day > 30) or
                day > 31)):
        return False

    return year, month, day


if __name__ == "__main__":
    print("isleap 2017 = ", isleap(2017))


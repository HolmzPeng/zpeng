#!/usr/bin/env python3

import kyo
import menu


def ball(index, args):
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


def mul9x9(index, args):
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


def blackFri(index, args):
    """
    已知1900年一月一号是星期一
    问今年有多少个黑色星期五(黑色星期五每个月13号是星期五)
    """
    year = int(input("请输入年: "))
    days = kyo.countYearDays(1900, year) + 13

    m = 1
    while m <= 12:
        if (days + kyo.countMonthDays(year, m)) % 7 == 5:
            print("%d 年 %d 月 13号是黑色星期五!" % (year, m))
        m += 1


def fish(index, args):
    """
    某人从2000年一月一号开始过着3天打鱼两天晒网的日子
    输入年月日判断此人在打鱼还是晒网
    """
    def core(year, month, day):
        day += kyo.countYearDays(2000, year) + kyo.countMonthDays(year, month)
        return "打鱼" if 1 <= day % 5 <= 3 else "晒网"

    try:
        print(core(*kyo.checkDate(input("请输入年月日: ").split('-'))))

        #  print(core(*kyo.checkDate(input("请输入年月日: "))))
    except:
        print("日期不合法....")

    #  print(core(*[int(x) for x in input("请输入年月日:").split()]))

    #  year, month, day = [int(x) for x in input("请输入年月日: ").split()]
    #  print(core(year, month, day))

    #自动测试代码
    #  for i in range(1, 16):
        #  print("%d-%d-%d: " % (year, month, i), core(year, month, i))


def main():
    def end(index, args):
        print("--------------------------------------------")
        print("本题测试完成, 回车继续, q为退出: ", end='')
        return True if input() == 'q' else False

    menuList = menu.add("拿球", ball)
    menu.add("买鸡", items=menuList)
    menu.add("9x9乘法表", mul9x9, items=menuList)
    menu.add("黑色星期五", blackFri, items=menuList)
    menu.add("打鱼晒网", fish, items=menuList)
    menu.add("最大公约数", items=menuList)
    menu.add("4, 5, 6, 7组合数", items=menuList)
    menu.add("输出1-1/2+1/3-1/4+1/5...+1/99-1/100结果", items=menuList)
    menu.add("日历", items=menuList)
    menu.add("回文数", items=menuList)
    menu.add("完数", items=menuList)
    menu.add("水仙花数", items=menuList)
    menu.add("八进制转十进制", items=menuList)
    menu.add("菜单打印图形", items=menuList)
    menu.add("把八进制数转化为十进制数输出", items=menuList)
    menu.add("实现菜单打印图形", items=menuList)
    menu.add("退出", items=menuList)
    menu.run(menuList, "====== 第一天练习题 ======", end)


if __name__ == "__main__":
    main()


#!/usr/bin/env python3


#袋子里有三个红球三个绿球六个黄球，一次拿六个，列出所有组合
def ball():
    red = 0

    while red <= 3:
        green = 0

        while green <= 3:
            print("红", red, "绿", green, "黄", 6 - red - green)
            green += 1

        red +=1


#公鸡5元一只，母鸡3元一只，小鸡1元三只，现打算用100元买一百只鸡，列出多有组合
def chicken():
    gongji = 0

    while gongji <= 20:
        muji = 0

        while muji <= 33:
            xiaoji = 100 - gongji - muji

            if xiaoji % 3 == 0 and gongji*5 + muji*3 + xiaoji//3 == 100:
                print(gongji, muji, xiaoji)

            muji += 1
        gongji += 1


#9*9乘法表
def li():
    r = 1

    while r < 10:
        c = 1

        while c <= r:
            print("%d*%d=%d  "%(c,r,r*c), end='' )
            c += 1

        print()
        r += 1


#4/5/6/7能组合多少个不能被4整除的4位数（位数不能相同，一个循环）
def fournum():
    n = 4567

    while n < 7655:
        h = n % 10
        j = n // 10 % 10
        k = n // 100 % 10
        l = n // 1000 % 10

#        print(n, h, j, k, l)

        if (h != j and h != k and h != l and j != k and j != l and k != l
                and n % 4 != 0
                and (h == 4 or h == 5 or h == 6 or h == 7)
                and (j == 4 or j == 5 or j == 6 or j == 7)
                and (k == 4 or k == 5 or k == 6 or k == 7)
                and (l == 4 or l == 5 or l == 6 or l == 7)):
            print (n)

        n += 1


#两个数的最大公约数
def maxgys():
    m = int(input("请输入一个数"))
    n = int(input("请再输入一个数"))
    a = m
    if n < m:
        a = n
    while True:
        if m % a == 0 and n % a== 0:
            print(a)
            break
        else:
            a -= 1


#已知1900年1月1号是礼拜一，问今年有多少个黑色星期五（每个月13号是星期五）
def isleap(year):
    return 1 if year % 400 == 0 or year % 4 == 0 and year % 100 != 0 else 0

def countYearDay(sy, ey):
    days = 0
    while sy < ey:
        days += 365 +isleap(sy)
        sy += 1

    return days

def black(ey):
    sy = 1900
    days = countYearDay(sy, ey) + 13

    m = 1
    while m < 13:
        if m == 3:
            days += 28 + isleap(ey)
        elif m == 5 or m == 7 or m == 10 or m == 12:
            days += 30
        elif m != 1:
            days += 31

        if days % 7 == 5:
            print ("%d年%d月13号是黑色星期五" %(ey, m))
        m += 1



def fish(y, m, d):
    days = countYearDay(2000, y) + d
    i = 1
    while i < m:
        if m == i:
            break
        if m == 3:
            days += 28 + isleap(y)
        elif m == 5 or m == 7 or m == 10 or m == 12:
            days += 30
        else:
            days += 31
        i += 1


    if 0 < (days % 5) < 4:
        print("他%d年%d月%d日在打鱼"%(y, m, d))
    else :
        print("他%d年%d月%d日在晒网"%(y, m, d))

while True:
    print("欢迎进入ｈｏｍｅｗｏｒｋ")
    print("输入数字 1 可以玩球")
    print("输入数字 2 可以玩鸡")
    print("输入数字 3 打印9*9表")
    print("输入数字 4 列出4/5/6/7不能被4整除的数字")
    print("输入数字 5 可以列出两个数的最大公约数")
    print("输入数字 6 可以看今年有多少黑色星期五")
    print("输入数字 7 可以判断这个人在打鱼还是晒网")
    print("输入 其他的东西 可以退出")
    num = input("请输入的数字：")
    print("-----------------------------------------")

    if num == "1":
        ball()
    elif num == "2":
        chicken()
    elif num == "3":
        li()
    elif num == "4":
        fournum()
    elif num == "5":
        maxgys()
    elif num == "6":
        black(int(input("输入今年是哪一年:")))
    elif num == "7":
        fish(int(input("输入当前年：")), int(input("请输入当前月")), int(input("请输入当前日")))
    else :
        break
    print("-----------------------------------------")






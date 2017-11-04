#!/usr/bin/env python3

import random

a = random.randint(1, 10000)
c = random.randint(1,10000)
b = a + c

randNum = random.randint(a, b)
count = c//10+1
m = a
n = b

while count != 0:
    print("---------------------------------------------------------")
    print("你猜的数在 ", m, "和", n, "中间")
    print("不想玩请输入0退出游戏")
    inputStr = input("请输入数字(你还有 %d 次机会):" % count )

    if inputStr == '':
        print("输入有误，请重新输入．．")
    elif inputStr == '0':
        print("88，欢迎下次再来挑战")
        break
    else:
        try:
            num = int(inputStr)
            if not m <= num <= n:
                print("非法操作..., 不退钱..")
            elif num == randNum:
                print("中大奖...")
                break
            elif num > randNum:
                print("真可惜，你输入的数就大了一点．．")
                n = num
            else:
                print("真可惜，你输入的数就小了一点．．")
                m = num
            count -= 1
        except:
            print("我让你输入是数字，好不好？")

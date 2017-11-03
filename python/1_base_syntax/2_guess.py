#!/usr/bin/env python3

#导入随机模块
import random

#获取1到100之间的随机数并且赋值给randNum变量
randNum = random.randint(1, 100)
#  print("randNum = ", randNum)

#定义猜数字的次数
count = 6

#根据猜数字的次数循环
while count != 0:
    inputStr = input("请输入1到100的数字(你还有 %d 次机会): " % count)

    if inputStr == '':
        print("输入有误, 请重新输入....")
    else:
        try:
            num = int(inputStr)
            if not 1 <= num <= 100:
                print("非法操作..., 不退钱..")
            elif num == randNum:
                print("中大奖...")
                break
            elif num > randNum:
                print("真可惜, 你输入的数就大了那么一点点...")
            else:
                print("真可惜, 你输入的数就小了那么一点点...")
            count -= 1
        except:
            print("我让你输入是数字, 好不好?")


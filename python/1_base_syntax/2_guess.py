#!/usr/bin/env python3

#导入随机模块
import random
import math

#  随机数范围个数
randCount = random.randint(10, 100)
#  随机数范围起始值
randStart = random.randint(1, 200)
randEnd = randStart + randCount

#获取1到100之间的随机数并且赋值给randNum变量
randNum = random.randint(randStart, randEnd)
#  print("randNum = ", randNum)

#定义猜数字的次数
count = int(math.log(randCount, 2)) - 1

#根据猜数字的次数循环
while count != 0:
    inputStr = input("请输入%d到%d的数字(你还有 %d 次机会): "
                        % (randStart, randEnd, count))

    if inputStr == '':
        print("输入有误, 请重新输入....")
    elif inputStr == 'q':   #支持退出
        break
    elif inputStr == 'p':   #内部作弊使用
        print(randNum)
    else:
        try:
            num = int(inputStr)

            if not randStart <= num <= randEnd:
                print("非法操作..., 不退钱..")
            elif num == randNum:
                print("恭喜你, 中大奖了!!!, 请留下联系电话等待通知!!")
                break
            elif num > randNum:
                print("真可惜, 你输入的数就大了那么一点点...")
            else:
                print("真可惜, 你输入的数就小了那么一点点...")

            count -= 1
        except:
            print("我让你输入是数字, 好不好?")


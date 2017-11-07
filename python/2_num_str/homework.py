#!/usr/bin/env python3

def isChar(ch):
    """
    判断它是否26个英文字母
    """
    return 'a' <= ch <= 'z' or 'A' <= ch <= 'Z'


def toConv(ch):
    """
    将传入的字符转化大小写, 传入字符必须是26个英文字母
    """
    ch = ord(ch)
    return chr(ch - 32 if ch >= 97 else ch + 32)


def toChar(ch):
    """
    对传入字符转换大小写, 依赖isChar和toConv函数
    """
    return toConv(ch) if isChar(ch) else ch


def toStr(inputStr):
    """
    将传入字符串转换大小写并且新字符串, 依赖toChar函数
    """
    newStr = ""

    for ch in inputStr:
        newStr += toChar(ch)

    return newStr



def toStrTest():
    """
    对输入的一个字符转换大小写(只需要使用三目运算符和函数)
        (考虑输入字符串进行同样转换)
    """
    s = input("请输入一个字符串: ")
    print(toStr(s))


def swapVarTest():
    """
    交换两个变量的值(至少两种方法)
    """
    a, b = 3, 5

    print("init a = %d, b = %d" % (a, b))

    #  第一种:
    #  a, b = b, a

    #  第二种:
    #  c = a
    #  a = b
    #  b = c

    #  第三种:
    a ^= b
    b ^= a
    a ^= b

    print("swap a = %d, b = %d" % (a, b))


def inputNum(run):
    while True:
        num  = input("请输入一个正整数(-1为退出): ")
        if num == "-1" or run(num):
            break


def bitOpTest():
    """
    将输入的正整数二进制第5位置0, 第3位置1, 再输出
    """
    def bitOp(userNum):
        num = int(userNum) & ~(1 << 4)
        num |= 1 << 2

        print("%s <=> %#x(%d)" % (userNum, num, num))

    inputNum(bitOp)


def countBitTest():
    """
    统计输入正整数的二进制中有多少个1
    """
    def countBit(userNum):
        num = int(userNum)
        count = 0

        for i in range(num.bit_length()):
            if num & (1 << i):
                count += 1

        print("%s <=> %d" % (userNum, count))

    inputNum(countBit)


def main():
    #  item = [swapVarTest, bitOpTest, countBitTest, toStrTest]
    item = [
            {"title":"交换两个变量的值(至少两种方法)", "call":swapVarTest},
            {"title":"将输入的正整数二进制第5位置0, 第3位置1, 再输出", "call":bitOpTest},
            {"title":"统计输入正整数的二进制中有多少个1", "call":countBitTest},
            {"title":"对输入字符串转换大小写", "call":toStrTest},
            {"title":"退出", "call":None}
           ]
    itemLen = len(item)

    while True:
        try:
            print("\033[2J\033[1;1H=======  第二天作业题 =======")
            #  print("=======  第二天作业题 =======")
            #  i = 1
            #  for s in item:
                #  print("%d. %s" % (i, s['title']))
                #  i += 1

            for i, s in enumerate(item):
                print("%d. %s" % (i + 1, s['title']))

            num = int(input("请输入[1 - %d]: " % itemLen))
            if num == itemLen:
                break

            item[int(num) - 1]['call']()

            print("--------------------------------------------")
            print("本题测试完成, 回车继续, q为退出: ", end='')
            if input() == 'q':
                break
        except:
            pass


if __name__ == "__main__":
    main()

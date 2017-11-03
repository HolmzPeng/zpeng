#!/usr/bin/env python3

def hello():
    global name     #告诉函数name这个变量是全局变量, 不声明无法修改全局变量
    print("hello name = ", name)
    name = "Kyo"


def out(s):
    print("id(s) = ", id(s))
    print("hello ", s, "!", sep="")
    s = "baidu"
    return s


def add(a, b):
    return a + b


name = "朱龙"
hello()
print("main name = ", name)

#  s = out(name)
print("main out return: ", out(name))
s = "world"
print("main s = ", s)
out(s)
out(add(add(6, 9), add(add(6, 7), 8)))

#!/usr/bin/env python3

class Student:
    def __init__(self, name="未知", age='20', sex='男'):
        self.name = name
        self.age = age
        self.sex = sex

    def say(self):
        print(type(self), id(self))
        print(self.name, self.age, self.sex)


def main():
    Student("张三", 12, '女').say()
    Student("李四").say()


if __name__ == "__main__":
    main()

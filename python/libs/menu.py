#!/usr/bin/env python3

def add(title, call=None, args=None, items=None):
    """
    组合菜单项
    """
    if type(items) != list:
        items = []

    data = {}
    data['title'] = title
    data['call'] = call
    data['args'] = args

    items.append(data)

    return items


def run(items, title=None, fixed=None):
    """
    运行菜单
    """
    if title is None:
        title = "菜单默认标题"

    itemLen = len(items)

    while True:
        try:
            if callable(fixed):
                print("\033[2J\033[1;1H", end="", flush=True)

            print(title)

            for i, s in enumerate(items):
                print("%d. %s" % (i + 1, s['title']))

            num = int(input("请输入[1 - %d]: " % itemLen)) - 1

            call = items[num]['call']

            if (not callable(call) or call(num, items[num]['args'])
                    or (callable(fixed) and fixed(num, items[num]['args']))):
                break
        except:
            pass


def main():
    def func(index, args):
        print("index = %d, args = %s" % (index, args))

    def end(index, args):
        print("--------------------------------------------")
        print("本题测试完成, 回车继续, q为退出: ", end='')
        return True if input() == 'q' else False

    menu = []

    add(menu, "1111111", func, "tom")
    add(menu, "2222222", func, "mary")
    add(menu, "3333333", func, "kyo")
    add(menu, "exit")

    run(menu, fixed=end)

if __name__ == "__main__":
    main()

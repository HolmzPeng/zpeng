#!/usr/bin/env python3

def main():
    r = r"hello\nworld"
    b = b"hello\nworld"
    u = "hello\rw\borl\"d"

    print(u)
    print("----------------------------------")
    print(r, b, u)

    print("num = %d, %x, %o, %#x, %#o" % (100, 100, 100, 100, 100))
    print("str = %s, char = %c" % ("hello", 97))
    print("float = %f" % (3456.789))

    print("H:%10d:H" % (199999))
    print("H:%010d:H" % (199999))
    print("H:%-10d:H" % (199))
    print("H:%-10.2f:H" % (199.567899))


if __name__ == "__main__":
    main()

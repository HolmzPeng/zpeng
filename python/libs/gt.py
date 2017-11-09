import sys, os
import time
import threading

def init():
    os.system("stty -echo -icanon")

def exit():
    os.system("stty echo icanon")

def get():
    return sys.stdin.read(1)

def w():
    return os.get_terminal_size().columns

def h():
    return os.get_terminal_size().lines

def size():
    tsize = os.get_terminal_size()
    return tsize.columns, tsize.lines

def sleep(sec=1):
    time.sleep(sec)

def run(f, interval=0.01, args=None, kwargs=None):
    t = threading.Timer(interval, f, args, kwargs)
    t.start()
    return t


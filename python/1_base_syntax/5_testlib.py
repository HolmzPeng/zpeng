#!/usr/bin/env python3

import threading
import kyo
import os

t = threading.Thread(target=kyo.progress, args=(0.1, ))
t.start()
t = threading.Thread(target=kyo.waitIco, args=(30, 0.1))
t.start()
#  kyo.progress(ms=0.01)
#  kyo.waitIco(ms=0.01)
input()

os._exit(0)


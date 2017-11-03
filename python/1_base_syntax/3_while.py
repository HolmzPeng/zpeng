#!/usr/bin/env python3

import time

i = 0

n = 0

while i < 10:
    i += 1

    #  过滤第3次和第5次
    if i == 3 or i == 5:
        continue

    #  累加
    n += i

    if n > 30:
        break

    print(i, ". hello n = ", n, sep='')
    #  if not (i == 3 or i == 5):
        #  print(i, ". hello", sep='')

i = 0
while i < 50:
    print("\033[s<", "=" * i, "> ", i * 2, "%\033[u", sep='', end='', flush=True)
    i += 1
    time.sleep(0.1)
print()

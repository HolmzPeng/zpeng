#!/usr/bin/env python3

import pygame
import time

def main():
    pygame.init()

    pygame.display.set_mode((640, 480))

    while True:
        e = pygame.event.wait()
        if e.type == pygame.QUIT:
            break
        elif e.type == pygame.KEYUP:
            if e.key == pygame.K_q:
                print("弹起")
        elif e.type == pygame.KEYDOWN:
            if e.key == pygame.K_q:
                print("按下")
            elif e.key == 27:
                break

    pygame.quit()

if __name__ == "__main__":
    main()

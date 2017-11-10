#!/usr/bin/env python3

import pygame
import time

def main():
    pygame.init()

    screen = pygame.display.set_mode((640, 480))

    #  screen.fill((255, 0, 0), (200, 200, 1, 1))
    pygame.draw.circle(screen, (255, 0, 0), (200, 200), 50)

    pygame.display.update()

    while True:
        e = pygame.event.wait()
        if e.type == pygame.QUIT:
            break
        elif e.type == pygame.KEYDOWN and (e.key == 27 or e.key == pygame.K_q):
            break

    pygame.quit()

if __name__ == "__main__":
    main()

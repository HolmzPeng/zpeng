#!/usr/bin/env python3

import pygame
import time

def main():
    pygame.init()

    screen = pygame.display.set_mode((640, 480))

    bg = pygame.image.load("./backGround.bmp")
    img = pygame.image.load("./player.bmp")
    img.set_colorkey((247, 0, 255))

    screen.blit(bg, (0, 0))

    for i in range(11):
        screen.blit(img, (58 * i, 0), (0, i * 48, 48, 48))

    for i in range(11):
        screen.blit(img, (58 * i, 48 * 5), (0, i * 48, 48, 48))

    pygame.display.update()

    i = 0
    y = 0
    x = 300
    while True:
        e = pygame.event.wait()
        if e.type == pygame.QUIT:
            break
        elif e.type == pygame.KEYDOWN:
            if e.key == 27 or e.key == pygame.K_q:
                break
            elif e.key == pygame.K_s:
                screen.blit(bg, (0, 0))
                screen.blit(img, (x, y), (0, i * 48, 48, 48))
                i += 1
                i %= 2
                y += 10
                pygame.display.update()
            elif e.key == pygame.K_w:
                if i < 2 or i > 3:
                    i = 2
                screen.blit(bg, (0, 0))
                screen.blit(img, (x, y), (0, i * 48, 48, 48))
                i += 1
                y -= 10
                pygame.display.update()
            elif e.key == pygame.K_a:
                if i < 4 or i > 6:
                    i = 4
                screen.blit(bg, (0, 0))
                screen.blit(img, (x, y), (0, i * 48, 48, 48))
                i += 1
                x -= 10
                pygame.display.update()
            elif e.key == pygame.K_d:
                if i < 6 or i > 8:
                    i = 6
                screen.blit(bg, (0, 0))
                screen.blit(img, (x, y), (0, i * 48, 48, 48))
                i += 1
                x += 10
                pygame.display.update()





    pygame.quit()

if __name__ == "__main__":
    main()

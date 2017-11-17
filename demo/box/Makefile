
CC = gcc

TARGET := box

SUBLIBS = main.o init.o game.o res.o gui.o #key.o

CFLAGS = -I.

LDFLAGS = -lSDL -lSDL_image -lSDL_ttf #-lSDL_mixer

.PHONY:$(TARGET)

all:$(TARGET)

$(TARGET):$(SUBLIBS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o:%.c
	$(CC) -g -o $@ -c $< $(CFLAGS)

clean:
	@rm $(TARGET) $(SUBLIBS) -rf


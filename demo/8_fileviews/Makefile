
TARGET ?= views

CC = gcc

LIBS = main.o
LIBS += views.o
LIBS += res.o
LIBS += sdl.o

CPPFLAGS = -I.

LDFLAGS =  -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer -lX11

.PHONY: $(TARGET)

all:$(TARGET)
	./views

$(TARGET):$(LIBS)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o:%.c
	$(CC) -c $< -o $@ $(CPPFLAGS)

clean:
	rm $(LIBS) $(TARGET) -f


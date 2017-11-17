MAIN = tank
CC = g++
CFLAGS =
INCLUDE = -I/usr/include/SDL
LIBS = -lSDL -lSDL_image -lSDL_mixer
HEADER =
OBJS = draw.o function.o game.o hit.o init.o keyevent.o newtank.o test.o

all: $(MAIN)

.c.o:
	$(CC) -c $(CFLAGS) $(INCLUDE) $<

$(MAIN): $(OBJS)
	$(CC) -o $(MAIN) $(OBJS) $(LIBS)

clean:
	rm -f *.o a.out $(MAIN) core *~

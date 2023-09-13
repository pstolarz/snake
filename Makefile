.SILENT:
.PHONY: all clean

CFLAGS:=-Icode

OBJS:=\
	code/bugs.o \
	code/game.o \
	code/main.o \
	code/snake.o

all: snake

snake: $(OBJS)
	$(CC) $+ -o $@ -lSDL2

clean:
	$(RM) $(OBJS) snake

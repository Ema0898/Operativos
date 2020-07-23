APP=main
CC=gcc
ROUTE=../bin/
FLAGS=-L../lib -I../include -lgui -lutilities -lcfg -llist -lmove -lSDL2_image -lSDL2_ttf -lconfig -llpthread -lm `sdl2-config --cflags --libs`

BIN=$(ROUTE)$(APP)
SRC=$(APP).c

all: COMPILE

COMPILE:
		$(CC) -o $(BIN) $(SRC) $(FLAGS)
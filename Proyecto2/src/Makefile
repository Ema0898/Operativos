APP=main
CC=gcc
ROUTE=../bin/
FLAGS=-L../lib -I../include -lgui -lutilities -lSDL2_image -lSDL2_ttf `sdl2-config --cflags --libs`

BIN=$(ROUTE)$(APP)
SRC=$(APP).c

all: COMPILE

COMPILE:
		$(CC) -o $(BIN) $(SRC) $(FLAGS)
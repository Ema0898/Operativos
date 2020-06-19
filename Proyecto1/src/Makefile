APP1=initializer
APP2=consumer
CC=gcc
ROUTE=../bin/
FLAGS=-L../lib -I../include -lutilities

BIN1=$(ROUTE)$(APP1)
SRC1=$(APP1).c
BIN2=$(ROUTE)$(APP2)
SRC2=$(APP2).c

all: COMPILE1 COMPILE2

COMPILE1:
		$(CC) -o $(BIN1) $(SRC1) $(FLAGS)

COMPILE2:
		$(CC) -o $(BIN2) $(SRC2)

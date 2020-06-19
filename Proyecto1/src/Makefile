APP1=initializer
APP2=consumer
APP3=producer
CC=gcc
ROUTE=../bin/
FLAGS=-L../lib -I../include -lutilities
FLAGS2=-I../include -lm

BIN1=$(ROUTE)$(APP1)
SRC1=$(APP1).c
BIN2=$(ROUTE)$(APP2)
SRC2=$(APP2).c
BIN3=$(ROUTE)$(APP3)
SRC3=$(APP3).c

all: COMPILE1 COMPILE2 COMPILE3

COMPILE1:
		$(CC) -o $(BIN1) $(SRC1) $(FLAGS)

COMPILE2:
		$(CC) -o $(BIN2) $(SRC2)

COMPILE3:
	$(CC) -o $(BIN3) $(SRC3) $(FLAGS2)

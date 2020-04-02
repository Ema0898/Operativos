.PHONY: clean all

APP=libutilities
LIB=utilities
OBJ=$(LIB).o
SRC=$(LIB).c
STATIC=$(APP).a
INCLUDE=../include/utilities.h
CC=gcc
MAKELIB=ar rv
RANLIB=ranlib

all: $(STATIC) clean

$(STATIC): $(OBJ)
		$(MAKELIB) $(STATIC) $(OBJ) && $(RANLIB) $(STATIC)


$(OBJ): $(SRC) $(INCLUDE)
		$(CC) -c $(SRC)

clean:
		rm  *.o

APP=median_mpi
ROUTE=../bin/
FLAGS=-L../lib -I../include -lppm -lprocess -lprint

BIN=$(ROUTE)$(APP)
SRC=$(APP).c

all: COMPILE

COMPILE:
		mpicc.mpich -o $(BIN) $(SRC) $(FLAGS)
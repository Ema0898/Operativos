.PHONY: clean all

APP_UTILITIES=libutilities
LIB_UTILITIES=utilities
OBJ_UTILITIES=$(LIB_UTILITIES).o
SRC_UTILITIES=$(LIB_UTILITIES).c
UTILITIES=$(APP_UTILITIES).a
INCLUDE_UTILITIES=../include/utilities.h

APP_SHMEM=libshmem
LIB_SHMEM=shmem
OBJ_SHMEM=$(LIB_SHMEM).o
SRC_SHMEM=$(LIB_SHMEM).c
SHMEM=$(APP_SHMEM).a
INCLUDE_SHMEM=../include/shmem.h
SHMEM_FLAGS=-I../include

APP_SEMAPHORE=libsemaphore
LIB_SEMAPHORE=semaphore
OBJ_SEMAPHORE=$(LIB_SEMAPHORE).o
SRC_SEMAPHORE=$(LIB_SEMAPHORE).c
SEMAPHORE=$(APP_SEMAPHORE).a
INCLUDE_SEMAPHORE=../include/semaphore.h

APP_PRINT=libprint
LIB_PRINT=print
OBJ_PRINT=$(LIB_PRINT).o
SRC_PRINT=$(LIB_PRINT).c
PRINT=$(APP_PRINT).a
INCLUDE_PRINT=../include/print.h

CC=gcc
MAKELIB=ar rv
RANLIB=ranlib

all: $(UTILITIES) $(SHMEM) $(SEMAPHORE) $(PRINT) clean

$(UTILITIES): $(OBJ_UTILITIES)
		$(MAKELIB) $(UTILITIES) $(OBJ_UTILITIES) && $(RANLIB) $(UTILITIES)

$(OBJ_UTILITIES): $(SRC_UTILITIES) $(INCLUDE_UTILITIES)
		$(CC) -c $(SRC_UTILITIES)


$(SHMEM): $(OBJ_SHMEM)
		$(MAKELIB) $(SHMEM) $(OBJ_SHMEM) && $(RANLIB) $(SHMEM)

$(OBJ_SHMEM): $(SRC_SHMEM) $(INCLUDE_SHMEM)
		$(CC) -c $(SRC_SHMEM) $(SHMEM_FLAGS)

$(SEMAPHORE): $(OBJ_SEMAPHORE)
		$(MAKELIB) $(SEMAPHORE) $(OBJ_SEMAPHORE) && $(RANLIB) $(SEMAPHORE)

$(OBJ_SEMAPHORE): $(SRC_SEMAPHORE) $(INCLUDE_SEMAPHORE)
		$(CC) -c $(SRC_SEMAPHORE)

$(PRINT): $(OBJ_PRINT)
		$(MAKELIB) $(PRINT) $(OBJ_PRINT) && $(RANLIB) $(PRINT)

$(OBJ_PRINT): $(SRC_PRINT) $(INCLUDE_PRINT)
		$(CC) -c $(SRC_PRINT)

clean:
		rm  *.o
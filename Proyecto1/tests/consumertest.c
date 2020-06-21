#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct
{
  int pid;
  int magic_numer;
  char date[50];
  char hour[50];
} message;

typedef struct
{
  int producers;
  int consumers;
  int size;
} global_variables;

int main()
{
  key_t key;
  int id_memory;
  global_variables *memory = NULL;

  key = ftok("/home/ema0898/Programas/Operativos/Proyecto1/gv", 33);
  if (key == -1)
  {
    printf("Shared Memory Key is Invalid\n");
    exit(0);
  }

  id_memory = shmget(key, sizeof(global_variables), 0777);
  if (id_memory == -1)
  {
    printf("Shared Memory Id is Invalid\n");
    exit(0);
  }

  memory = (global_variables *)shmat(id_memory, (char *)0, 0);
  if (memory == NULL)
  {
    printf("Can't get shared memory\n");
    exit(0);
  }

  printf("Read data 1 = %d\n", memory->size);

  if (id_memory != -1)
  {
    shmdt((char *)memory);
  }

  return 0;
}
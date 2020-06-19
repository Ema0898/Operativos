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
} global_variables;

int main()
{
  key_t key;
  int id_memory;
  message *memory = NULL;

  key = ftok("/home/ema0898/Programas/Operativos/Proyecto1/test", 33);
  if (key == -1)
  {
    printf("Shared Memory Key is Invalid\n");
    exit(0);
  }

  id_memory = shmget(key, sizeof(message) * 1024, 0777);
  if (id_memory == -1)
  {
    printf("Shared Memory Id is Invalid\n");
    exit(0);
  }

  memory = (message *)shmat(id_memory, (char *)0, 0);
  if (memory == NULL)
  {
    printf("Can't get shared memory\n");
    exit(0);
  }

  int gv_shm_id;
  global_variables *memory2 = NULL;
  key_t key2;

  key2 = ftok("/bin/ls", 33);

  if (key2 == -1)
  {
    printf("Shared Memory Key is Invalid\n");
    exit(0);
  }

  gv_shm_id = shmget(key2, sizeof(global_variables), 0777);

  if (gv_shm_id == -1)
  {
    printf("Can't create shared memory for global variables\n");
    exit(0);
  }

  /* Not necesary, test only */
  memory2 = (global_variables *)shmat(gv_shm_id, (char *)0, 0);
  if (memory2 == NULL)
  {
    printf("Can't get shared memory for global variable\n");
    exit(0);
  }

  printf("Global variable for producers = %d\n", memory2->producers);
  printf("Global variable for consumer = %d\n", memory2->consumers);

  // for (int i = 0; i < 10; i++)
  // {
  //   printf("Read data1 = %d\n", memory[i].data1);
  //   printf("Read data2 = %d\n", memory[i].data2);
  //   printf("Read data3 = %s\n", memory[i].data3);
  //   sleep(1);
  // }

  printf("Read data1 = %d\n", memory[1].pid);
  printf("Read data2 = %d\n", memory[1].magic_numer);
  printf("Read data3 = %s\n", memory[1].date);

  printf("Read data1 = %d\n", memory[2].pid);
  printf("Read data2 = %d\n", memory[2].magic_numer);
  printf("Read data3 = %s\n", memory[2].date);

  if (id_memory != -1)
  {
    shmdt((char *)memory);
  }

  shmdt((char *)memory2);

  return 0;
}
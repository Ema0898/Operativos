#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct
{
  int data1;
  int data2;
  char data3[10];
} message;

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

  // for (int i = 0; i < 10; i++)
  // {
  //   printf("Read data1 = %d\n", memory[i].data1);
  //   printf("Read data2 = %d\n", memory[i].data2);
  //   printf("Read data3 = %s\n", memory[i].data3);
  //   sleep(1);
  // }

  printf("Read data1 = %d\n", memory[1].data1);
  printf("Read data2 = %d\n", memory[1].data2);
  printf("Read data3 = %s\n", memory[1].data3);

  printf("Read data1 = %d\n", memory[2].data1);
  printf("Read data2 = %d\n", memory[2].data2);
  printf("Read data3 = %s\n", memory[2].data3);

  if (id_memory != -1)
  {
    shmdt((char *)memory);
  }

  return 0;
}
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
  key_t key;
  int id_memory;
  char *memory = NULL;

  key = ftok("/home/ema0898/Programas/Operativos/Proyecto1/test", 33);
  if (key == -1)
  {
    printf("Shared Memory Key is Invalid\n");
    exit(0);
  }

  id_memory = shmget(key, sizeof(char) * 1, 0777);
  if (id_memory == -1)
  {
    printf("Shared Memory Id is Invalid\n");
    exit(0);
  }

  memory = (char *)shmat(id_memory, (char *)0, 0);
  if (memory == NULL)
  {
    printf("Can't get shared memory\n");
    exit(0);
  }

  for (int i = 0; i < 10; i++)
  {
    printf("Read %c\n", memory[0]);
    sleep(1);
  }

  if (id_memory != -1)
  {
    shmdt((char *)memory);
  }

  return 0;
}
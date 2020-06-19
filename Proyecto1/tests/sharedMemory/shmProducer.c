#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
  key_t key;
  int id_memory;
  int *memory = NULL;

  key = ftok("/bin/ls", 33);
  if (key == -1)
  {
    printf("Shared Memory Key is Invalid\n");
    exit(0);
  }

  id_memory = shmget(key, sizeof(int) * 100, 0777 | IPC_CREAT);

  if (id_memory == -1)
  {
    printf("Shared Memory Id is Invalid\n");
    exit(0);
  }

  memory = (int *)shmat(id_memory, (char *)0, 0);
  if (memory == NULL)
  {
    printf("Can't get shared memory\n");
    exit(0);
  }

  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 100; j++)
    {
      memory[j] = i;
    }
    printf("Write %d on shared memory\n", i);

    sleep(1);
  }

  shmdt((char *)memory);
  shmctl(id_memory, IPC_RMID, (struct shmid_ds *)NULL);

  return 0;
}
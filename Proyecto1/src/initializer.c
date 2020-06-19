#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <utilities.h>

typedef struct
{
  int data1;
  int data2;
  char data3[10];
} message;

int main(int argc, char *argv[])
{
  /* Argument Validation */
  if (argc != 3)
  {
    printf("Usage: ./initializer <buffer_size> <buffer_name>\n");
    exit(0);
  }

  int buffer_size = atoi(argv[1]);
  char *buffer_name = argv[2];

  if (buffer_size == 0)
  {
    printf("Please insert a correct buffer size\n");
    exit(0);
  }

  checkDir(buffer_name);

  /* Shared Memory Initialization */

  key_t key;
  int id_memory;
  message *memory = NULL;

  key = ftok(buffer_name, 33);
  if (key == -1)
  {
    printf("Shared Memory Key is Invalid\n");
    exit(0);
  }

  id_memory = shmget(key, sizeof(message) * buffer_size, 0777 | IPC_CREAT);

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

  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 100; j++)
    {
      memory[j].data1 = 10 * j;
      memory[j].data2 = 20 * j;
      strcpy(memory[j].data3, "Hola");
    }
    printf("Write %d on shared memory\n", i);

    sleep(1);
  }

  shmdt((char *)memory);
  shmctl(id_memory, IPC_RMID, (struct shmid_ds *)NULL);

  return 0;
}
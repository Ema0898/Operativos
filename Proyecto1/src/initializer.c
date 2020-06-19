#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/sem.h>
#include <string.h>
#include <utilities.h>
#include <structs.h>

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

  /* Shared Memory Buffer Initialization */

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

  /* Shared Memory for Global Variables Initialization */
  int gv_shm_id;
  global_variables *memory2 = NULL;
  key_t key_global;

  key_global = ftok("/bin/ls", 33);

  if (key_global == -1)
  {
    printf("Shared Memory Key is Invalid\n");
    exit(0);
  }

  gv_shm_id = shmget(key_global, sizeof(global_variables), 0777 | IPC_CREAT);

  if (gv_shm_id == -1)
  {
    printf("Can't create shared memory for global variables\n");
    exit(0);
  }

  /* Create Semaphore */
  key_t key_semaphore;
  int id_semaphore;

  key_semaphore = ftok("/bin/ls", 33);

  if (key_semaphore == -1)
  {
    printf("Can't get semaphore key\n");
    exit(0);
  }

  id_semaphore = semget(key_semaphore, buffer_size, 0600 | IPC_CREAT);

  if (id_semaphore == -1)
  {
    printf("Can't create semaphore\n");
    exit(0);
  }

  struct sembuf operation;

  operation.sem_op = 1;
  operation.sem_flg = 0;

  for (int i = 0; i < buffer_size; ++i)
  {
    operation.sem_num = i;

    semop(id_semaphore, &operation, 1);
  }
  

  /* Not necesary, test only */

  // shmdt((char *)memory);
  // shmctl(id_memory, IPC_RMID, (struct shmid_ds *)NULL);

  // shmdt((char *)memory2);
  // shmctl(gv_shm_id, IPC_RMID, (struct shmid_ds *)NULL);

  // semctl(id_semaphore, 0, IPC_RMID, NULL);

  return 0;
}
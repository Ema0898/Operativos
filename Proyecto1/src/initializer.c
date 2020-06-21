#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <utilities.h>
#include <structs.h>
#include <shmem.h>
#include <stdlib.h>
#include <unistd.h>

void init_dirs(char *buffer_name);

int main(int argc, char *argv[])
{
  key_t key;
  int id_memory;

  key_t key_global;
  int gv_shm_id;
  global_variables *memory2 = NULL;

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

  /* Check and init directories */
  init_dirs(buffer_name);

  /* Shared Memory Buffer Initialization */
  char *key_route = concat("share_files/", buffer_name);
  checkDir(key_route);

  key = ftok(key_route, 33);
  if (key == -1)
  {
    printf("Buffer Key is Invalid\n");
    exit(0);
  }

  if (create_buffer(&id_memory, key, buffer_size) == 0)
  {
    printf("Can't create buffer memory\n");
    exit(0);
  }

  /* Shared Memory for Global Variables Initialization */
  key_global = ftok("share_files/global", 33);

  if (key_global == -1)
  {
    printf("Global Variables Key is Invalid\n");
    exit(0);
  }

  if (create_global(&gv_shm_id, key_global) == 0)
  {
    printf("Can't create global memory\n");
    exit(0);
  }

  if (get_global_memory(&gv_shm_id, &memory2) == 0)
  {
    printf("Can't get global memory\n");
    exit(0);
  }

  memory2->size = buffer_size;

  /* Create Semaphore */
  key_t key_semaphore;
  int id_semaphore;

  key_semaphore = ftok("share_files/sem", 33);

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

  return 0;
}

void init_dirs(char *buffer_name)
{
  if (checkDir("share_files") == 0)
  {
    create_dirs();
    char *route = concat("share_files/", buffer_name);
    create_dir(route);
  }
}
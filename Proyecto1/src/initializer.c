#include <stdio.h>
#include <stdlib.h>
#include <utilities.h>
#include <structs.h>
#include <shmem.h>
#include <semaphore.h>
#include <sys/sem.h>

int main(int argc, char *argv[])
{
  key_t key;
  int id_memory;
  message *memory = NULL;

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

  if (buffer_size == 0 || !is_number(argv[1]))
  {
    printf("Please insert a correct buffer size\n");
    exit(0);
  }

  /* Check and init directories */
  create_dirs(buffer_name);

  /* Shared Memory Buffer Initialization */
  char *key_route;
  if (check_bin_dir())
  {
    key_route = concat("../share_files/", buffer_name);
  }
  else
  {
    key_route = concat("share_files/", buffer_name);
  }

  check_dir(key_route);

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

  if (get_buffer_memory(&id_memory, &memory) == 0)
  {
    printf("Can't get buffer memory\n");
    exit(0);
  }

  /* Shared Memory for Global Variables Initialization */
  if (check_bin_dir())
  {
    key_global = ftok("../share_files/global", 33);
  }
  else
  {
    key_global = ftok("share_files/global", 33);
  }

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
  if (check_bin_dir())
  {
    create_semaphore("../share_files/sem", buffer_size);
  }
  else
  {
    create_semaphore("share_files/sem", buffer_size);
  }

  /* Set buffer to zero */
  init_memory(buffer_size, memory);

  return 0;
}
#include "../include/shmem.h"

/* Creates the buffer for shared memory */
int create_buffer(int *id, key_t mem_key, int mem_size)
{
  *id = shmget(mem_key, sizeof(message) * mem_size, 0777 | IPC_CREAT);
  if (*id == -1)
  {
    printf("Error en el get del buffer\n");
    return 0;
  }

  return 1;
}

/* Returns the buffer id for shared memory */
int get_buffer(int *id, key_t mem_key, int mem_size)
{
  *id = shmget(mem_key, sizeof(message) * mem_size, 0777);
  if (*id == -1)
  {
    printf("Error en el get del buffer\n");
    return 0;
  }

  return 1;
}

/* Returns the buffer memory for shared memory */
int get_buffer_memory(int *id, message **memory)
{
  *memory = (message *)shmat(*id, (char *)0, 0);
  if (*memory == NULL)
  {
    printf("Error en el mat del buffer\n");
    return 0;
  }

  return 1;
}

/* Creates the global variables for shared memory */
int create_global(int *id, key_t mem_key)
{
  *id = shmget(mem_key, 1 * sizeof(global_variables), 0777 | IPC_CREAT);
  if (*id == -1)
  {
    printf("Error en el get del buffer\n");
    return 0;
  }

  return 1;
}

/* Returns the global variables id for shared memory */
int get_global(int *id, key_t mem_key)
{
  *id = shmget(mem_key, 1 * sizeof(global_variables), 0777);
  if (*id == -1)
  {
    printf("Error en el get del buffer\n");
    return 0;
  }

  return 1;
}

/* Returns the global variables memory for shared memory */
int get_global_memory(int *id, global_variables **memory)
{
  *memory = (global_variables *)shmat(*id, (char *)0, 0);
  if (*memory == NULL)
  {
    printf("Error en el mat del buffer\n");
    return 0;
  }

  return 1;
}

/*Return is used*/
int get_index(int flag, int size, message *memory)
{
  for(int i = 0; i < size; i++)
  {
    if(memory[i].is_used == flag)
    {
      return i;
    }
  }

  return -1;
}

/*Set is_used*/
void init_memory(int size, message *memory)
{
  for(int i = 0; i < size; i++)
  {
    memory[i].is_used = 0;
    memory[i].pid = 0;
    memory[i].magic_number = 0;
    memset(memory[i].date, 0, 50 * sizeof(char));
  }
}

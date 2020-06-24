#ifndef _SHMEM_H
#define _SHMEM_H

#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <structs.h>
#include <string.h>
#include <sys/sem.h>


/* Library to create and manage shared memory */

int create_buffer(int *id, key_t mem_key, int mem_size);
int create_global(int *id, key_t mem_key);
int get_buffer(int *id, key_t mem_key, int mem_size);
int get_global(int *id, key_t mem_key);
int get_buffer_memory(int *id, message **memory);
int get_global_memory(int *id, global_variables **memory);
int get_index(int flag, int size, message *memory, int ref, int id_semaphore);
void init_memory(int size, message *memory);
int get_unread_messages(int size, message *memory);

#endif

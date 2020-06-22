#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

/* Library to create and init semaphores */

int init_semaphore(char *buff_name, int buffer_size);
void create_semaphore(char *route, int buffer_size);
void up_semaphore(int id, int index);
void down_semaphore(int id, int index);

#endif

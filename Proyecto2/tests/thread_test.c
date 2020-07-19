#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "lpthread.c"

lpthread_t tid[2];
int counter;
lpthread_mutex_t lock;

void *trythis(void *arg)
{
  Lmutex_lock(&lock);

  unsigned long i = 0;
  counter += 1;
  printf("\n Job %d has started\n", counter);

  for (i = 0; i < (0xFFFFFFFF); i++)
    ;

  printf("\n Job %d has finished\n", counter);

  Lmutex_unlock(&lock);

  return NULL;
}

int main(void)
{
  int i = 0;
  int error;

  if (Lmutex_init(&lock, NULL) != 0)
  {
    printf("\n mutex init has failed\n");
    return 1;
  }

  while (i < 2)
  {
    error = Lthread_create(&(tid[i]), NULL, &trythis, NULL);
    if (error != 0)
      printf("\nThread can't be created :[%s]",
             strerror(error));
    i++;
  }

  Lthread_join(tid[0], NULL);
  Lthread_join(tid[1], NULL);
  Lmutex_destroy(&lock);

  printf("Counter = %d\n", counter);

  return 0;
}
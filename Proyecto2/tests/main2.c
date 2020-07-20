#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *foo(void *i);

pthread_mutex_t lock;
int list[2];

int finish = 0;

int main()
{
  pthread_t threads[2];
  int iret1;

  list[0] = 0;
  list[1] = 10;
  list[2] = 20;

  if (pthread_mutex_init(&lock, NULL) != 0)
  {
    printf("Mutex init has failed\n");
    return 1;
  }

  for (int i = 0; i < 3; ++i)
  {
    int *arg = malloc(sizeof(*arg));
    *arg = i;

    iret1 = pthread_create(&threads[i], NULL, &foo, arg);

    if (iret1)
    {
      fprintf(stderr, "Error - pthread_create() return code: %d\n", iret1);
      exit(EXIT_FAILURE);
    }
  }

  int counter = 0;

  while (counter < 2)
  {
    counter++;
    sleep(1);
    printf("%d SECONDS HAS PASSED\n", counter);
  }

  finish = 1;

  pthread_join(threads[0], NULL);
  pthread_join(threads[1], NULL);
  pthread_join(threads[2], NULL);

  return 0;
}

void *foo(void *i)
{
  int hola;
  // pthread_mutex_lock(&lock);

  // int a = *((int *)i);
  // printf("List Index = %d, value = %d\n", a, list[a]);
  // free(i);

  // pthread_mutex_unlock(&lock);

  while (!finish)
  {
    hola++;
  }

  printf("THREAD FINISHED with %d\n", hola);

  return NULL;
}

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

int socket_desc, client_sock, c, *new_sock;
struct sockaddr_in server, client;
int port = 10001;

//the only filed that it parsed is 'Content-Length'
int ParseHeader(int sock)
{
  char c;
  char buff[1024] = "", *ptr = buff + 4;
  int bytes_received, status;
  printf("Begin HEADER ..\n");
  while (bytes_received = recv(sock, ptr, 1, 0))
  {
    if (bytes_received == -1)
    {
      perror("Parse Header");
      exit(1);
    }

    if (
        (ptr[-3] == '\r') && (ptr[-2] == '\n') &&
        (ptr[-1] == '\r') && (*ptr == '\n'))
      break;
    ptr++;
  }

  *ptr = 0;
  ptr = buff + 4;
  //printf("%s",ptr);

  if (bytes_received)
  {
    ptr = strstr(ptr, "Content-Length:");
    if (ptr)
    {
      sscanf(ptr, "%*s %d", &bytes_received);
    }
    else
      bytes_received = -1; //unknown size

    printf("Content-Length: %d\n", bytes_received);
  }
  printf("End HEADER ..\n");
  return bytes_received;
}

void *connection_handler(void *socket_desc)
{
  int sock = *(int *)socket_desc;
  int contentlengh, bytes_received;
  char recv_data[1024];

  if (contentlengh = ParseHeader(sock))
  {
    int bytes = 0;
    FILE *fd = fopen("test.png", "wb");
    printf("Saving data...\n\n");

    while (bytes_received = recv(sock, recv_data, 1024, 0))
    {
      if (bytes_received == -1)
      {
        perror("recieve");
        exit(3);
      }

      fwrite(recv_data, 1, bytes_received, fd);
      bytes += bytes_received;
      printf("Bytes recieved: %d from %d\n", bytes, contentlengh);
      if (bytes == contentlengh)
        break;
    }
    fclose(fd);
  }

  close(sock);
  printf("\n\nDone.\n\n");
}

void init_server()
{
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);

  if (socket_desc == -1)
  {
    printf("Could not create socket");
  }
  puts("Socket created");
  printf("Running on port %d\n", port);

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(port);

  if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
  {
    perror("bind failed. Error");
  }
  puts("bind done");

  listen(socket_desc, 3);

  c = sizeof(struct sockaddr_in);

  puts("Waiting for incoming connections...");
}

int run_server()
{
  while ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c)))
  {
    puts("Connection accepted");

    pthread_t sniffer_thread;
    new_sock = (int *)malloc(1);
    *new_sock = client_sock;

    if (pthread_create(&sniffer_thread, NULL, connection_handler, (void *)new_sock) < 0)
    {
      perror("could not create thread");
      return 1;
    }

    puts("Handler assigned");
    //pthread_join(sniffer_thread, NULL);
  }

  if (client_sock < 0)
  {
    perror("accept failed");
    return 1;
  }
}

int main()
{
  init_server();
  run_server();

  return 0;
}
#include <server.h>

void init_server()
{
    int port = 1773;
    char *colors = "/home/ema0898/Programas";
    char *histo = "/home/ema0898/Programas";
    char *logs = "/home/ema0898/Programas";

    getData(&port, &colors, &histo, &logs);

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
        EXIT_FAILURE;
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

void *connection_handler(void *socket_desc)
{
    int sock = *(int *)socket_desc;
    int read_size;
    char client_message[256];
    int bytesReceived = 0;

    FILE *fp;
    fp = fopen("/home/ema0898/Programas/Operativos/Tarea1/barbara2.jpg", "ab");
    if (NULL == fp)
    {
        printf("Error opening file");
    }

    /* Receive data in chunks of 256 bytes */
    while ((bytesReceived = read(sock, client_message, 256)) > 0)
    {
        if (strcmp(client_message, "END\r\n") == 0 || strcmp(client_message, "END") == 0)
        {
            printf("While terminated\n");
            break;
        }

        printf("Bytes received %d\n", bytesReceived);
        fwrite(client_message, 1, bytesReceived, fp);
    }

    fclose(fp);

    if (read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if (read_size == -1)
    {
        perror("recv failed");
    }

    free(socket_desc);
    close(sock);
}
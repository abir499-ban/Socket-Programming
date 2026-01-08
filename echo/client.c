#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

void chat(int sock_fd)
{
    char buffer[500];
    while (1)
    {
        printf("Enter data to be sent to server\n");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcmp(buffer, "exit") == 0)
            break;

        ssize_t n = strlen(buffer);
        write(sock_fd, buffer, n);

        ssize_t r = read(sock_fd, buffer, sizeof(buffer) - 1);
        if (r <= 0)
        {
            printf("Server closed\n");
            break;
        }
        buffer[r] = '\0';
        printf("Data received back from server: %s\n" , buffer);
    }
}

int main()
{
    printf("Client://\n");
    struct sockaddr_in serverAddr;

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
        printf("Client socket failed to open\n");
        exit(1);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9099);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock_fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        printf("Failed to connect\n");
        exit(1);
    }

    chat(sock_fd);
    close(sock_fd);
}
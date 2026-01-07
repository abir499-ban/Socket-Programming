#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void chat(int newSocket)
{
    char buffer[500];
    while (1)
    {
        ssize_t n = read(newSocket, buffer, sizeof(buffer));
        if (n == 0)
        {
            printf("Client closed connection\n");
            break;
        }

        if (n < 0)
        {
            printf("Error while reading");
            break;
        }
        write(newSocket, buffer, n);
    }
}

int main()
{
    struct sockaddr_in serveraddr, clientaddr;

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (sock_fd < 0)
    {
        printf("Error while opening Socket on Server\n");
        exit(1);
    }

    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(5001);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock_fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
    {
        printf("Error while binding server socket with address\n");
        exit(1);
    }

    listen(sock_fd, 1);

    while (1)
    {
        socklen_t clientAddLen = sizeof(clientaddr);
        int newSock_fd = accept(sock_fd, (struct sockaddr *)&clientaddr, &clientAddLen);

        if (newSock_fd >= 0)
        {
            chat(newSock_fd);
            close(newSock_fd);
        }
        else
        {
            printf("Error while accepting client connection request from kernel queue\n");
            exit(1);
        }
    }
    close(sock_fd);

    return 0;
}
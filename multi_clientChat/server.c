#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>
#define PORT 8040
#define MAX_CLIENTS 10

struct client
{
    int thread_id;
    int socketID;
    bool active;
};

bool lock = false;
struct client clients[MAX_CLIENTS];

bool TestAndSet(bool *lock)
{
    bool tmp = *lock;
    *lock = true;
    return tmp;
}

void *chat(void *args)
{
    struct client c = *(struct client *)args;
    char buffer[500];
    while (1)
    {
        memset(buffer, 0, sizeof(buffer));
        ssize_t n = read(c.socketID, buffer, sizeof(buffer) - 1);
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
        printf("data received:%s\n", buffer);
        write(c.socketID, buffer, n);
    }

    close(c.socketID);

    while(TestAndSet(&lock));
    clients[c.thread_id].active = false;
    lock = false;

    return NULL;
}

int main()
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        clients[i].active = false;
    }

    struct sockaddr_in serverAddr, clientAddr;

    int sockFd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockFd < 0)
    {
        printf("Error while opening Socket on Server\n");
        exit(1);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        printf("Error while binding server socket with address\n");
        exit(1);
    }

    listen(sockFd, 5);

    while (1)
    {
        socklen_t n = sizeof(clientAddr);
        int newSockFd = accept(sockFd, (struct sockaddr *)&clientAddr, &n);

        if (newSockFd < 0)
        {
            printf("Error while accepting client connection request from kernel queue\n");
            continue;
        }

        while (TestAndSet(&lock));
        int i = -1;
        for(int j = 0; j<MAX_CLIENTS; j++){
            if(!clients[j].active){
                i = j;
                break;
            }
        }
        if(i == -1){
            printf("Maximum limit of processing client connecton exceeded\n");
            close(newSockFd);
            lock = false;
            continue;
        }
        pthread_t p;
        clients[i].thread_id = i;
        clients[i].socketID = newSockFd;
        clients[i].active = true;
        pthread_create(&p, NULL, chat, &clients[i]);
        pthread_detach(p);
        lock = false;
    }
    close(sockFd);

    return 0;
}
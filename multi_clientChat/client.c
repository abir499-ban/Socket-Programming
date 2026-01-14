#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>
#define PORT 8040

void chat(int sockFd){
    char buffer[500];
    while(1){
        // New conversation
        memset(buffer , 0 , sizeof(buffer));
        printf("Enter data to be sent to server\n");
        fgets(buffer , sizeof(buffer) - 1 , stdin);


        buffer[strcspn(buffer , "\n")] = '\0';

        if(strcmp(buffer , "exit") == 0){
            break;
        }
        
        ssize_t n = strlen(buffer);
        write(sockFd , buffer , n);

        ssize_t r = read(sockFd , buffer , sizeof(buffer) - 1);
        if(r <= 0){
            printf("Server closed\n");
            break;
        }
        
        buffer[r] = '\0';
        printf("Received string: %s\n" , buffer);

    }
}

int main(){
    struct sockaddr_in serverAddr ;
    
    int sockFd = socket(AF_INET , SOCK_STREAM , 0);

    if (sockFd < 0)
    {
        printf("Client socket failed to open\n");
        exit(1);
    }


    memset(&serverAddr  , 0 ,sizeof(serverAddr));
    serverAddr.sin_family =AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(sockFd , (struct sockaddr*)&serverAddr , sizeof(serverAddr)) < 0){
        perror("Failed to connect\n");
        exit(1);
    }

    chat(sockFd);
    close(sockFd);

    return 0;

}
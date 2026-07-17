#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/epoll.h>

void* write_function(int* fd) {
    char message[1024];
    while(1) {
        memset(message, '\0', 1024);
        fgets(message, 1023, stdin);
        int len = strlen(message);
        if(len < 1)
            continue;
        write(*fd, message, len - 1);
    }
}

int main() {
    
    struct sockaddr_in client;

    client.sin_port = htons(17112);
    client.sin_family = AF_INET;
    client.sin_addr.s_addr = inet_addr("127.0.0.1");

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    int length = sizeof(client);
    int connection_status = connect(client_socket, (struct sockaddr *) &client, length) ;

    if(connection_status == 0)
        printf("Connected to server successfully\n");
    else {
        printf("connecting to server failed\n");
        exit(1);
    }

    char message[1024];
    pthread_t write_thread;
    pthread_create(&write_thread, NULL, (void *)write_function, &client_socket);

    int epfd = epoll_create1(0);

    struct epoll_event ev, events[10];
    ev.events = EPOLLIN;
    ev.data.fd = client_socket;
    
    epoll_ctl(epfd, EPOLL_CTL_ADD, client_socket, &ev);

    while(1) {
        epoll_wait(epfd, events, 10, -1);
        memset(message, '\0', 1023);
        read(client_socket, message, 1023);
        printf("%s\n", message);
    }
    
    return 0;
}

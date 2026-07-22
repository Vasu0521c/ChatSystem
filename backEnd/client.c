#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
/* #include <stdint.h> */
#include <netinet/in.h>
#include <arpa/inet.h>
/* #include <pthread.h> */
#include <sys/epoll.h>

#include "client.h"

/* void* write_function(int* fd) { */
/*     char message[1024]; */
/*     while(1) { */
/*         memset(message, '\0', 1024); */
/*         fgets(message, 1023, stdin); */
/*         int len = strlen(message); */
/*         if(len < 1) */
/*             continue; */
/*         write(*fd, message, len - 1); */
/*     } */
/* } */
int server_connect(char *ip, int port) {

    struct sockaddr_in client;
    client.sin_port = htons(port);
    client.sin_family = AF_INET;
    client.sin_addr.s_addr = inet_addr(ip);

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    int length = sizeof(client);

    int connection_status = connect(client_socket, (struct sockaddr *) &client, length) ;

    if(connection_status == 0)
        printf("Connected to server successfully\n");
    else {
        printf("connecting to server failed\n");
        return -1;
    }
    return client_socket;
}
    
void send_msg(int server_fd, char *msg, int length) {

    int c;
    while((c = getchar() != '\n' && c != EOF));
    write(server_fd, msg, length - 1);
}

void recv_msg(int client_socket) {

    char msg[1024];
    int size = 1023;
    memset(msg, 0, size);
    read(client_socket, msg, size);
    printf("%s\n", msg);
}

/* int main() { */
    
/*     int client_socket = server_connect("127.0.0.1", 17112); */
/*     if (client_socket == -1) { */
/*         printf("server connection failed\n" */
/*                "check server configuration or server is running\n"); */
/*         exit(1); */
/*     } */
/*     char message[1024]; */
/*     while(1) { */
/*         printf("Functions : \n" */
/*             "1. Send Message\n" */
/*             "2. Receive Message\n" */
/*             "3. Exit : \n"); */

/*         int choice = 0; */
/*         int c; */
/*         while((c = getchar() != '\n' && c != EOF)); */
/*         scanf("%d", &choice); */
/*         switch(choice) { */
/*             case 1: */
/*                 send_msg(client_socket, message, 1023); */
/*                 break; */
/*             case 2: */
/*                 recv_msg(client_socket, message, 1023); */
/*                 break; */
/*             case 3: */
/*                 exit(1); */
/*                 break; */
/*             default: */
/*                 printf("Enter valid opertions"); */
/*                 break; */
/*         } */
/*     } */
/*     /\* pthread_t write_thread; *\/ */
/*     /\* pthread_create(&write_thread, NULL, (void *)write_function, &client_socket); *\/ */

/*     /\* int epfd = epoll_create1(0); *\/ */

/*     /\* struct epoll_event ev, events[10]; *\/ */
/*     /\* ev.events = EPOLLIN; *\/ */
/*     /\* ev.data.fd = client_socket; *\/ */
    
/*     /\* epoll_ctl(epfd, EPOLL_CTL_ADD, client_socket, &ev); *\/ */

/*     /\* while(1) { *\/ */
/*     /\*     epoll_wait(epfd, events, 10, -1); *\/ */
/*     /\*     memset(message, '\0', 1023); *\/ */
/*     /\*     read(client_socket, message, 1023); *\/ */
/*     /\*     printf("%s\n", message); *\/ */
/*     /\* } *\/ */
/*     return 0; */
/* } */

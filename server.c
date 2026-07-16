#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <string.h>
#include <errno.h>

#define MAX_MSG_SIZE 1025

int main()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(17112);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    int binding_status = bind(server_fd, (struct sockaddr *)&server, sizeof(server));
    if(binding_status == -1) {
        printf("binding failed, errno : %d\n", errno);
        exit(1);
    }
    int listen_status = listen(server_fd, 10);
    if(listen_status != 0) {
        printf("listening failed\n");
        exit(1);
    }
    /* printf("listening on : %s  %d", server.sin_addr.s_addr, server.sin_port); */
    printf("listening on\n");

    int epfd = epoll_create1(0);

    struct epoll_event ev, events[10];

    ev.events = EPOLLIN;
    ev.data.fd = server_fd;

    epoll_ctl(epfd, EPOLL_CTL_ADD, server_fd, &ev);

    char buffer[MAX_MSG_SIZE];

    while (1)
    {
        int n = epoll_wait(epfd, events, 10, -1);
        if(n == -1) {
            printf("epoll waiting sequence failed\n");
            exit(1);
        }
        memset(buffer, '\0', 1023);

        for(int i = 0; i < n; i++) {
            int fd = events[i].data.fd;

            if(fd == server_fd) {
                int client = accept(server_fd, NULL, NULL);
                if(client == -1) {
                    printf("connection acceptance failed\n");
                    continue;
                }
                printf("connection acceptanced\n");
                ev.events = EPOLLIN;
                ev.data.fd = client;
                int ctl_status = epoll_ctl(epfd, EPOLL_CTL_ADD, client, &ev);
                if(ctl_status != 0) {
                    printf("epol control add sequence failed\n");
                    continue;
                }
                printf("epol control add sequence successfull\n");
                printf("Client %d connected\n", client);
            }
            else {
                int bytes = read(fd, buffer, sizeof(buffer));
                printf("%s\n", buffer);
                if(bytes <= 0) {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
                    close(fd);
                    printf("Client %d disconnected\n", fd);
                }
                else {
                    write(fd, "messafge received", strlen("messafge received"));
                }
            }
        }
    }
    return 0;
}

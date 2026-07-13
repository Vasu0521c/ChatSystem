#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>
  
typedef uint16_t bytes;

int main() {

    int the_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr, client;
    fd_set ready_set;
    addr.sin_port   = htons(17112);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int length = sizeof(addr);

    int binding_status = bind(the_sock, (struct sockaddr *) &addr, length);
    if(binding_status == 0) {
        printf("socket binding successfull\n");
    }
    else {
        printf("socket bindin failed\n");
        exit(1);
    }

    int epo = epoll_create(1);
    struct epoll_event eve;
    eve.data.fd = the_sock;
    epoll_ctl(epo, EPOLL_CTL_ADD , the_sock,);

    while(1) {
        char message[10000] = {};
        read(cli_soc, message, 9999);
        printf("The client says : %s",message);
        write(cli_soc, "message received", strlen("message received"));
    }

    return 0;
}

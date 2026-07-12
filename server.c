#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>
  
typedef uint16_t bytes;

int main() {

    int the_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr, client;
    addr.sin_port   = htons(17112);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int length = sizeof(addr);

    if(bind(the_sock, (struct sockaddr *) &addr, length) < 0)
        printf("socket bindin failed\n");

    listen(the_sock, 1);
    int x = sizeof(client);
    printf("server is listerning .....\n");
    int cli_soc = accept(the_sock, (struct sockaddr *) &client, &x);
    while(1) {
    char message[100] = {};
    read(cli_soc, message, x);
    printf("The client says : %s",message);
    write(cli_soc, "message received", strlen("message received"));
    }

    return 0;
}

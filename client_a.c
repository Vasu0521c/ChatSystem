#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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
    while(1) {
        char message[100];
        fgets(message, 99, stdin);
        int len = strlen(message);
        if(len < 1)
            continue;
        write(client_socket, message, len);
    }
    
    return 0;
}

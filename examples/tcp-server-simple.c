#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <getopt.h>

#define BUFFER_SIZE 500

int main(int argc, char *argv[]) {
    char *hostname = "0.0.0.0";
    int port=5555;
    int opt;
    while ((opt = getopt(argc, argv, "h:p:")) != -1) {
        switch (opt) {
        case 'h':
            hostname = optarg;
            break;
        case 'p':
            port = atoi(optarg);
            break;
        default: /* '?' */
            fprintf(stderr, "Usage: %s [-h hostname] [-p port]\n", argv[0]);
            exit(1);
        }
    }
    char buffer[BUFFER_SIZE+1];
    struct sockaddr_in my_address, client_address;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    int srv_socket;

    my_address.sin_family = AF_INET;
    my_address.sin_port = htons(port);
    inet_pton(AF_INET, hostname, &(my_address.sin_addr));
    memset(&(my_address.sin_zero), 0, sizeof(my_address.sin_zero));
    srv_socket = socket(AF_INET, SOCK_STREAM, 0); // Check for errors
    bind(srv_socket, (struct sockaddr *) &my_address, sizeof(struct sockaddr_in)); // Check for errors

    listen(srv_socket, 10);
    int client_socket = accept(srv_socket, (struct sockaddr *) &client_address, &addr_size); // Check for errors

    while(1) {
        size_t bytes_read = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Received: %s\n", buffer);
            printf("From: %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
            for(int i = 0; i < BUFFER_SIZE; i++) {
              if(buffer[i] == '\n' || buffer[i] == EOF) {
                buffer[i] = '\0';
                break;
              }
            }
            strcat(buffer, " - Message received by the server");
            size_t bytes_sent = send(client_socket, buffer, strlen(buffer)+1, 0); // Check for errors
        }
    }
    // TODO: Cleanup
    close(client_socket);
    close(srv_socket);
    return 0;
}
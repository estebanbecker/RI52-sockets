#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <getopt.h>
#include <poll.h>

#define BUFFER_SIZE 1000
#define MAX_CLIENTS 100

struct pollfd clients[MAX_CLIENTS];
struct sockaddr_in client_addresses[MAX_CLIENTS];
int nb_clients = 0;

void broadcast_message(char *message, int exclude) {
    for (int i = 1; i < nb_clients; i++) {
        if (clients[i].fd != exclude) {
            ssize_t bytes_sent = send(clients[i].fd, message, strlen(message) + 1, 0);
            if (bytes_sent == -1) {
                perror("send");
            }
        }
    }
}

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
    clients[0].fd = srv_socket;
    clients[0].events = POLLIN;
    nb_clients++;

    printf("Server started on %s:%d\n", hostname, port);

    while(1) {
        poll(clients, nb_clients, -1);
        for (int i = 0; i < nb_clients; i++) {
            if (clients[i].revents & POLLIN) {
                if (clients[i].fd == srv_socket) {
                    // New connection
                    int client_socket = accept(srv_socket, (struct sockaddr *) &client_address, &addr_size);
                    clients[nb_clients].fd = client_socket;
                    clients[nb_clients].events = POLLIN;
                    client_addresses[nb_clients] = client_address;
                    nb_clients++;
                    printf("New connection from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
                } else {
                    // Message from a client
                    size_t bytes_read = recv(clients[i].fd, buffer, BUFFER_SIZE, 0);
                    if (bytes_read > 0) {
                        buffer[bytes_read] = '\0';
                        printf("Received: %s\n", buffer);
                        printf("From: %s:%d\n", inet_ntoa(client_addresses[i].sin_addr), ntohs(client_addresses[i].sin_port));
                        broadcast_message(buffer, clients[i].fd);
                    }
                }
            }
        }
    }
    for (int i = 0; i < nb_clients; i++) {
        close(clients[i].fd);
    }
    return 0;
}
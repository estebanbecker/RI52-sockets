#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <getopt.h> // Add this line to include the definition of 'optarg'

#define BUFFER_SIZE 1000

int main(int argc, char *argv[]) {
    char *hostname = "localhost";
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
    struct sockaddr_in remote_address;
    int sock_to_server;

    printf("Connecting to the serveur\n");
    remote_address.sin_family = AF_INET;
    remote_address.sin_port = htons(port);
    inet_pton(AF_INET, hostname, &(remote_address.sin_addr));
    memset(&(remote_address.sin_zero), 0, sizeof(remote_address.sin_zero));
    sock_to_server = socket(AF_INET, SOCK_STREAM, 0); // Check for errors
    connect(sock_to_server, (struct sockaddr *) &remote_address, sizeof(struct sockaddr_in));

    char name[BUFFER_SIZE+1];
    printf("Enter your username: ");
    fflush(stdout);
    size_t len_name = read(STDIN_FILENO, name, BUFFER_SIZE);
    if (name[len_name - 1] == '\n') {
        name[len_name - 1] = '\0';
    } else {
        name[len_name] = '\0';
    }


    while(1) {
        printf("Enter your message: ");
        fflush(stdout);
        size_t bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
        buffer[bytes_read] = '\0';
        //Concat name and buffer
        char message[BUFFER_SIZE+1];
        strncpy(message, name, BUFFER_SIZE); // Copy the username to the message
        strcat(message, ": "); // Add a colon and a space after the username
        strcat(message, buffer); // Add the message after the username
        size_t bytes_sent = send(sock_to_server, message, bytes_read+len_name+1, 0);

        size_t bytes_recieved = recv(sock_to_server, buffer, BUFFER_SIZE, 0);
        if (bytes_recieved > 0) {
            buffer[bytes_recieved] = '\0';
            printf("Received: %s\n", buffer);
            printf("From: %s:%d\n", inet_ntoa(remote_address.sin_addr), ntohs(remote_address.sin_port));
        }
    }
    // TODO: Cleanup
    return 0;
}

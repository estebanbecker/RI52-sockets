#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 500

int main(int argc, char *argv[]) {
  char buffer[BUFFER_SIZE+1];
  struct sockaddr_in my_address, client_address;
  int srv_socket;
  socklen_t addr_size = sizeof(struct sockaddr_in);

  my_address.sin_family = AF_INET;
  my_address.sin_port = htons(5555);
  inet_pton(AF_INET, "0.0.0.0", &(my_address.sin_addr));
  memset(&(my_address.sin_zero), 0, sizeof(my_address.sin_zero));

  srv_socket = socket(AF_INET, SOCK_DGRAM, 0); // Create a socket
  if (srv_socket == -1) {
    perror("socket");
    return 1;
  }

  if (bind(srv_socket, (struct sockaddr *) &my_address, sizeof(struct sockaddr_in)) == -1) {
    perror("bind");
    return 1;
  }

  while(1) {
    if (recvfrom(srv_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &client_address, &addr_size) > 0) {
      printf("Received: %s\n", buffer);
      printf("From: %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
    } else {
      perror("recvfrom");
      return 1;
    }
  }
  // TODO: Cleanup
  close(srv_socket);
  return 0;
}
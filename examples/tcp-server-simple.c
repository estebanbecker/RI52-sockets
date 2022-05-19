#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 500

int main(int argc, char *argv[]) {
  char buffer[BUFFER_SIZE+1];
  struct sockaddr_in my_address;
  int srv_socket;

  my_address.sin_family = AF_INET;
  my_address.sin_port = htons(5555);
  inet_pton(AF_INET, "0.0.0.0", &(my_address.sin_addr));
  memset(&(my_address.sin_zero), 0, sizeof(my_address.sin_zero));
  srv_socket = socket(AF_INET, SOCK_STREAM, 0); // Check for errors
  bind(srv_socket, (struct sockaddr *) &my_address, \
    sizeof(struct sockaddr_in));

  listen(srv_socket, 10);
  int client_socket = accept(srv_socket, NULL, NULL);
  while(1) {
    if (recv(client_socket, buffer, BUFFER_SIZE, 0) > 0) {
      printf("Received: %s\n", buffer);
    }
  }
  return 0;
}

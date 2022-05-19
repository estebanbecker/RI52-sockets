#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int main() {
  struct addrinfo addr_info;
  memset(&addr_info, 0, sizeof(struct addrinfo));

  addr_info.ai_flags = AI_PASSIVE;
  addr_info.ai_socktype = SOCK_STREAM;
  addr_info.ai_family = AF_INET;

  struct addrinfo *res = NULL;

  getaddrinfo("www.utbm.fr", NULL, &addr_info, &res);
/* ... */

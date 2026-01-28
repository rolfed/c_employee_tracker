#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 5555 
#define BACKLOG 5

int main(int argc, char *argv[] ) {
  if (argc != 2) {
    printf("Usage: %s <ip of the host>\n", argv[0]);
    return 0;
  }

  struct sockaddr_in server_info = {0};

  server_info.sin_family = AF_INET; // ipv4
  server_info.sin_addr.s_addr = inet_addr(argv[1]);
  server_info.sin_port = htons(PORT); // convert to network endiness

  int fd = socket(AF_INET, SOCK_STREAM, 0); 

  if (fd == -1) {
    perror("socket");
    return -1;
  }

  if (connect(fd, (struct sockaddr*)&server_info, sizeof(server_info)) == -1) {
    perror("connect");
    close(fd);
    return 0;
  }

  close(fd);
}



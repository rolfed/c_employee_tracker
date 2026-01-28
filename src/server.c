#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 5555 
#define BACKLOG 5

typedef enum {
  PROTO_HELLO,
} proto_type_e;

typedef struct {
  proto_type_e type;
  unsigned short len;
} proto_header_t;

void handle_client(int fd) {
  char buf[4096] = {0}; 
  proto_header_t *hdr = (proto_header_t *)buf;
  hdr->type = htonl(PROTO_HELLO);
  hdr->len = sizeof(int); // 4 bytes long

  int reallen = hdr->len;
  hdr->len = htons(hdr->len);

  int *data = (int*)&hdr[1];
  *data = htonl(1);

  write(fd, hdr, sizeof(proto_header_t) + reallen);
}

int main() {
  struct sockaddr_in server_info = {0};
  struct sockaddr_in client_info = {0};
  socklen_t client_size = 0;

  server_info.sin_family = AF_INET;
  server_info.sin_addr.s_addr = 0;
  server_info.sin_port = htons(PORT); // convert to network endiness

  int fd = socket(AF_INET, SOCK_STREAM, 0); 

  if (fd == -1) {
    perror("socket");
    return -1;
  }

  if (bind(fd, (struct sockaddr*)&server_info, sizeof(server_info)) == -1) {
    perror("bind");
    close(fd);
    return -1;
  }

  if (listen(fd, BACKLOG) == -1) {
    perror("listen");
    close(fd);
    return -1;
  }

  while(1) { // not a good practice temp work around
    int client_socket_fd = accept(fd, (struct sockaddr*)&client_info, &client_size);
    if (client_socket_fd == -1) {
      perror("accept");
      close(fd);
      return -1;
    } else {
      handle_client(client_socket_fd);
      printf("Accepted client_socket_fd=%d\n", client_socket_fd);
    }

    close(client_socket_fd);
  }

  return fd;
}



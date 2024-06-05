#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// TLV

typedef enum { PROTO_TLV } proto_type_e;

typedef struct {
  proto_type_e type;
  unsigned short len;
} proto_hdr_t;

void handle_client(int fd) {
  unsigned short headerlen = sizeof(int);
  char buf[4096] = {0};
  proto_hdr_t *hdr = (proto_hdr_t *)buf;

  hdr->type = htonl(PROTO_TLV);
  hdr->len = htons(headerlen);

  int *data = (int *)&hdr[1];
  *data = htonl(1);

  if (write(fd, hdr, sizeof(proto_hdr_t) + headerlen) == -1) {
    perror("write");

    return;
  }

  return;
}

int main(int argc, char *argv[]) {
  struct sockaddr_in serverInfo = {0};

  serverInfo.sin_family = AF_INET;
  serverInfo.sin_addr.s_addr = INADDR_ANY;
  serverInfo.sin_port = htons(8080);

  struct sockaddr_in clientInfo = {0};
  socklen_t clientAddrLen = sizeof(clientInfo);

  int sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sfd == -1) {
    perror("socket");
    return 1;
  }

  if (bind(sfd, (struct sockaddr *)&serverInfo, sizeof(serverInfo)) == -1) {
    perror("bind");
    close(sfd);

    return -1;
  }

  if (listen(sfd, 0) == -1) {
    perror("listen");
    close(sfd);

    return -1;
  }

  while (1) {
    int cfd = accept(sfd, (struct sockaddr *)&clientInfo, &clientAddrLen);
    if (cfd == -1) {
      perror("accept");
      close(sfd);

      return -1;
    }

    handle_client(cfd);
    close(cfd);
  }

  close(sfd);
  return 0;
}

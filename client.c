#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s <server ip>\n", argv[0]);

    return 0;
  }

  struct sockaddr_in serverInfo = {0};

  serverInfo.sin_family = AF_INET;
  serverInfo.sin_addr.s_addr = inet_addr(argv[1]);
  serverInfo.sin_port = htons(8080);

  int sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sfd == -1) {
    perror("socket");
    return 1;
  }

  if (connect(sfd, (struct sockaddr *)&serverInfo, sizeof(serverInfo)) == -1) {
    perror("connect");
    close(sfd);

    return -1;
  }

  close(sfd);

  return 0;
}

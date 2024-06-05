#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

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

  if (accept(sfd, (struct sockaddr *)&clientInfo, &clientAddrLen) == -1) {
    perror("accept");
    close(sfd);

    return -1;
  }

  close(sfd);

  return 0;
}

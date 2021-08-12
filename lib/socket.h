#ifndef SOCKET_H
#define SOCKET_H
#include <netinet/in.h>

#define PORT	8080	/* web server listening port */

int _bind_(int *listen_fd, struct sockaddr_in *server_addr);
int _connect_(int *listen_fd, struct sockaddr_in *server_addr);
void _send_(FILE *fd, int socket_fd);
void _recv_(int listen_fd,char *buff, int max_len);

#endif

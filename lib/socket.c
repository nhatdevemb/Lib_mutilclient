#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "socket.h"

//khởi tao socket_server
int _bind_(int *listen_fd, struct sockaddr_in *server_addr)
{
	if ((*listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket() error. in init_sock et.c");
		return -1;
	}
	printf("[+]Server socket created successfully.\n");
	server_addr->sin_family = AF_INET;
	server_addr->sin_port = htons(PORT);
	server_addr->sin_addr.s_addr = INADDR_ANY;

	// init_socket(&listen_fd, &server_addr);
	if (bind(*listen_fd, (struct sockaddr *)server_addr, sizeof(struct sockaddr_in)) == -1)
	{
		perror("bind() error.  in init_socket.c");
		return -1;
	}

	return 0;
}
//khởi tao socket_client
int _connect_(int *socket_fd, struct sockaddr_in *server_addr)
{
	if ((*socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket() error. in init_sock et.c");
		return -1;
	}
	server_addr->sin_family = AF_INET;
	server_addr->sin_port = htons(PORT);
	server_addr->sin_addr.s_addr = INADDR_ANY;

	if (connect(*socket_fd, (struct sockaddr *)server_addr, sizeof(struct sockaddr_in)) == -1)
	{
		perror("bind() error.  in init_socket.c");
		return -1;
	}

	return 0;
}
//gửi dữ liệu
void _send_(FILE *fd, int socket_fd)
{
	char data[50];

	while (fgets(data, 50, fd) != NULL)
	{
		if (send(socket_fd, data, sizeof(data), 0) == -1)
		{
			perror("send file error");
			exit(1);
		}
		bzero(data, 50);
	}
}
//nhận dữ liệu
void _recv_(int listen_fd, char *buff, int max_len)
{

	char tmp[max_len];
	int n;
	n = recv(listen_fd, tmp, max_len, 0);
	if (n <= 0)
	{
		return;
		printf("recv error !");
	}
	snprintf(buff, max_len, "%s", tmp); //kiem tra gioi han bo dem,nap du lieu vao buffer
}
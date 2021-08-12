
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "socket.h"
#define PORT 8080
#define SIZE 1024

int main(int argc, char const *argv[])
{ 
  char buffer[1024];
  FILE *fp;
  char *filename = "send.txt";
  int socket_fd;
  struct sockaddr_in server_addr;
  //Khởi tạo socket
  _connect_(&socket_fd, &server_addr);
  printf("Connected to Server.\n");
  //mo file
  fp = fopen(filename, "r");
  if (fp == NULL)
  {
    perror("[-]Error in reading file.");
    exit(1);
  }
  //gui file
  _send_(fp, socket_fd);
  printf("File data sent successfully.\n");
  memset(buffer, 0, sizeof(buffer));
  _recv_(socket_fd, buffer, sizeof(buffer));
 

  printf("Closing the connection.\n");
  close(socket_fd);

  return 0;
}
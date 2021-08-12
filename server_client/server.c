#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>	   //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include "socket.h"

#define TRUE 1
#define FALSE 0

int main(int argc, char *argv[])
{
	int opt = TRUE;
	int listen_fd, addrlen, new_socket, client_socket[30], max_clients = 30, act, i, valread, sd;
	int max_sd, len;
	struct sockaddr_in server_addr, client_addr;

	char buffer[1025];

	//mô tả file sẵn sàng đọc
	fd_set readfds;

	char *message = "hello from server Nhat \r\n";

	//khỏi tạo client_socket[] bằng 0
	for (i = 0; i < max_clients; i++)
	{
		client_socket[i] = 0;
	}
	//Khởi tạo socket
	_bind_(&listen_fd, &server_addr);
	//tạo hàng chờ cho client
	if (listen(listen_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	addrlen = sizeof(client_addr);
	puts("Waiting for connections ...");

	while (TRUE)
	{
		//khởi tạo tập hợp mô tả file có địa chỉ fdset về trạng thái rỗng
		FD_ZERO(&readfds);

		//add listen_fd vào tệp readfds
		FD_SET(listen_fd, &readfds);
		max_sd = listen_fd;

		//thêm client_sockets vào tệp readfds
		for (i = 0; i < max_clients; i++)
		{
			//sd là mô tả của các socket
			sd = client_socket[i];

			//sd >0 thì thêm vào tệp readfds
			if (sd > 0)
				FD_SET(sd, &readfds);

			//max_sd là sô bộ mô tả tệp nhiều nhất
			if (sd > max_sd)
				max_sd = sd;
		}

		//Select() hoạt động bằng cách gom các mô tả file muốn theo dõi vào một tập hợp (fd_set)
		//và sẽ block chương trình cho đến khi một hoặc nhiều file trong tập hợp sẵn sàng.
		//Select() trả về một số nguyên là số mô tả file đã sẵn sàng, hoặc 0 nếu xảy ra timeout, hoặc -1 nếu xảy ra lỗi.
		//Tập hợp mô tả file writefds, exceptfds và timeout truyền vào NULL
		act = select(max_sd + 1, &readfds, NULL, NULL, NULL);

		if ((act < 0) && (errno != EINTR))
		{
			printf("select error");
		}

		// Nếu server sock có sự kiện ready to read nghĩa là nó có kết nối mới
		if (FD_ISSET(listen_fd, &readfds))
		{
			//accept 1 kết nối mới đó
			if ((new_socket = accept(listen_fd, (struct sockaddr *)&client_addr, (socklen_t *)&addrlen)) < 0)
			{
				perror("accept");
				exit(EXIT_FAILURE);
			}

			printf("Ket noi moi , socket fd is %d\n", new_socket);

			//sgửi message cho client đó
			if (send(new_socket, message, strlen(message), 0) != strlen(message))
			{
				perror("send");
			}

			//thêm socket mới vào mảng các socket
			for (i = 0; i < max_clients; i++)
			{
				if (client_socket[i] == 0)
				{
					client_socket[i] = new_socket;
					printf("Them vao danh sach socket vi tri: %d\n", i);

					break;
				}
			}
		}

		//kiểm tra các socket đã kết nối để nhận dữ liệu hoặc sự kiện đóng kêt nối
		for (i = 0; i < max_clients; i++)
		{
			sd = client_socket[i];

			if (FD_ISSET(sd, &readfds))
			{
				memset(buffer, 0, sizeof(buffer));
				//Nhận dữ liệu
				if ((valread = recv(sd, buffer, 1024, 0)) > 0)
				{
					printf("nhan thanh cong :'%s'，sum %d byte\n", buffer, valread);
				}

				//Ngắt kết nối
				else
				{ //hàm dùng để lấy lấy thông tin địa chỉ của client.
					getpeername(sd, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr);
					printf("Host disconnected , ip %s , port %d \n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

					//đóng socket
					close(sd);
					client_socket[i] = 0;
				}
			}
		}
	}

	return 0;
}

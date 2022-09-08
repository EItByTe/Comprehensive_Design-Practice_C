
//UdpNetClient.cpp

#include <Winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS

using namespace std;

void main()
{
	//加载套接字库
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return;
	}

	if (LOBYTE(wsaData.wVersion) != 1 ||     //低字节为主版本
		HIBYTE(wsaData.wVersion) != 1)      //高字节为副版本
	{
		WSACleanup();
		return;
	}

	printf("Client is operating!\n\n");
	//创建用于监听的套接字
	SOCKET sockSrv = socket(AF_INET, SOCK_DGRAM, 0);

	sockaddr_in  addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("192.168.0.107");//输入你想通信的她（此处是本机内部）
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8090);


	int len = sizeof(SOCKADDR);

	char recvBuf[100];    //收
	char sendBuf[100];    //发
	char tempBuf[100];    //存储中间信息数据

	while (1)
	{

		printf("Please input data: \n");
		std::cin>>sendBuf;
		sendto(sockSrv, sendBuf, strlen(sendBuf) + 1, 0, (SOCKADDR*)&addrSrv, len);
		//等待并数据
		recvfrom(sockSrv, recvBuf, 100, 0, (SOCKADDR*)&addrSrv, &len);

		if ('q' == recvBuf[0])
		{
			sendto(sockSrv, "q", strlen("q") + 1, 0, (SOCKADDR*)&addrSrv, len);
			printf("Chat end!\n");
			break;
		}
		sprintf_s(tempBuf, "%s say : %s", inet_ntoa(addrSrv.sin_addr), recvBuf);
		printf("%s\n", tempBuf);

		//发送数据

	}
	closesocket(sockSrv);
	WSACleanup();
}
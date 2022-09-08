
//UdpNetClient.cpp

#include <Winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS

using namespace std;

void main()
{
	//�����׽��ֿ�
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return;
	}

	if (LOBYTE(wsaData.wVersion) != 1 ||     //���ֽ�Ϊ���汾
		HIBYTE(wsaData.wVersion) != 1)      //���ֽ�Ϊ���汾
	{
		WSACleanup();
		return;
	}

	printf("Client is operating!\n\n");
	//�������ڼ������׽���
	SOCKET sockSrv = socket(AF_INET, SOCK_DGRAM, 0);

	sockaddr_in  addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("192.168.0.107");//��������ͨ�ŵ������˴��Ǳ����ڲ���
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8090);


	int len = sizeof(SOCKADDR);

	char recvBuf[100];    //��
	char sendBuf[100];    //��
	char tempBuf[100];    //�洢�м���Ϣ����

	while (1)
	{

		printf("Please input data: \n");
		std::cin>>sendBuf;
		sendto(sockSrv, sendBuf, strlen(sendBuf) + 1, 0, (SOCKADDR*)&addrSrv, len);
		//�ȴ�������
		recvfrom(sockSrv, recvBuf, 100, 0, (SOCKADDR*)&addrSrv, &len);

		if ('q' == recvBuf[0])
		{
			sendto(sockSrv, "q", strlen("q") + 1, 0, (SOCKADDR*)&addrSrv, len);
			printf("Chat end!\n");
			break;
		}
		sprintf_s(tempBuf, "%s say : %s", inet_ntoa(addrSrv.sin_addr), recvBuf);
		printf("%s\n", tempBuf);

		//��������

	}
	closesocket(sockSrv);
	WSACleanup();
}
//UdpNetSrv.cpp
#include <Winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <iostream>
using namespace std;
//#define _WINSOCK_DEPRECATED_NO_WARNINGS

int main()
{
	//�����׽��ֿ�
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);//����᷵��WSASYSNOTREADY
	if (err != 0)
	{
		return 0;
	}

	if (LOBYTE(wsaData.wVersion) != 1 ||     //���ֽ�Ϊ���汾
		HIBYTE(wsaData.wVersion) != 1)      //���ֽ�Ϊ���汾
	{
		WSACleanup();
		return 0;
	}

	printf("server is operating!\n\n");
	//�������ڼ������׽���
	SOCKET sockSrv = socket(AF_INET, SOCK_DGRAM, 0);//ʧ�ܻ᷵�� INVALID_SOCKET
	//printf("Failed. Error Code : %d",WSAGetLastError())//��ʾ������Ϣ

	SOCKADDR_IN addrSrv;     //����sockSrv���ͺͽ������ݰ��ĵ�ַ
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8080);

	//���׽���, �󶨵��˿�
	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));//�᷵��һ��SOCKET_ERROR
	//���׽�����Ϊ����ģʽ�� ׼�����տͻ�����


	SOCKADDR_IN addrClient;   //�������տͻ��˵ĵ�ַ��Ϣ
	int len = sizeof(SOCKADDR);
	char recvBuf[10];    //��
	char sendBuf[1024];    //��
	//char tempBuf[1000];    //�洢�м���Ϣ����

	while (1)
	{
		//�ȴ�������
		recvfrom(sockSrv, recvBuf, 10, 0, (SOCKADDR*)&addrClient, &len);
		for (int i = 0; i < 10; i++) {
			if(int(recvBuf[i])>0)
			cout << recvBuf[i];
		}
		//printf("recv %s\n", recvBuf);
		cout << "here" << endl;

		if ('q' == recvBuf[0])
		{
			sendto(sockSrv, "q", strlen("q") + 1, 0, (SOCKADDR*)&addrClient, len);
			printf("Chat end!\n");
			break;
		}
		//sprintf_s(tempBuf,1, "%s say : %s", inet_ntoa(addrClient.sin_addr), recvBuf);
		//printf("%s\n", tempBuf);

		//��������
		printf("Please input data: \n");
		cin>>sendBuf;
		sendto(sockSrv, sendBuf, strlen(sendBuf) + 1, 0, (SOCKADDR*)&addrClient, len);
	}
	closesocket(sockSrv);
	WSACleanup();
	return 0;
}

#include <graphics.h>         //引用easyX图形库
#include <Windows.h> 
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <MMSystem.h>

#include<winsock2.h>
#include<string.h>
#include<iostream>
using namespace std;
#pragma comment(lib,"ws2_32.lib")
#define BUFFER_SIZE 4000
#define _WINSOCK_DEPRECATED_NO_WARNINGS 

bool flag = 1;
enum DIRECTION { UP, DOWN, LEFT, RIGHT };  

//0 accessable, 1 barrier
int map[40][40] = {
	//{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	//{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	//{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0 },
	//{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0 },
	//{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0 },
	//{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0 },
	//{ 0,0,1,1,0,0,1,1,0,0,1,1,2,2,1,1,0,0,1,1,0,0,1,1,0,0 },
	//{ 0,0,1,1,0,0,1,1,0,0,1,1,2,2,1,1,0,0,1,1,0,0,1,1,0,0 },
	//{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0 },
	//{ 0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0 },
	//{ 0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0 },
	//{ 0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0 },
	//{ 0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0 },
	//{ 1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1 },
	//{ 2,2,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,2,2 },
	//{ 0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0 },
	//{ 0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0 },
	//{ 0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0 },
	//{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0 },
	//{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0 },
	//{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0 },
	//{ 0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0 },
	//{ 0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0 },
	//{ 0,0,1,1,0,0,1,1,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0 },
	//{ 0,0,0,0,0,0,0,0,0,0,0,1,3,4,1,0,0,0,0,0,0,0,0,0,0,0 },
	//{ 0,0,0,0,0,0,0,0,0,0,0,1,4,4,1,0,0,0,0,0,0,0,0,0,0,0 }
};

int path[400][3] = {};

//CAR(CUSTOMER)
struct CAR
{
	int CAR_x;
	int CAR_y;
	int direction;          //判断坦克方向，从而确定子弹的飞行轨迹    1 为上 ，2为下 ， 3为左， 4为右
	bool state;
};

//*******************  First page   **********************
void outputPage1()
{
	IMAGE logoImg;
	loadimage(&logoImg, _T("logo.png"), 413, 127);
	putimage(113, 40, &logoImg);

	settextcolor(RGB(65, 105, 225));
	settextstyle(30, 0, _T("微软雅黑"));
	//rectangle(195, 200, 275, 240);
	setlinecolor(RGB(65, 105, 225));
	setlinestyle(0, 2.5);
	rectangle(245, 260, 415, 300);
	//outtextxy(210, 205, _T("说 明"));
	outtextxy(250, 265, _T("CALL THE TAXI"));
	outtextxy(220, 205, _T("A Simulation of DIDI"));

	settextstyle(22, 0, _T("微软雅黑"));
	outtextxy(220, 500, _T("Designed by CC ZRG LEP CS"));

	//******************  detect the action of mouse  *********************
	MOUSEMSG m;
	while (1)
	{
		m = GetMouseMsg();
		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN:
			if ((m.x >= 245 && m.x <= 415) && (m.y >= 260 && m.y <= 300))
			{
				cleardevice();
				return;
			}
			break;

		}

	}

}

//***************** initialize the main page ********************
void outputPage2()
{

	IMAGE home;
	loadimage(&home, _T("building.png"), 25, 25);
	for (int i = 0; i < 26; i++)
	{
		for (int j = 0; j < 26; j++)
		{
			switch (map[i][j])
			{
			case 1:putimage(25 * j, 25 * i, &home); break;
			}
		}
	}
	return;
}
//********** manipulate the map arrary ************
void map_par(CAR* CAR, int n)
{
	map[CAR->CAR_y][CAR->CAR_x] = n;
	map[CAR->CAR_y + 1][CAR->CAR_x] = n;
	map[CAR->CAR_y][CAR->CAR_x + 1] = n;
	map[CAR->CAR_y + 1][CAR->CAR_x + 1] = n;
}
//*****************  move car  ********************
int CARMove(CAR* CAR, DIRECTION direction, IMAGE* img, int n)
{
	
	map_par(CAR, 0);
	setfillcolor(WHITE);
	solidrectangle(CAR->CAR_x * 25, CAR->CAR_y * 25, CAR->CAR_x * 25 + 50, CAR->CAR_y * 25 + 50);  //cover its old place
	if (direction == UP)
	{
		CAR->CAR_y--;
	}
	else if (direction == DOWN)
	{
		CAR->CAR_y++;
	}
	else if (direction == LEFT)
	{
		CAR->CAR_x--;
	}
	else if (direction == RIGHT)
	{
		CAR->CAR_x++;
	}
	else
	{
		return 0;
	}
	map_par(CAR, n);
	putimage(CAR->CAR_x * 25, CAR->CAR_y * 25, img);              //update the car image

}

void victory()
{
	//cleardevice();
	settextcolor(RGB(255,215,0));
	settextstyle(70, 35, _T("微软雅黑"));
	outtextxy(170, 170, _T("Got You!"));
}


// ******************************* move control *******************************
void allCARMove(CAR* CAR, DIRECTION direction, IMAGE* img, int n)
{
	switch (direction)
	{                                   //forward
	case UP:
		if (CAR->direction == UP && (CAR->CAR_y - 1) >= 0 && map[CAR->CAR_y - 1][CAR->CAR_x] == 0 && map[CAR->CAR_y - 1][CAR->CAR_x + 1] == 0)
		{
			CARMove(CAR, UP, img, n);
		}
		else if (CAR->direction != UP)
		{
			CAR->direction = UP;
			putimage(CAR->CAR_x * 25, CAR->CAR_y * 25, img);
		}
		break;                                       //backward
	case DOWN:
		if (CAR->direction == DOWN && (CAR->CAR_y + 2) <= 25 && map[CAR->CAR_y + 2][CAR->CAR_x] == 0 && map[CAR->CAR_y + 2][CAR->CAR_x + 1] == 0)
		{
			CARMove(CAR, DOWN, img, n);
		}
		else if (CAR->direction != DOWN)
		{
			CAR->direction = DOWN;
			putimage(CAR->CAR_x * 25, CAR->CAR_y * 25, img);
		}
		break;                                           //left
	case LEFT:
		if (CAR->direction == LEFT && (CAR->CAR_x - 1) >= 0 && map[CAR->CAR_y][CAR->CAR_x - 1] == 0 && map[CAR->CAR_y + 1][CAR->CAR_x - 1] == 0)
		{
			CARMove(CAR, LEFT, img, n);
		}
		else if (CAR->direction != LEFT)
		{
			CAR->direction = LEFT;
			putimage(CAR->CAR_x * 25, CAR->CAR_y * 25, img);
		}
		break;                                               //right
	case RIGHT:
		if (CAR->direction == RIGHT && (CAR->CAR_x + 2) <= 25 && map[CAR->CAR_y][CAR->CAR_x + 2] == 0 && map[CAR->CAR_y + 1][CAR->CAR_x + 2] == 0)
		{
			CARMove(CAR, RIGHT, img, n);
		}
		else if (CAR->direction != RIGHT)
		{
			CAR->direction = RIGHT;
			putimage(CAR->CAR_x * 25, CAR->CAR_y * 25, img);
		}
		break;
	}
}
//*********************   Start the simulation   ************************
void Start(int taxi_x, int taxi_y, int customer_x, int customer_y)
{
	srand((unsigned)time(NULL));
	int times = 0;                            //记录当前程序的休眠次数
	int taxiTotal = 0;
	//***********************  the Taxi  ***********************
	CAR myCARMove;                              //TAXI
	IMAGE myCAR_img;
	loadimage(&myCAR_img, _T("taxi.png"), 50, 50);

	myCARMove.CAR_x = taxi_x;                           //original location
	myCARMove.CAR_y = taxi_y;
	myCARMove.direction = UP;
	myCARMove.state = true;
	map_par(&myCARMove, 200);
	putimage(myCARMove.CAR_x * 25, myCARMove.CAR_y * 25, &myCAR_img);      //initialization

	//***************************  Customer **************************
	CAR Customer;
	IMAGE Customer_img;
	loadimage(&Customer_img, _T("school.png"), 50, 50);


	Customer.CAR_y = customer_y;
	Customer.CAR_x = customer_x;
		
	Customer.state = true;
	Customer.direction = DOWN;
	
	map_par(&Customer, 100);
	putimage(Customer.CAR_x * 25, Customer.CAR_y * 25, &Customer_img);
	
	taxiTotal = 1;

	// ***************   检测键盘事件  *******************
	bool Flag = 1;
	int step = 1;
	while (1)
	{
		//终点
		map_par(&Customer, 100);
		putimage(Customer.CAR_x * 25, Customer.CAR_y * 25, &Customer_img);
		
		// *********************************   小车移动 ****************************
		if (_kbhit())
		{

			//use the data from matlab
			int key = int(path[step][2]);   //direction
			int speed = abs(int(path[step][0]- path[step+1][0]))+ abs(int(path[step][1] - path[step + 1][1]));
			while (speed > 0) {
				speed--;
				switch (key)
				{
				case 0:
					if (Customer.CAR_x * 25 == myCARMove.CAR_x * 25 && Customer.CAR_y * 25 == myCARMove.CAR_y * 25 - 50)
					{
						Flag = 0;
						break;
					}
					allCARMove(&myCARMove, UP, &myCAR_img, 200);
					break;
				case 1:
					if (Customer.CAR_x * 25 == myCARMove.CAR_x * 25 && Customer.CAR_y * 25 == myCARMove.CAR_y * 25 + 50)
					{
						Flag = 0;
						break;
					}
					allCARMove(&myCARMove, DOWN, &myCAR_img, 200);
					break;
				case 2:
					if (Customer.CAR_x * 25 == myCARMove.CAR_x * 25 - 50 && Customer.CAR_y * 25 == myCARMove.CAR_y * 25)
					{
						Flag = 0;
						break;
					}
					allCARMove(&myCARMove, LEFT, &myCAR_img, 200);
					break;
				case 3:
					if (Customer.CAR_x * 25 == myCARMove.CAR_x * 25 + 50 && Customer.CAR_y * 25 == myCARMove.CAR_y * 25)
					{
						Flag = 0;
					}
					allCARMove(&myCARMove, RIGHT, &myCAR_img, 200);
					break;
				}
				Sleep(500);
			}
			
		}
		step++;
		if (!Flag)
		{
			break;
		}		
		/*outputPage2(map);*/

		Sleep(15);
	}
}
int main()
{
	SOCKET sock_Client; //客户端用于通信的Socket
	WSADATA WSAData;
	char receBuf[BUFFER_SIZE]; //接受数据的缓冲区
	char sendBuf1[2] = { 'm', '\0' }; //发送数据的缓冲区
	char sendBuf2[2] = { 'p', '\0' };
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
	{
		printf("初始化失败!");
		return -1;
	}
	//初始化
	sock_Client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);//创建客户端用于通信的Socket
	SOCKADDR_IN addr_server; //服务器的地址数据结构
	addr_server.sin_family = AF_INET;
	addr_server.sin_port = htons(8080);//端口号
	addr_server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); //127.0.0.1为本电脑IP地址
	SOCKADDR_IN sock;
	int len = sizeof(sock);

	sendto(sock_Client, sendBuf1, strlen(sendBuf1), 0, (SOCKADDR*)&addr_server, sizeof(SOCKADDR));
	cout << "send:" << sendBuf1 << endl;
	cout << "reciving map" << endl;
	//while (true)
	//{
		//int last=recv(sock_Client, receBuf, strlen(receBuf), 0); // (调用recv和recvfrom都可以)
	int last = recvfrom(sock_Client, receBuf, BUFFER_SIZE, 0, (SOCKADDR*)&addr_server, &len);
	if (last > 0)
	{
		receBuf[last] = '\0'; //给字符数组加一个'\0'，表示结束了。不然输出有乱码
		short* buff = (short*)receBuf;
		//printf("接收到数据：%s\n", buff);
		for (int i = 0; i < 40; i++) {
			for (int j = 0; j < 40; j++)
			{
				map[i][j] = buff[i * 40 + j];
			}
		}
		for (int i = 0; i < 40; i++) {
			for (int j = 0; j < 40; j++)
			{
				cout << map[i][j];
			}
			cout << endl;
		}
		//break;
	}
	//}
	memset(&receBuf[0], 0, BUFFER_SIZE);

	int path_len, taxix, taxiy, cus_x, cus_y;
	sendto(sock_Client, sendBuf2, strlen(sendBuf2), 0, (SOCKADDR*)&addr_server, sizeof(SOCKADDR));
	cout << "send:" << sendBuf2 << endl;
	cout << "reciving path..." << endl;
	//while (true)
	//{
		//int last=recv(sock_Client, receBuf, strlen(receBuf), 0); // (调用recv和recvfrom都可以)
	last = recvfrom(sock_Client, receBuf, BUFFER_SIZE, 0, (SOCKADDR*)&addr_server, &len);

	if (last > 0)
	{
		cout << "recived!" << last << endl;
		receBuf[last] = '\0'; //给字符数组加一个'\0'，表示结束了。不然输出有乱码
		short* buff = (short*)receBuf;
		path_len = buff[0];
		cus_x = buff[path_len * 3 - 2];
		cus_y = buff[path_len * 3 - 1];
		taxix = buff[1];
		taxiy = buff[2];
		for (int i = 0; i < 400; i++)
		{
			if (1 + i * 3 < last)
			{
				for (int j = 0; j < 3; j++)
				{
					path[i][j] = buff[1 + i * 3 + j];
				}
			}
		}
		//printf("接收到数据：%s\n", receBuf);
		for (int i = 0; i < 200; i++)
		{
			if (1 + i * 3 < last)
			{
				for (int j = 0; j < 3; j++)
				{
					if (path[i][j] > 0)
						cout << path[i][j];
				}
				cout << "  ";
			}
		}
		//break;
	}
	//}

	Sleep(20000);
	//主程序
	initgraph(650, 650);                  //初始化界面
	setbkcolor(0xffffff);
	cleardevice();
	outputPage1();
	outputPage2();

	closesocket(sock_Client);
	WSACleanup();

	Start(taxix, taxiy, cus_x, cus_y);
	victory();
	
	//system("pause");
	closegraph();
	return 0;
}

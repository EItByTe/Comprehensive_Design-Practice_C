#include "world.h"

World::World()
{
}

World::~World()
{
}

void World::open()
{
	map.Initialize(height, width);
}

void World::generate()
{

	const int MapSize = height * width;
	const double OccupancyRate = 0.3;
	const int OccupancyNum = int(OccupancyRate * MapSize);

	int OccupancyCnt = 0;


	//初始障碍物数
	srand(time(nullptr)); // 用当前时间作为种子
	int min = OccupancyNum / 30, max = OccupancyNum / 20;//这两个参数影响障碍物离散程度
	//int min = 200, max = 280;
	int OccupancyInitNum = (rand() % (max - min)) + min;//范围[min,max)
	//randomValue = (rand() % (max - min + 1)) + min;//范围[min,max]
	//randomValue = (rand() % (max - min)) + min + 1;//范围(min,max]

	//生成初始障碍物
	for (OccupancyCnt = 0; OccupancyCnt < OccupancyInitNum; ) {

		//srand(time(nullptr));
		//生成障碍物的位置
		int OccupancyRow = rand() % height;
		int OccupancyCol = rand() % width;
		if (map.map_ptr[OccupancyRow * height + OccupancyCol] == 0) {
			map.map_ptr[OccupancyRow * height + OccupancyCol] = 1;
			OccupancyCnt++;
		}

	}

	//循环填充
	while (OccupancyCnt < OccupancyNum)
	{
		for (int row = 0; row < height; row++) {
			for (int col = 0; col < width; col++) {
				//从已有障碍物扩展
				if (map.map_ptr[row * height + col] == 1) {
					//srand(time(nullptr));
					int row_d = rand() % 2 - 1;
					int col_d = rand() % 2 - 1;
					if (map.map_ptr[(row + row_d) * height + (col + col_d)] == 0
						&& (row + row_d) >= 0 && (row + row_d) < height
						&& (col + col_d) >= 0 && (col + col_d) < width) {
						map.map_ptr[(row + row_d) * height + (col + col_d)] = 1;
						OccupancyCnt++;
					}
				}

			}
		}
	}

	//智能体初始化，生成起点终点,保证连通
	srand(time(nullptr)); // 用当前时间作为种子
	for (int id = 0; id < num_of_agents; id++)
		agents[id].Initialize(height, width, map.map_ptr);

}

short* World::getMap() {
	return map.map_ptr;
}

bool World::showMap() {
	auto map_buf = getMap();
	if (map_buf == nullptr) {
		cout << "无初始化地图" << endl;
		return false;

	}
	else {
		for (int row = 0; row < height; row++) {
			for (int col = 0; col < width; col++) {
				cout << map_buf[row * height + col];
			}
			cout << endl;
		}

		return true;
	}

}


void World::getGlobalOptimalPath()
{
	//求出各智能体路径并保存
	for (int id = 0; id < num_of_agents; id++)
	{
		agents[id].getPath(height, width, map.map_ptr);


		// test: 输出智能体路径
		/*if (id == 0)
		{
			cout << "Agent number: " << id << endl;
			for (int i = 0; i < agents[id].length_of_path; i++)
				cout << i<<": "<<agents[id].movement[i * 3 + 0] << " "
				<< agents[id].movement[i * 3 + 1] << " "
				<< agents[id].movement[i * 3 + 2] << endl;
		}*/

		//agents[id].showPath(height, width, (short*)map.map_ptr);

	}
}

void World::startMove()
{
	// 取路径长度最大为总步数
	int total_step = 0;
	for (int id = 0; id < num_of_agents; id++)
	{
		total_step = max(total_step, agents[id].length_of_path);
	}

	// variables for clock test
	//int tmp = 0;
	//int tmpx, tmpy, tmpdir;
	while (total_step > 0)
	{
		total_step--;
		clock.delay_real();
		clock.step();
		for (int id = 0; id < num_of_agents; id++)
			clock.evolve(&agents[id]);

		//test clock
		//agents[tmp].getPose(&tmpx, &tmpy, &tmpdir);
		//cout << agents[tmp].current_step<<": " << tmpx << " " << tmpy << " " << tmpdir << endl;

		//TODO：保存当前世界状态并发送
		getSnapshot();
		encode();
	}

}


void World::getSnapshot()
{
	snapshot.agent_num = num_of_agents;
	snapshot.map_size = sizeof(short) * height * width;
	snapshot.pose_size = sizeof(short) * 3 * num_of_agents;

	if (snapshot.map == nullptr) {
		snapshot.map = (short*)malloc(snapshot.map_size);
	}
	if (snapshot.agent_pose == nullptr) {
		snapshot.agent_pose = (short*)malloc(snapshot.pose_size);
	}


	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			snapshot.map[row * width + col] = (short)map.map_ptr[row * width + col];
		}
	}

	for (int id = 0; id < num_of_agents; id++) {
		int x, y, dir;
		agents[id].getPose(&x, &y, &dir);
		snapshot.agent_pose[3 * id] = (short)x;
		snapshot.agent_pose[3 * id + 1] = (short)y;
		snapshot.agent_pose[3 * id + 2] = (short)dir;

	}

}

void World::encode() {
	short code_byte_num = sizeof(snapshot.agent_num) + sizeof(snapshot.map_size) + sizeof(snapshot.pose_size)
		+ snapshot.map_size + snapshot.pose_size + sizeof(short);
	//4
	if (code == nullptr) {
		code = (short*)malloc(code_byte_num);
	}
	code[0] = code_byte_num;
	code[1] = snapshot.agent_num;
	code[2] = snapshot.map_size;
	code[3] = snapshot.pose_size;

	//height*width
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			code[4 + row * width + col] = snapshot.map[row * width + col];
		}
	}
	////height*width*3
	//for (int row = 0; row < height; row++) {
	//	for (int col = 0; col < width; col++) {
	//		code[4 + height * width + row * width + col] = snapshot.movement[row * width + col];
	//	}
	//}

	for (int id = 0; id < num_of_agents; id++) {

				code[4 + height * width + id* 3] =snapshot.agent_pose[3 * id];
				code[4 + height * width + id * 3 + 1] =snapshot.agent_pose[3 * id + 1];
				code[4 + height * width + id * 3 + 2] =snapshot.agent_pose[3 * id + 2];

	}

	//cout << "\n\n\nencode test\n\n\n" << endl;
	//for (int i = 0; i < code_byte_num / 2; i++) {
	//	cout << code[i];
	//}
}

void World::decode() {
	short code_byte_num = code[0];
	snapshot.agent_num = code[1];
	snapshot.map_size = code[2];
	snapshot.pose_size = code[3];

	if (snapshot.map == nullptr) {
		snapshot.map = (short*)malloc(snapshot.map_size);
	}
	if (snapshot.agent_pose == nullptr) {
		snapshot.agent_pose = (short*)malloc(snapshot.pose_size);
	}

	//height*width
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			snapshot.map[row * width + col] = code[4 + row * width + col];
		}
	}
	////height*width*3
	//for (int row = 0; row < height; row++) {
	//	for (int col = 0; col < width; col++) {
	//		snapshot.movement[row * width + col]=code[4 + height * width + row * width + col];
	//	}
	//}
	for (int id = 0; id < num_of_agents; id++) {

		snapshot.agent_pose[3 * id]=code[4 + height * width + id * 3];
		snapshot.agent_pose[3 * id + 1]=code[4 + height * width + id * 3 + 1];
		snapshot.agent_pose[3 * id + 2]=code[4 + height * width + id * 3 + 2];

	}

}

void World::encode_map()
{
	//short code_byte_num = sizeof(snapshot.map_size) + snapshot.map_size + sizeof(short)
	//	+3*num_of_agents* sizeof(short);
	short code_byte_num = snapshot.map_size;
	//head(0)+size+map
	map_packet_len = code_byte_num;
	//4
	if (map_packet == nullptr) {
		map_packet = (short*)malloc(map_packet_len);
	}
	if (map_packet == nullptr) {
		cout<<"memory error map"<<endl;
	}	//map_packet[0] = 0;
	//map_packet[1] = snapshot.map_size;
	//for (int id = 0; id < num_of_agents; id++)
	//{
	//	//map_packet[2+id * 3 + 0] = (short)agents[id].movement[0];
	//	//map_packet[2+id * 3 + 1] = (short)agents[id].movement[1];
	//	//map_packet[2+id * 3 + 2] = (short)agents[id].movement[2];

	//	map_packet[ id * 3 + 0] = (short)agents[id].movement[0];
	//	map_packet[ id * 3 + 1] = (short)agents[id].movement[1];
	//	map_packet[ id * 3 + 2] = (short)agents[id].movement[2];
	//}

	//height*width
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			//map_packet[2+ num_of_agents*3 + row * width + col] = snapshot.map[row * width + col];
			map_packet[row * width + col] = snapshot.map[row * width + col];
		}
	}



}
void World::encode_path()
{

	//short code_byte_num = num_of_agents* sizeof(short)+ sizeof(short);//路径长度+head(1)
	int code_byte_num = num_of_agents * sizeof(short) ;//路径长度+head(1)
	for (int id = 0; id < num_of_agents; id++) {
		code_byte_num += agents[id].length_of_path * 3 * sizeof(short);
		//cout << agents[id].length_of_path << endl;
	}
	//head(0)+size+map
	path_packet_len = code_byte_num;
	//4
	if (path_packet == nullptr) {
		path_packet = (short*)malloc(code_byte_num);
	}

	//path_packet[0] = 1;//head

	//auto mark = 1;
	auto mark = 0;
	for (int id = 0; id < num_of_agents; id++) {

		path_packet[mark] = agents[id].length_of_path;
		mark += 1;
		for (int lp = 0; lp < agents[id].length_of_path; lp++)
		{
			path_packet[mark+ lp * 3 + 0] = (short)agents[id].movement[lp * 3 + 0];
			path_packet[mark+ lp * 3 + 1] = (short)agents[id].movement[lp * 3 + 1];
			path_packet[mark+ lp * 3 + 2] = (short)agents[id].movement[lp * 3 + 2];
		}
		mark += agents[id].length_of_path * 3 ;
		
	}

	for (int i = 0; i < 50; i++) {
		cout << path_packet[i] << " ";
	}
}



void World::sendCode()
{

	getSnapshot();
	
	encode_path();

	encode_map();



	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return;
	}

	if (LOBYTE(wsaData.wVersion) != 1 ||
		HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return;
	}

	printf("server is operating!\n\n");


	//发送两次
	SOCKET sockSen = socket(AF_INET, SOCK_DGRAM, 0);
	SOCKADDR_IN addrSen;
	//addrSen.sin_addr.S_un.S_addr = inet_addr("192.168.0.107");
	addrSen.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSen.sin_family = AF_INET;

	addrSen.sin_port = htons(8089);
	/*-----------add by ccsama----------*/
	//绑定套接字, 绑定到端口
	bind(sockSen, (SOCKADDR*)&addrSen, sizeof(SOCKADDR));//会返回一个SOCKET_ERROR
	//将套接字设为监听模式， 准备接收客户请求
	SOCKADDR_IN addrClient;   //用来接收客户端的地址信息
	int len_SOCKADDR = sizeof(SOCKADDR);

	//***********************************
	char recvBuf3[10];    //收
	printf("Now recving... \n");
	//等待接收数据
	recvfrom(sockSen, recvBuf3, 10, 0, (SOCKADDR*)&addrClient, &len_SOCKADDR);
	//打印接受的数据
	for (int i = 0; i < 10; i++) {
		if (int(recvBuf3[i]) > 0)
			cout << recvBuf3[i];
	}
	if (recvBuf3[0] == 'p') {
		printf("Now sending path... \n");
		sendto(sockSen, (char*)path_packet, path_packet_len, 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR));
		printf("sending finished! \n");
	}
	//********************************************

	char recvBuf2[10];    //收
	printf("Now recving... \n");
	//等待接收数据
	recvfrom(sockSen, recvBuf2, 10, 0, (SOCKADDR*)&addrClient, &len_SOCKADDR);
	//打印接受的数据
	for (int i = 0; i < 10; i++) {
		if (int(recvBuf2[i]) > 0)
			cout << recvBuf2[i];
	}

	if (recvBuf2[0] == 'm') {
		printf("Now sending map... \n");
		sendto(sockSen, (char*)map_packet, map_packet_len, 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR));
		printf("sending finished! \n");
	}

	char recvBuf[10];    //收
	printf("Now recving... \n");
	//等待接收数据
	recvfrom(sockSen, recvBuf, 10, 0, (SOCKADDR*)&addrClient, &len_SOCKADDR);
	//打印接受的数据
	for (int i = 0; i < 10; i++) {
		if (int(recvBuf[i]) > 0)
			cout << recvBuf[i];
	}
	if (recvBuf[0] == 'p') {
		printf("Now sending path... \n");
		sendto(sockSen, (char*)path_packet, path_packet_len, 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR));
		printf("sending finished! \n");
	}



	closesocket(sockSen);
	WSACleanup();







	//for (int i = 0; i < 50; i++) {
	//	cout << map_packet[i] << " ";
	//}

}

#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "map.h"
#include "Agent.h"
#include "Clock.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

#include <Winsock2.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996) 
using namespace std;
//typedef struct MapStruct
//{
//	
//}Map;

//typedef struct AgentStruct
//{
//
//}Agent;
typedef struct SnapshotStruct
{
	short* map;
	short map_size;

	short pose_size;
	short agent_num;
	short* agent_pose;
	

}Snapshot;

class World
{
public:
	World();
	~World();
	void generate();
	void open();
	short* getMap();
	bool showMap();

	void getGlobalOptimalPath();
	void startMove();

	void encode();
	void encode_map();
	void encode_path();
	void decode();
	void getSnapshot();

	void sendCode();

private:
	/*地图参数*/
	const int height = 40;
	const int width = 40;

	/*智能体参数*/
	const int num_of_agents = 1;

	Map map;
	Agent agents[5];
	Clock clock;

	Snapshot snapshot;
	short* code;
	short* map_packet;
	short map_packet_len;
	short* path_packet;
	short path_packet_len;
};
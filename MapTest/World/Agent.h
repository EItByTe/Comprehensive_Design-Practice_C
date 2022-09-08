#pragma once

#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

class Agent
{
public:
	Agent();
	~Agent();
	void Initialize(const int height, const int width, const short* map);
	bool getPath(const int height, const int width, const short* map);
	void showPath(const int height, const int width, const short* map);
	void setStep(const int step_of_clock);
	void getPose(int* x, int* y, int* dir);

	int startx, starty, finishx, finishy, length_of_path;
	int *movement; // ��¼agentÿһ����λ�ü��䷽�򣬴�0��ʼ���ܳ���Ϊlength_of_path
	int current_step; // agent��ǰ����

private:

	const int direction[4][2] = { {-1,0},{1,0},{0,-1},{0,1} };//�������ɽڵ���Χ�������ҵ�����
	const int UP = 0;
	const int DOWN = 1;
	const int LEFT = 2;
	const int RIGHT = 3;
	const int num_of_props = 3;

};
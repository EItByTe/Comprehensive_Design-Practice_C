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
	int *movement; // 记录agent每一步的位置及其方向，从0开始，总长度为length_of_path
	int current_step; // agent当前步数

private:

	const int direction[4][2] = { {-1,0},{1,0},{0,-1},{0,1} };//用来生成节点周围上下左右的坐标
	const int UP = 0;
	const int DOWN = 1;
	const int LEFT = 2;
	const int RIGHT = 3;
	const int num_of_props = 3;

};
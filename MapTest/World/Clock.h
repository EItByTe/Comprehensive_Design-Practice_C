#pragma once

#include "Agent.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <Windows.h>
using namespace std;

class Clock
{
public:
	Clock();
	~Clock();
	void step();
	void delay_real();
	void evolve(Agent *agent);


private:
	int counter;
	const int interval = 0; // 虚拟世界一个时间单位对应现实世界250ms
	
};
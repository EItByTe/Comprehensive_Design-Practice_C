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
	const int interval = 0; // ��������һ��ʱ�䵥λ��Ӧ��ʵ����250ms
	
};
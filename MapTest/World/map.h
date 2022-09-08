#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;
class Map
{
public:
	Map();
	Map(const int height, const int width);
	~Map();
	void Initialize(const int height, const int width);
	short* map_ptr = nullptr;

private:
	
	
};
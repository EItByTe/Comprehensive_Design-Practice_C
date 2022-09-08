#include <iostream>
#include <cstdlib>
#include <ctime>
#include "world.h"
using namespace std;

int main() {
	//const int height = 10;
	//const int width = 10;

	World world;
	world.open();
	world.generate();

	//Map test
	//world.showMap();

	//Agent test
	world.getGlobalOptimalPath();

	//Clock test
	//world.startMove();
	world.sendCode();

	return 0;
}
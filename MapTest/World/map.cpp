#include "map.h"

Map::Map()
{

}

Map::Map(const int height, const int width)
{
	map_ptr = (short*)malloc(sizeof(short) * height * width);
}

Map::~Map()
{
	free(map_ptr);
}

void Map::Initialize(const int height, const int width)
{
	map_ptr = (short*)malloc(sizeof(short) * height * width);
	// set 0
	if (map_ptr == nullptr) {
		cout << "³õÊ¼»¯µØÍ¼Ê§°Ü!" << endl;
	}
	else {
		for (int row = 0; row < height; row++) {
			for (int col = 0; col < width; col++) {
				map_ptr[row * width + col] = 0;
			}
		}
	}
}
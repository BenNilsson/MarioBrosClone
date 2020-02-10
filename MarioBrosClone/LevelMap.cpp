#include "LevelMap.h"

LevelMap::LevelMap(int map[MAP_HEIGHT][MAP_WIDTH])
{
	// Allocate memory for the level map
	mMap = new int* [MAP_HEIGHT];
	for(unsigned int i = 0; i < MAP_HEIGHT; i++)
	{
		mMap[i] = new int[MAP_WIDTH];
	}

	// Populate the array
	for (unsigned int i = 0; i < MAP_HEIGHT; i++)
	{
		for (unsigned int j = 0; j < MAP_WIDTH; j++)
		{
			mMap[i][j] = map[i][j];
		}
	}
}

LevelMap::~LevelMap()
{
	// Delete all elements of the array
	for (unsigned int i = 0; i < MAP_HEIGHT; i++)
	{
		delete[] mMap[i];
	}
	delete[] mMap;
}

int LevelMap::GetTileAt(unsigned int h, unsigned int w)
{
	// Only get tile within range
	if (h < MAP_HEIGHT && w < MAP_WIDTH)
	{
		return mMap[h][w];
	}

	// Return 0 if outside of range
	return 0;
}

void LevelMap::ChangeTileAt(unsigned int row, unsigned int column, unsigned int newValue)
{
	if (column < MAP_HEIGHT && row < MAP_WIDTH)
		mMap[column][row] = newValue;
}

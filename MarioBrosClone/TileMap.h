#pragma once
#include "Tile.h"
#include <vector>
#include "Sprite.h"
#include "Constants.h"

/*
	TILE MAP IDs
	0 = air
	1 = platform, unbreakable
	2 = floor
*/

class TileMap
{

public:

	TileMap(SDL_Renderer* renderer);
	~TileMap();

	void GenerateTileMap(int** map, int rows, int columns);
	void GenerateTileMap(int map[MAP_HEIGHT][MAP_WIDTH]);
	void DrawTileMap();

	Tile GetTileAt(unsigned int height, unsigned int width);
	void ChangeTileAt(unsigned int row, unsigned int column, Tile tile);

	std::vector<Tile*> mTileMap;
private:
	int** mMap;
	SDL_Renderer* mRenderer;

	Sprite* block;
	Sprite* floor;

};

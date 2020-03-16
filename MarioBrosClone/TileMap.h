#pragma once
#include "Tile.h"
#include <vector>
#include "Sprite.h"
#include "Constants.h"
#include "GameScreen.h"

/*
	TILE MAP IDs
	0 = air
	1 = platform
	2 = floor
	3 = question mark block
*/

class TileMap
{

public:

	TileMap(SDL_Renderer* renderer, GameScreen* level);
	~TileMap();

	void GenerateTileMap(int** map, int rows, int columns);
	void DrawTileMap();

	std::vector<Tile*> mTileMap;
private:
	int** mMap;
	SDL_Renderer* mRenderer;

	GameScreen* mGameScreen;

	Sprite* block;
	Sprite* floor;

};

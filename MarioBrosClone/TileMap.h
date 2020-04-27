#pragma once
#include "Tile.h"
#include "Sprite.h"
#include "Constants.h"
#include "Commons.h"
#include "GameScreen.h"
#include "CharacterKoopa.h"
#include "Coin.h"
#include "Flag.h"

#include <vector>

/*
	TILE MAP IDs
	0 = air
	1 = platform
	2 = floor
	3 = question mark block
	4 = step
	5 = pipe top left
	6 = pipe top right
	7 = pipe bottom left
	8 = pipe bottom right
	k = koopa
	p = platform BLUE
	f = floor BLUE
	q = question mark block BLUE
	s = step BLUE
	c = coin
	w = flag
*/

class TileMap
{

public:

	TileMap(SDL_Renderer* renderer);
	~TileMap();

	void GenerateTileMap(char** map, int rows, int columns);
	void DrawTileMap();

	Tile* GetTileAt(int x, int y);

	int GetWidth() { return mWidth; }
	int GetHeight() { return mHeight; }

	CollisionType GetCollision(int x, int y);

	bool IsLoaded() { return mLoaded; }

	std::vector<CharacterKoopa*> mKoopas;
	std::vector<Coin*> mCoins;

	Flag* mFlag;

private:
	char** mMap;
	SDL_Renderer* mRenderer;

	std::vector<std::vector<Tile*>>* mTiles;

	int mWidth;
	int mHeight;

	bool mLoaded;

};

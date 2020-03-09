#include "TileMap.h"

TileMap::TileMap(SDL_Renderer* renderer)
{
	mRenderer = renderer;

	block = new Sprite(mRenderer);
	block->LoadFromFile("Textures/block.png");

	floor = new Sprite(mRenderer);
	floor->LoadFromFile("Textures/floor.png");

	mMap = nullptr;
}

TileMap::~TileMap()
{
	delete mRenderer;
	mRenderer = nullptr;

	delete block;
	block = nullptr;

	delete floor;
	floor = nullptr;
}

void TileMap::GenerateTileMap(int** map, int rows, int columns)
{
	mMap = new int* [rows];
	for (unsigned int i = 0; i < rows; i++)
	{
		mMap[i] = new int[columns];
	}

	for (unsigned int row = 0; row < rows; row++)
	{
		for (unsigned int column = 0; column < columns; column++)
		{
			mMap[row][column] = map[row][column];

			int type = mMap[row][column];

			switch (type)
			{
			case 0:
				mTileMap.push_back(new Tile(Vector2D(column * 32, row * 32), nullptr, CollisionType::TILE_WALKABLE));
				break;

			case 1:
				mTileMap.push_back(new Tile(Vector2D(column * 32, row * 32), block, CollisionType::TILE_NONWALKABLE));
				break;

			case 2:
				mTileMap.push_back(new Tile(Vector2D(column * 32, row * 32), floor, CollisionType::TILE_NONWALKABLE));
			}
		}
	}
}

void TileMap::GenerateTileMap(int map[MAP_HEIGHT][MAP_WIDTH])
{
	// Allocate memory for the map
	mMap = new int* [MAP_HEIGHT];
	for (unsigned int i = 0; i < MAP_HEIGHT; i++)
	{
		mMap[i] = new int[MAP_WIDTH];
	}

	// Populate the array
	for (unsigned int row = 0; row < MAP_HEIGHT; row++)
	{
		for (unsigned int column = 0; column < MAP_WIDTH; column++)
		{
			mMap[row][column] = map[row][column];

			int type = mMap[row][column];

			switch (type)
			{
			case 0:
				mTileMap.push_back(new Tile(Vector2D(column * 32, row * 32), nullptr, CollisionType::TILE_WALKABLE));
				break;

			case 1:
				mTileMap.push_back(new Tile(Vector2D(column * 32, row * 32), block, CollisionType::TILE_NONWALKABLE));
				break;

			case 2:
				mTileMap.push_back(new Tile(Vector2D(column * 32, row * 32), floor, CollisionType::TILE_NONWALKABLE));
			}
		}
	}
}


void TileMap::DrawTileMap()
{
	for (const Tile* tile : mTileMap)
	{
		tile->Render();
	}
}



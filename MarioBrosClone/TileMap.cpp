#include "TileMap.h"
#include "Camera.h"
#include "Block.h"
#include "BlockQuestionMark.h"

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

	mWidth = columns;
	mHeight = rows;

	mTiles = new std::vector<std::vector<Tile*>>(columns, std::vector<Tile*>(rows));

	for (unsigned int row = 0; row < rows; row++)
	{
		for (unsigned int column = 0; column < columns; column++)
		{
			mMap[row][column] = map[row][column];

			int type = mMap[row][column];

			switch (type)
			{
			case 1:
				(*mTiles)[column][row] = new Tile(new Block(mRenderer, "Textures/block.png", Block::BlockType::BLOCK_PLATFORM, Vector2D(column * 32, row * 32)), CollisionType::TILE_SOLID);
				break;

			case 2:
				(*mTiles)[column][row] = new Tile(new Block(mRenderer, "Textures/floor.png", Block::BlockType::BLOCK_FLOOR, Vector2D(column * 32, row * 32)), CollisionType::TILE_SOLID);
				break;
			case 3:
				(*mTiles)[column][row] = new Tile(new BlockQuestionMark(mRenderer, "Textures/QuestionMarkBlock.png", Block::BlockType::BLOCK_QUESTION_MARK, Vector2D(column * 32, row * 32)), CollisionType::TILE_SOLID);
				break;
			case 4:
				(*mTiles)[column][row] = new Tile(new Block(mRenderer, "Textures/Step.png", Block::BlockType::BLOCK_STEP, Vector2D(column * 32, row * 32)), CollisionType::TILE_SOLID);
			}

		}
	}
}

void TileMap::DrawTileMap()
{
	for (int x = 0; x < GetWidth(); x++)
	{
		for (int y = 0; y < GetHeight(); y++)
		{
			Tile* t = GetTileAt(x, y);
			if (t != nullptr)
				t->Render(Camera::GetInstance()->GetPosition().x, Camera::GetInstance()->GetPosition().y);
		}
	}
}

Tile* TileMap::GetTileAt(int x, int y)
{
	if (x >= GetWidth() || x < 0) return nullptr;
	if (y >= GetHeight() || y < 0) return nullptr;

	Tile* t = mTiles->at(x).at(y);

	if (t != nullptr)
		return t;
	
	return nullptr;
}

CollisionType TileMap::GetCollision(int x, int y)
{
	// Only return collision if in bounds of level
	if (x < 0 || x >= GetWidth())
		return CollisionType::TILE_SOLID;

	// Allow jumping past the level top and falling through the bottom.
	if (y < 0 || y >= GetHeight())
		return CollisionType::TILE_AIR;

	if (mTiles->at(x).at(y) == nullptr) return CollisionType::TILE_AIR;

	return mTiles->at(x).at(y)->GetCollisionType();
}



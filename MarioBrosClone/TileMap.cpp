#include "TileMap.h"
#include "Camera.h"
#include "Block.h"
#include "BlockQuestionMark.h"

TileMap::TileMap(SDL_Renderer* renderer, GameScreen* level)
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
				mTileMap.push_back(new Tile(nullptr, CollisionType::TILE_WALKABLE));
				break;

			case 1:
				mTileMap.push_back(new Tile(new Block(mRenderer, "Textures/block.png", Block::BlockType::BLOCK_PLATFORM, Vector2D(column * 32, row * 32)), CollisionType::TILE_NONWALKABLE));
				break;

			case 2:
				mTileMap.push_back(new Tile(new Block(mRenderer, "Textures/floor.png", Block::BlockType::BLOCK_FLOOR, Vector2D(column * 32, row * 32)), CollisionType::TILE_NONWALKABLE));
				break;
			case 3:
				mTileMap.push_back(new Tile(new BlockQuestionMark(mRenderer, "Textures/QuestionMarkBlock.png", Block::BlockType::BLOCK_QUESTION_MARK, Vector2D(column * 32, row * 32)), CollisionType::TILE_NONWALKABLE));
				
			}

		}
	}
}

void TileMap::DrawTileMap()
{
	for (const Tile* tile : mTileMap)
	{
		tile->Render(Camera::GetInstance()->GetPosition().x, Camera::GetInstance()->GetPosition().y);
	}
}



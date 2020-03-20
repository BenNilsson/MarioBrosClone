#include "TileMap.h"
#include "Camera.h"
#include "Block.h"
#include "BlockQuestionMark.h"
#include "CharacterKoopa.h"

TileMap::TileMap(SDL_Renderer* renderer)
{
	mRenderer = renderer;
	
	mLoaded = false;

	mMap = nullptr;
}

TileMap::~TileMap()
{
	delete mRenderer;
	mRenderer = nullptr;

	for (CharacterKoopa* koopa : mKoopas)
	{
		delete koopa;
		koopa = nullptr;
	}

}

void TileMap::GenerateTileMap(char** map, int rows, int columns)
{
	mMap = new char* [rows];
	for (unsigned char i = 0; i < rows; i++)
	{
		mMap[i] = new char[columns];
	}

	mWidth = columns;
	mHeight = rows;

	

	mTiles = new std::vector<std::vector<Tile*>>(columns, std::vector<Tile*>(rows));

	mKoopas = std::vector<CharacterKoopa*>();

	for (unsigned int row = 0; row < rows; row++)
	{
		for (unsigned int column = 0; column < columns; column++)
		{
			mMap[row][column] = map[row][column];

			char type = mMap[row][column];

			switch (type)
			{
			case '1':
				(*mTiles)[column][row] = new Tile(new Block(mRenderer, "Textures/block.png", Block::BlockType::BLOCK_PLATFORM, Vector2D(column * 32, row * 32)), CollisionType::TILE_SOLID);
				break;
			case '2':
				(*mTiles)[column][row] = new Tile(new Block(mRenderer, "Textures/floor.png", Block::BlockType::BLOCK_FLOOR, Vector2D(column * 32, row * 32)), CollisionType::TILE_SOLID);
				break;
			case '3':
				(*mTiles)[column][row] = new Tile(new BlockQuestionMark(mRenderer, "Textures/QuestionMarkBlock.png", Block::BlockType::BLOCK_QUESTION_MARK, Vector2D(column * 32, row * 32)), CollisionType::TILE_SOLID);
				break;
			case '4':
				(*mTiles)[column][row] = new Tile(new Block(mRenderer, "Textures/Step.png", Block::BlockType::BLOCK_STEP, Vector2D(column * 32, row * 32)), CollisionType::TILE_SOLID);
				break;
			case '5':
				(*mTiles)[column][row] = new Tile(new Block(mRenderer, "Textures/PipeTopLeft.png", Block::BlockType::BLOCK_PIPE, Vector2D(column * 32, row * 32)), CollisionType::TILE_SOLID);
				break;
			case '6':
				(*mTiles)[column][row] = new Tile(new Block(mRenderer, "Textures/PipeTopRight.png", Block::BlockType::BLOCK_PIPE, Vector2D(column * 32, row * 32)), CollisionType::TILE_SOLID);
				break;
			case '7':
				(*mTiles)[column][row] = new Tile(new Block(mRenderer, "Textures/PipeBottomLeft.png", Block::BlockType::BLOCK_PIPE, Vector2D(column * 32, row * 32)), CollisionType::TILE_SOLID);
				break;
			case '8':
				(*mTiles)[column][row] = new Tile(new Block(mRenderer, "Textures/PipeBottomRight.png", Block::BlockType::BLOCK_PIPE, Vector2D(column * 32, row * 32)), CollisionType::TILE_SOLID);
				break;
			case 'k':
				mKoopas.push_back(new CharacterKoopa(mRenderer, "Textures/Koopa.png", Vector2D(column * 32, row * 32), this, 80.0f, FACING::FACING_RIGHT));
				break;
			}

		}
	}

	
	mLoaded = true;
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



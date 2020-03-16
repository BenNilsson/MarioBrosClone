#include "Block.h"
#include <iostream>

Block::Block(SDL_Renderer* renderer, std::string imagePath, BlockType type, Vector2D pos)
{
	mRenderer = renderer;

	mSprite = new Sprite(mRenderer);

	if (!mSprite->LoadFromFile(imagePath))
	{
		std::cerr << "Could not load file " << imagePath << std::endl;
	}

	mSingleSpriteWidth = mSprite->GetWidth();
	mSingleSpriteHeight = mSprite->GetHeight();

	mBlockType = type;
	mAvailable = true;

	mPosition = pos;
}

Block::~Block()
{
	
}

void Block::Update(float deltaTime, SDL_Event e)
{
	
}

void Block::Render(int camX, int camY)
{
	mSprite->Render(mPosition.x - camX, mPosition.y - camY, nullptr, 0.0, nullptr, SDL_FLIP_NONE);
}

void Block::SetAvailable(bool available)
{
	mAvailable = available;
}

float Block::GetWidth()
{
	return mSingleSpriteWidth;
}

float Block::GetHeight()
{
	return mSingleSpriteHeight;
}

void Block::SetPosition(Vector2D pos)
{
	mPosition = pos;
}

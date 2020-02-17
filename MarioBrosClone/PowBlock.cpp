#include "PowBlock.h"
#include <iostream>

PowBlock::PowBlock(SDL_Renderer* renderer, LevelMap* map)
{
	mRenderer = renderer;

	std::string imagePath = "Textures/PowBlock.png";
	mTexture = new Texture2D(renderer);
	if (!mTexture->LoadFromFile(imagePath.c_str()))
	{
		std::cout << "Failed to load texture: " << imagePath << std::endl;
		return;
	}

	mLevelMap = map;

	mSingleSpriteWidth = mTexture->GetWidth() / 3;
	mSingleSpriteHeight = mTexture->GetHeight();
	mNumberOfHitsLeft = 3;
	mPosition = Vector2D((SCREEN_WIDTH * 0.5) - mSingleSpriteWidth * 0.5f, 260);
}

PowBlock::~PowBlock()
{
	mRenderer = NULL;

	delete mTexture;
	mTexture = NULL;

	mLevelMap = NULL;
}

void PowBlock::Render()
{
	if (mNumberOfHitsLeft > 0)
	{
		// Get the position of the spritesheet
		int left = mSingleSpriteHeight * (mNumberOfHitsLeft - 1);

		// Set Rect
		SDL_Rect portionOfSpritesheet = { left, 0, mSingleSpriteWidth, mSingleSpriteHeight };

		// Determine where to draw
		SDL_Rect destRect = { (int)(mPosition.x), (int)(mPosition.y), mSingleSpriteWidth, mSingleSpriteHeight };

		// Draw
		mTexture->Render(portionOfSpritesheet, destRect, SDL_FLIP_NONE);
	}
}

void PowBlock::TakeAHit()
{
	mNumberOfHitsLeft--;

	if (mNumberOfHitsLeft <= 0)
	{
		mNumberOfHitsLeft = 0;
		mLevelMap->ChangeTileAt(8, 7, 0);
		mLevelMap->ChangeTileAt(8, 8, 0);
	}
}

#include "PowBlock.h"
#include <iostream>
#include "Constants.h"

PowBlock::PowBlock(SDL_Renderer* renderer)
{
	mRenderer = renderer;

	std::string imagePath = "Textures/PowBlock.png";
	mSprite = new Sprite(renderer);
	if (!mSprite->LoadFromFile(imagePath.c_str()))
	{
		std::cout << "Failed to load texture: " << imagePath << std::endl;
		return;
	}


	mSingleSpriteWidth = mSprite->GetWidth() / 3;
	mSingleSpriteHeight = mSprite->GetHeight();
	mNumberOfHitsLeft = 3;
	mPosition = Vector2D((SCREEN_WIDTH * 0.5) - mSingleSpriteWidth * 0.5f, 260);
}

PowBlock::~PowBlock()
{
	mRenderer = NULL;

	delete mSprite;
	mSprite = NULL;
}

void PowBlock::Render(int camX, int camY)
{
	if (mNumberOfHitsLeft > 0)
	{
		// Get the position of the spritesheet
		int left = mSingleSpriteHeight * (mNumberOfHitsLeft - 1);

		// Set Rect
		SDL_Rect portionOfSpritesheet = { left, 0, mSingleSpriteWidth, mSingleSpriteHeight };

		// Draw
		mSprite->Render(mPosition.x - camX, mPosition.y - camY, &portionOfSpritesheet, 0.0, nullptr, SDL_FLIP_NONE);
	}
}

void PowBlock::TakeAHit()
{
	mNumberOfHitsLeft--;

	if (mNumberOfHitsLeft <= 0)
	{
		mNumberOfHitsLeft = 0;
	}
}

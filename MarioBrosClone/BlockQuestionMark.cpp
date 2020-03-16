#include "BlockQuestionMark.h"
#include <iostream>
#include "Collisions.h"

BlockQuestionMark::BlockQuestionMark(SDL_Renderer* renderer, std::string imagePath, BlockType type, Vector2D pos) : Block(renderer, imagePath, type, pos)
{
	mSingleSpriteWidth = mSprite->GetWidth() / 2;
	mAvailable = true;
	coinSpawned = false;
}

BlockQuestionMark::~BlockQuestionMark()
{
	delete mRenderer;
	mRenderer = nullptr;

	delete mSprite;
	mSprite = nullptr;
}

void BlockQuestionMark::Update(float deltaTime, SDL_Event e)
{
	if (!IsAvailable())
	{
		if (!coinSpawned)
		{
			coinSpawned = true;

		}
	}
}

void BlockQuestionMark::Render(int camX, int camY)
{
	int left = 0;

	if (IsAvailable())
		left = 0;
	else left = mSingleSpriteWidth;

	SDL_Rect portionOfSpritesheet = { left, 0, mSingleSpriteWidth, mSingleSpriteHeight };

	mSprite->Render(mPosition.x - camX, mPosition.y - camY, &portionOfSpritesheet, 0.0, nullptr, SDL_FLIP_NONE);
}



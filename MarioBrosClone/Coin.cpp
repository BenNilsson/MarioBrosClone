#include "Coin.h"

Coin::Coin(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition)
{
	mRenderer = renderer;
	mSprite = new Sprite(mRenderer);
	if (!mSprite->LoadFromFile(imagePath))
		std::cout << "Could not load character image file";

	mPosition = startPosition;

	mFrame = 1;
	mFrameCount = 3;

	mSingleSpriteWidth = mSprite->GetWidth() / 3;
	mSingleSpriteHeight = mSprite->GetHeight();

	mCollisionRadius = 20.0f;
}

Coin::~Coin()
{
	delete mRenderer;
	mRenderer = nullptr;

	delete mSprite;
	mSprite = nullptr;
}

void Coin::Render()
{

	int left = mSingleSpriteHeight * (mFrame - 1);

	SDL_Rect portionOfSpritesheet = { left, 0, mSingleSpriteWidth, mSingleSpriteHeight };

	SDL_Rect destRect = { (int)(mPosition.x), (int)(mPosition.y), mSingleSpriteWidth, mSingleSpriteHeight };

	mSprite->Render(portionOfSpritesheet, destRect, SDL_FLIP_NONE);
}

void Coin::Update(float deltaTime, SDL_Event e)
{
	mCurFrameTime += deltaTime * 10;

	if (mCurFrameTime > 2)
	{
		mFrame++;

		if (mFrame > mFrameCount)
			mFrame = 1;

		mCurFrameTime = 0;
	}
}

void Coin::SetPosition(Vector2D newPosition)
{
	mPosition = newPosition;
}

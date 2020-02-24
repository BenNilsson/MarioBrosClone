#include "Coin.h"

Coin::Coin(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition, LevelMap* map)
{
	mRenderer = renderer;
	mTexture = new Texture2D(mRenderer);
	if (!mTexture->LoadFromFile(imagePath))
		std::cout << "Could not load character image file";

	mPosition = startPosition;
	mCurrentLevelMap = map;

	mFrame = 1;
	mFrameCount = 3;

	mSingleSpriteWidth = mTexture->GetWidth() / 3;
	mSingleSpriteHeight = mTexture->GetHeight();
}

Coin::~Coin()
{
	delete mRenderer;
	mRenderer = nullptr;

	delete mTexture;
	mTexture = nullptr;
}

void Coin::Render()
{

	int left = mSingleSpriteHeight * (mFrame - 1);

	SDL_Rect portionOfSpritesheet = { left, 0, mSingleSpriteWidth, mSingleSpriteHeight };

	SDL_Rect destRect = { (int)(mPosition.x), (int)(mPosition.y), mSingleSpriteWidth, mSingleSpriteHeight };

	mTexture->Render(portionOfSpritesheet, destRect, SDL_FLIP_NONE);
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

#pragma once
#include "SDL.h"
#include "Commons.h"
#include "Sprite.h"
#include "LevelMap.h"

class PowBlock
{
public:
	PowBlock(SDL_Renderer* renderer, LevelMap* map);
	~PowBlock();
	void Render();
	Rect2D GetCollisionBox() { return Rect2D(mPosition.x, mPosition.y, mSingleSpriteWidth, mSingleSpriteHeight); }
	void TakeAHit();
	bool IsAvailable() { return mNumberOfHitsLeft > 0; }

private:
	SDL_Renderer* mRenderer;
	Vector2D mPosition;
	Sprite* mTexture;
	LevelMap* mLevelMap;

	float mSingleSpriteWidth;
	float mSingleSpriteHeight;

	int mNumberOfHitsLeft;
};

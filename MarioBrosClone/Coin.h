#pragma once
#include "SDL.h"
#include <iostream>
#include "Commons.h"
#include "LevelMap.h"
#include "Texture2D.h"

class Coin
{
public:
	Coin(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition, LevelMap* map);
	~Coin();

	void Render();
	void Update(float deltaTime, SDL_Event e);

	Vector2D GetPosition() { return mPosition; }
	float GetCollisionRadius() { return mCollisionRadius; }

	void SetPosition(Vector2D newPosition);

private:
	SDL_Renderer* mRenderer;
	Vector2D mPosition;
	Texture2D* mTexture;
	LevelMap* mCurrentLevelMap;

	float mCollisionRadius;

	int mFrame;
	float mCurFrameTime;
	int mFrameCount;

	float mSingleSpriteWidth;
	float mSingleSpriteHeight;

};


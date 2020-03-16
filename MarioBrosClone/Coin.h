#pragma once
#include "SDL.h"
#include <iostream>
#include "Commons.h"
#include "Sprite.h"

class Coin
{
public:
	Coin(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition);
	~Coin();

	void Render(int camX, int camY);
	void Update(float deltaTime, SDL_Event e);

	Vector2D GetPosition() { return mPosition; }
	float GetCollisionRadius() { return mCollisionRadius; }

	void SetPosition(Vector2D newPosition);

private:
	SDL_Renderer* mRenderer;
	Vector2D mPosition;
	Sprite* mSprite;

	float mCollisionRadius;

	int mFrame;
	float mCurFrameTime;
	int mFrameCount;

	float mSingleSpriteWidth;
	float mSingleSpriteHeight;

};


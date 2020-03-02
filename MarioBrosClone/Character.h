#pragma once
#include "SDL.h"
#include <iostream>
#include "Commons.h"
#include "TileMap.h"

class Sprite;

class Character
{
public:
	Character(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition, TileMap* map);
	~Character();

	virtual void Render();
	virtual void Update(float deltaTime, SDL_Event e);
	
	void Jump();
	void AddGravity(float deltaTime);
	void SetPosition(Vector2D newPosition);
	void SetMovementSpeed(float speed);
	Vector2D GetPosition();
	float GetCollisionRadius();
	Rect2D GetCollisionBox() { return Rect2D(mPosition.x, mPosition.y, mSingleSpriteWidth, mSingleSpriteHeight); }
	bool IsJumping() { return mJumping; }
	void CancelJump();
	void SetCanJump(bool jump);
	void SetAlive(bool boolean);
	bool GetAlive() { return mAlive; }
	void SetCanMove(bool move);
	bool GetCanMove() { return mCanMove; }

	float GetWidth() { return mSingleSpriteWidth; }
	float GetHeight() { return mSingleSpriteHeight; }
protected:
	SDL_Renderer* mRenderer;
	Vector2D mPosition;
	Sprite* mSprite;

	TileMap* mCurrentTileMap;

	virtual void MoveLeft(float deltaTime);
	virtual void MoveRight(float deltaTime);
	virtual void UpdateFrame(float deltaTime);
	
	float mCollisionRadius;

	FACING mfacingDirection;
	bool mMovingLeft;
	bool mMovingRight;

	bool mAlive;

	float mSingleSpriteWidth;
	float mSingleSpriteHeight;

	int frame;
	float curFrameTime;
	int frameCount;
	int jumpFrame;

	bool mJumping;
	bool mCanJump;
	float mJumpForce;
	float movementSpeed = 220.0f;
	bool mCanMove;

};
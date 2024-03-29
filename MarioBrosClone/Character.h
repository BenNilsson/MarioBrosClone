#pragma once
#include "SDL.h"
#include "Commons.h"


#include <iostream>

class Sprite;
class TileMap;

class Character
{
public:
	Character(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition, TileMap* map);
	~Character();

	virtual void Render(int x, int y);
	virtual void Update(float deltaTime, SDL_Event e);
	
	void Jump();
	void JumpSmall();
	void AddGravity(float deltaTime);
	void SetPosition(Vector2D newPosition);
	void SetMovementSpeed(float speed);
	Vector2D GetPosition();
	float GetCollisionRadius();
	Rect2D GetCollisionBox() { return Rect2D(mPosition.x, mPosition.y, mSingleSpriteWidth, mSingleSpriteHeight); }
	bool IsJumping() { return mJumping; }
	void CancelJump();
	void SetCanJump(bool jump);
	bool IsAlive() { return mAlive; }
	void SetAlive(bool boolean);
	bool GetAlive() { return mAlive; }
	void SetCanMove(bool move);
	bool CanJump() { return mCanJump; }
	bool GetCanMove() { return mCanMove; }
	bool IsFalling() { return mVelocityDelta.y == -1; }
	FACING GetFacingDirection() { return mfacingDirection; }
	

	float GetWidth() { return mSingleSpriteWidth; }
	float GetHeight() { return mSingleSpriteHeight; }

	TileMap* mCurrentTileMap;
protected:
	SDL_Renderer* mRenderer;
	Vector2D mPosition;
	Sprite* mSprite;

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

	Vector2D mVelocityDelta;

};
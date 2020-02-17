#pragma once
#include "SDL.h"
#include <iostream>
#include "Commons.h"
#include "LevelMap.h"

class Texture2D;

class Character
{
public:
	Character(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition, LevelMap* map);
	~Character();

	virtual void Render();
	virtual void Update(float deltaTime, SDL_Event e);
	
	void Jump();
	void AddGravity(float deltaTime);
	void SetPosition(Vector2D newPosition);
	void SetMovementSpeed(float speed);
	Vector2D GetPosition();
	float GetCollisionRadius();
	Rect2D GetCollisionBox();
	bool IsJumping() { return mJumping; }
	void CancelJump();
protected:
	SDL_Renderer* mRenderer;
	Vector2D mPosition;
	Texture2D* mTexture;

	virtual void MoveLeft(float deltaTime);
	virtual void MoveRight(float deltaTime);
	
	float mCollisionRadius;

	FACING mfacingDirection;
	bool mMovingLeft;
	bool mMovingRight;

private:
	bool mJumping;
	bool mCanJump;
	float mJumpForce;
	float movementSpeed = 220.0f;

	LevelMap* mCurrentLevelMap;
};
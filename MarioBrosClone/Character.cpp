#include "Character.h"
#include "Texture2D.h"
#include "Constants.h"
#include <iostream>

Character::Character(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition, LevelMap* map)
{
	mRenderer = renderer;
	mTexture = new Texture2D(mRenderer);
	if (!mTexture->LoadFromFile(imagePath))
		std::cout << "Could not load character image file";
	mPosition = startPosition;
	mfacingDirection = FACING_RIGHT;
	mMovingLeft = false;
	mMovingRight = false;
	mJumping = false;
	mCollisionRadius = 15.0f;
	mCurrentLevelMap = map;
}

Character::~Character()
{
	delete mRenderer;
	mRenderer = nullptr;

	delete mTexture;
	mTexture = nullptr;
}

void Character::Render()
{
	if (mfacingDirection == FACING_RIGHT)
		mTexture->Render(mPosition, SDL_FLIP_NONE);
	else
		mTexture->Render(mPosition, SDL_FLIP_HORIZONTAL);
}

void Character::Update(float deltaTime, SDL_Event e)
{
	// Apply gravity
	//AddGravity(deltaTime);

	// Collision position variables
	int centralXPosition = (int)(mPosition.x + (mTexture->GetWidth() * 0.5f)) / TILE_WIDTH;
	int footPosition = (int)(mPosition.y + mTexture->GetHeight()) / TILE_HEIGHT;

	// Check for empty tile, if so apply gravity, if not empty so canJump to true
	if (mCurrentLevelMap->GetTileAt(footPosition, centralXPosition) == 0)
		AddGravity(deltaTime);
	else
		mCanJump = true;

	// Jumping
	if (mJumping)
	{
		// Adjust position
		mPosition.y -= mJumpForce * deltaTime;

		// Reduce jump force
		mJumpForce -= JUMP_FORCE_DECREMENT * deltaTime;

		if (mJumpForce <= 0.0f)
			mJumping = false;
	}

	// Control movement
	if (mMovingLeft)
		MoveLeft(deltaTime);
	else if (mMovingRight)
		MoveRight(deltaTime);
}

void Character::Jump()
{
	if (!mJumping && mCanJump)
	{
		mJumpForce = INITIAL_JUMP_FORCE;
		mJumping = true;
		mCanJump = false;
	}
}

void Character::AddGravity(float deltaTime)
{
	mPosition.y += gravityForce * deltaTime;
	mCanJump = false;
}

void Character::SetPosition(Vector2D newPosition)
{
	mPosition = newPosition;
}

void Character::SetMovementSpeed(float speed)
{
	movementSpeed = speed;
}

Vector2D Character::GetPosition()
{
	return mPosition;
}

float Character::GetCollisionRadius()
{
	return mCollisionRadius;
}

Rect2D Character::GetCollisionBox()
{
	return Rect2D(mPosition.x, mPosition.y, mTexture->GetWidth(), mTexture->GetHeight());
}

void Character::MoveLeft(float deltaTime)
{
	mfacingDirection = FACING_LEFT;
	mPosition.x -= movementSpeed * deltaTime;
}

void Character::MoveRight(float deltaTime)
{
	mfacingDirection = FACING_RIGHT;
	mPosition.x += movementSpeed * deltaTime;
}

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
	mMovingLeft = false;
	mMovingRight = false;
	mJumping = false;
	mCollisionRadius = 15.0f;
	mCurrentLevelMap = map;

	frame = 1;
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
	// Get the position of the spritesheet
	int left = mSingleSpriteHeight * (frame - 1);

	// Set Rect
	SDL_Rect portionOfSpritesheet = { left, 0, mSingleSpriteWidth, mSingleSpriteHeight };

	// Determine where to draw
	SDL_Rect destRect = { (int)(mPosition.x), (int)(mPosition.y), mSingleSpriteWidth, mSingleSpriteHeight };

	// Draw
	if (mfacingDirection == FACING::FACING_RIGHT)
		mTexture->Render(portionOfSpritesheet, destRect, SDL_FLIP_NONE);
	else
		mTexture->Render(portionOfSpritesheet, destRect, SDL_FLIP_HORIZONTAL);
}

void Character::Update(float deltaTime, SDL_Event e)
{
	// Update Frame
	UpdateFrame(deltaTime);

	// Collision position variables
	int centralXPosition = (int)(mPosition.x + (mSingleSpriteWidth * 0.5f)) / TILE_WIDTH;
	int footPosition = (int)(mPosition.y + mSingleSpriteHeight) / TILE_HEIGHT;

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

void Character::CancelJump()
{
	mJumpForce = 0.0f;
}

void Character::SetAlive(bool boolean)
{
	mAlive = boolean;
}

void Character::MoveLeft(float deltaTime)
{
	mfacingDirection = FACING::FACING_LEFT;
	mPosition.x -= movementSpeed * deltaTime;
}

void Character::MoveRight(float deltaTime)
{
	mfacingDirection = FACING::FACING_RIGHT;
	mPosition.x += movementSpeed * deltaTime;
}

void Character::UpdateFrame(float deltaTime)
{
	// Update frame counter

	// Jumping
	if (IsJumping() || !mCanJump)
	{
		frame = jumpFrame;
	}
	// moving
	else if (mMovingRight || mMovingLeft)
	{
		curFrameTime += deltaTime * 10;

		if (curFrameTime > 1)
		{
			frame++;

			if (frame > frameCount)
				frame = 1;

			curFrameTime = 0;
		}
	}
	else
	{
		frame = 1;
		curFrameTime = 0;
	}
}

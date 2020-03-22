#include "Character.h"
#include "Sprite.h"
#include "Constants.h"
#include <iostream>
#include "SoundManager.h"
#include "Collisions.h"
#include "Camera.h"
#include "TileMap.h"

Character::Character(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition, TileMap* map)
{
	mRenderer = renderer;
	mCurrentTileMap = map;
	mSprite = new Sprite(mRenderer);
	if (!mSprite->LoadFromFile(imagePath))
		std::cout << "Could not load character image file";

	mPosition = startPosition;
	mMovingLeft = false;
	mMovingRight = false;
	mJumping = false;
	mCanMove = true;
	mCanJump = true;
	mAlive = true;
	mCollisionRadius = 15.0f;
	mfacingDirection = FACING::FACING_RIGHT;
	mVelocityDelta = Vector2D(0, 0);
	frame = 1;

}

Character::~Character()
{
	delete mRenderer;
	mRenderer = nullptr;

	delete mSprite;
	mSprite = nullptr;
}

void Character::Render(int camX, int camY)
{
	if (!mAlive) return;

	// Get the position of the spritesheet
	int left = mSingleSpriteWidth * (frame - 1);

	// Set Rect
	SDL_Rect portionOfSpritesheet = { left, 0, mSingleSpriteWidth, mSingleSpriteHeight };

	// Draw
	if (mfacingDirection == FACING::FACING_RIGHT)
	{
		mSprite->Render(mPosition.x - camX, mPosition.y - camY, &portionOfSpritesheet, 0.0, nullptr, SDL_FLIP_NONE);
		
	}
	else
	{
		mSprite->Render(mPosition.x - camX, mPosition.y - camY, &portionOfSpritesheet, 0.0, nullptr, SDL_FLIP_HORIZONTAL);
	}
}

void Character::Update(float deltaTime, SDL_Event e)
{
	if (!mAlive) return;

	if (mPosition.y >= 438)
	{
		mAlive = false;
	}

	Vector2D old_pos = GetPosition();
	Vector2D new_pos = old_pos;
	bool gravity = false;
	mCanMove = true;

	if(mMovingLeft)
		new_pos.x -= movementSpeed * deltaTime;
	else if (mMovingRight)
		new_pos.x += movementSpeed * deltaTime;

	new_pos.y += gravityForce * deltaTime;

	int centralXPositionInGrid = (int)(mPosition.x + (mSingleSpriteWidth * 0.50f)) / 32;
	int centralYPositionInGrid = (int)(mPosition.y + (mSingleSpriteHeight * 0.50f)) / 32;
	int footPositionInGrid = (int)(mPosition.y + (mSingleSpriteHeight)) / 32;
	int headPositionInGrid = (int)(mPosition.y) / 32;

	int rightSidePositionInGrid = (int)(mPosition.x + mSingleSpriteWidth) / 32;
	int leftSidePositionInGrid = (int)mPosition.x / 32;

	// Get position in grid and add gravity based on the collision detected
	if (mCurrentTileMap != nullptr)
	{
		if (mCurrentTileMap->GetTileAt(centralXPositionInGrid, footPositionInGrid) == nullptr)
		{
			// Asumming there is no tile, we are in air
			AddGravity(deltaTime);
		}
		else
		{
			mCanJump = true;
			mVelocityDelta.y = 1;
		}

		// Check if the head bumps into a block
		if (mCurrentTileMap->GetTileAt(centralXPositionInGrid, headPositionInGrid) != nullptr)
		{
			if (mCurrentTileMap->GetTileAt(centralXPositionInGrid, headPositionInGrid)->GetCollisionType() == CollisionType::TILE_SOLID)
			{
				// Play sound
				soundmanager::SoundManager::GetInstance()->PlaySFX("SFX/bump.wav");

				// If the tile is solid, push the player down
				if (mJumping)
					mJumping = false;
				mPosition.y += 10;
			}
		}
		
		// Sideway collision
		if (mfacingDirection == FACING::FACING_RIGHT)
		{
			if (mCurrentTileMap->GetTileAt(rightSidePositionInGrid, centralYPositionInGrid) != nullptr)
			{
				// If the right side collides with a solid tile, stop movement
				if (mCurrentTileMap->GetTileAt(rightSidePositionInGrid, centralYPositionInGrid)->GetCollisionType() == CollisionType::TILE_SOLID)
				{
					mCanMove = false;
					mPosition = Vector2D(old_pos.x - 1, old_pos.y);
				}
			}
		}
		else if (mfacingDirection == FACING::FACING_LEFT)
		{
			if (mCurrentTileMap->GetTileAt(leftSidePositionInGrid, centralYPositionInGrid) != nullptr)
			{
				// If the left side collides with a solid tile, stop movement
				if (mCurrentTileMap->GetTileAt(leftSidePositionInGrid, centralYPositionInGrid)->GetCollisionType() == CollisionType::TILE_SOLID)
				{
					mCanMove = false;
					mPosition = Vector2D(old_pos.x + 1, old_pos.y);
				}
			}
		}
	}

	// Update Frame
	UpdateFrame(deltaTime);

	// Control movement
	if (mCanMove)
	{
		if (mMovingLeft)
			MoveLeft(deltaTime);
			
		else if (mMovingRight)
			MoveRight(deltaTime);
	}

	// Jumping
	if (mJumping)
	{
		// Adjust position
		mPosition.y -= mJumpForce * deltaTime;

		// Reduce jump force
		mJumpForce -= JUMP_FORCE_DECREMENT * deltaTime;


		if (mJumpForce <= 0.0f)
		{
			mJumpForce = 0.0f;
			mJumping = false;
		}
	}

}

void Character::Jump()
{
	if (!mJumping && mCanJump)
	{
		soundmanager::SoundManager::GetInstance()->PlaySFX("SFX/mario-jump.wav");
		mJumpForce = INITIAL_JUMP_FORCE;
		mJumping = true;
		mCanJump = false;
	}
}

void Character::JumpSmall()
{
	mJumpForce = INITIAL_JUMP_FORCE / 1.5;
	mJumping = true;
	mCanJump = false;
}

void Character::AddGravity(float deltaTime)
{
	if(!IsJumping()) mVelocityDelta.y = -1;
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

void Character::SetCanJump(bool jump)
{
	mCanJump = jump;
}

void Character::SetAlive(bool boolean)
{
	mAlive = boolean;
}

void Character::SetCanMove(bool move)
{
	mCanMove = move;
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
	if (IsJumping())
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
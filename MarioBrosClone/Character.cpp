#include "Character.h"
#include "Sprite.h"
#include "Constants.h"
#include <iostream>
#include "SoundManager.h"
#include "Collisions.h"

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
	mCollisionRadius = 15.0f;

	frame = 1;
}

Character::~Character()
{
	delete mRenderer;
	mRenderer = nullptr;

	delete mSprite;
	mSprite = nullptr;
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
		mSprite->Render(portionOfSpritesheet, destRect, SDL_FLIP_NONE);
	else
		mSprite->Render(portionOfSpritesheet, destRect, SDL_FLIP_HORIZONTAL);
}

void Character::Update(float deltaTime, SDL_Event e)
{
	Vector2D old_pos = GetPosition();
	Vector2D new_pos = old_pos;
	bool gravity = true;

	if(mMovingLeft)
		new_pos.x -= movementSpeed * deltaTime;
	else if (mMovingRight)
		new_pos.x += movementSpeed * deltaTime;

	new_pos.y += gravityForce * deltaTime;

	// Check through the tilemap and ensure collision happens
	for (unsigned int i = 0; i < mCurrentTileMap->mTileMap.size(); i++)
	{
		if (mCurrentTileMap->mTileMap[i]->GetCollisionType() == CollisionType::TILE_NONWALKABLE)
		{
			// Check if the new position is colliding with the player
			if (Collisions::Instance()->Box(Rect2D(mCurrentTileMap->mTileMap[i]->GetPosition().x, mCurrentTileMap->mTileMap[i]->GetPosition().y, mCurrentTileMap->mTileMap[i]->width, 2),
				Rect2D(new_pos.x, new_pos.y + GetHeight() - 2, GetWidth(), 1)))
			{
				// Collision!
				mPosition = old_pos;
				gravity = false;
				if(!mJumping)
					mCanJump = true;
				break;
			}
		}
	}

	// Update Frame
	UpdateFrame(deltaTime);
	
	if(gravity)
		AddGravity(deltaTime);



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
			mJumping = false;

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

void Character::AddGravity(float deltaTime)
{
	mPosition.y += gravityForce * deltaTime;
	//mCanJump = false;
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

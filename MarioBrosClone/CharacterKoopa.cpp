#include "SDL.h"
#include "Character.h"
#include "CharacterKoopa.h"
#include "Texture2D.h"

CharacterKoopa::CharacterKoopa(SDL_Renderer* renderer, std::string imagePath, Vector2D startPos, LevelMap* map, float speed, FACING direction) : Character(renderer, imagePath, startPos, map)
{
	mInjured = false;
	movementSpeed = speed;
	mfacingDirection = direction;
	mSingleSpriteWidth = mTexture->GetWidth() / 6;
	mSingleSpriteHeight = mTexture->GetHeight();
	mAlive = true;
	jumpFrame = 5;
	frameCount = 4;
}

CharacterKoopa::~CharacterKoopa()
{
	delete mRenderer;
	mRenderer = nullptr;

	delete mTexture;
	mTexture = nullptr;
}

void CharacterKoopa::TakeDamage()
{
	mInjured = true;
	mInjuredTime = 2.5f;
	Jump();
}

void CharacterKoopa::Jump()
{
	if (!mJumping) 
	{
		mJumpForce = INITIAL_JUMP_FORCE_SMALL;

		mJumping = true;
		mCanJump = false;
	}
}

void CharacterKoopa::Update(float deltaTime, SDL_Event e)
{
	if (!mInjured)
	{
		// Not injured, do movement
		if (mfacingDirection == FACING::FACING_LEFT)
		{
			mMovingLeft = true;
			mMovingRight = false;
		}
		else if (mfacingDirection == FACING::FACING_RIGHT)
		{
			mMovingRight = true;
			mMovingLeft = false;
		}
	}
	else
	{
		// Injured, do not move
		mMovingRight = false;
		mMovingLeft = false;

		// Count down injured time
		mInjuredTime -= deltaTime;

		// Reset injured state when time has ran out
		if (mInjuredTime <= 0.0)
			Flip();
	}

	// Call charcacter's update function
	Character::Update(deltaTime, e);
	
}

void CharacterKoopa::Render()
{	
	// Variable to hold the left position of the sprite we want to draw
	int left = mSingleSpriteWidth * (frame - 1);

	// Inf ionjured, move the left position to the second image of the spirte sheet
	if (mInjured)
		left = mSingleSpriteWidth * jumpFrame;

	// Get portion of spritesheet to draw
	SDL_Rect portionOfSpritesheet = { left, 0, mSingleSpriteWidth, mSingleSpriteHeight };

	// Determine where to draw
	SDL_Rect destRect = { (int)(mPosition.x), (int)(mPosition.y), mSingleSpriteWidth, mSingleSpriteHeight };

	if (mfacingDirection == FACING::FACING_RIGHT)
	{
		mTexture->Render(portionOfSpritesheet, destRect, SDL_FLIP_NONE);
	}
	else
	{
		mTexture->Render(portionOfSpritesheet, destRect, SDL_FLIP_HORIZONTAL);
	}
	
	
}

void CharacterKoopa::Flip()
{
	if (mfacingDirection == FACING::FACING_LEFT) mfacingDirection = FACING::FACING_RIGHT;
	else if (mfacingDirection == FACING::FACING_RIGHT) mfacingDirection = FACING::FACING_LEFT;

	mInjured = false;
	Jump();

}

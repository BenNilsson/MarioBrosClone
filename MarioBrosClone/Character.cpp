#include "Character.h"
#include "Texture2D.h"
#include "Constants.h"
#include <iostream>

Character::Character(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition)
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
}

Character::~Character()
{
	mRenderer = nullptr;
}

void Character::Render()
{
	if (mfacingDirection == FACING_RIGHT)
	{
		mTexture->Render(mPosition, SDL_FLIP_NONE);
	}
	else
	{
		mTexture->Render(mPosition, SDL_FLIP_HORIZONTAL);
	}
}

void Character::Update(float deltaTime, SDL_Event e)
{
	// Apply gravity
	AddGravity(deltaTime);

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

	// Handle events
	switch (e.type)
	{
	// KeyDown
	case SDL_KEYDOWN:
		// Get Key Pressed
		switch (e.key.keysym.sym)
		{
		// If user hits left arrow
		case(SDLK_LEFT):
			mMovingLeft = true;
			break;
		// If user hits left arrow
		case(SDLK_RIGHT):
			mMovingRight = true;
			break;
		// If the user hits the up arrow
		case(SDLK_UP):
			Jump();
			break;
		}
	break;
	// KeyUp
	case SDL_KEYUP:
		// get Key Pressed
		switch (e.key.keysym.sym)
		{
			// If user hits left arrow
		case(SDLK_LEFT):
			mMovingLeft = false;
			break;
			// If user hits left arrow
		case(SDLK_RIGHT):
			mMovingRight = false;
			break;
		}
		break;
	}
}

void Character::Jump()
{
	if (!mJumping)
	{
		mJumpForce = INITIAL_JUMP_FORCE;
		mJumping = true;
		mCanJump = false;
	}
}

void Character::AddGravity(float deltaTime)
{
	if (mPosition.y < (SCREEN_HEIGHT - 42)) mPosition.y += gravityValue * deltaTime;
	// Else colided with ground, set jump to true
	else if (mPosition.y > (SCREEN_HEIGHT - 42)) mCanJump = true;
}

void Character::SetPosition(Vector2D newPosition)
{
	mPosition = newPosition;
}

Vector2D Character::GetPosition()
{
	return mPosition;
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

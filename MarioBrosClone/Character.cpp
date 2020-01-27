#include "Character.h"
#include "Texture2D.h"
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
	mPosition.x -= 220 * deltaTime;
}

void Character::MoveRight(float deltaTime)
{
	mfacingDirection = FACING_RIGHT;
	mPosition.x += 220 * deltaTime;
}

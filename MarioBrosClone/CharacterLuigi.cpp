#include "SDL.h"
#include "Character.h"
#include "CharacterLuigi.h"
#include "Sprite.h"

CharacterLuigi::CharacterLuigi(SDL_Renderer* renderer, std::string imagePath, Vector2D startPos, TileMap* map) : Character(renderer, imagePath, startPos, map)
{
	mRenderer = renderer;

	mSingleSpriteWidth = mSprite->GetWidth() / 6;
	mSingleSpriteHeight = mSprite->GetHeight();
	jumpFrame = 6;
	frameCount = 4;
}

CharacterLuigi::~CharacterLuigi()
{
	delete mRenderer;
	mRenderer = nullptr;

	delete mSprite;
	mSprite = nullptr;
}

void CharacterLuigi::Update(float deltaTime, SDL_Event e)
{
	Character::Update(deltaTime, e);

	// Handle events
	switch (e.type)
	{
		// KeyDown
	case SDL_KEYDOWN:
		// Get Key Pressed
		switch (e.key.keysym.sym)
		{
			// If user hits left arrow
		case(SDLK_a):
			mMovingLeft = true;
			break;
			// If user hits left arrow
		case(SDLK_d):
			mMovingRight = true;
			break;
			// If the user hits the up arrow
		case(SDLK_w):
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
		case(SDLK_a):
			mMovingLeft = false;
			break;
			// If user hits left arrow
		case(SDLK_d):
			mMovingRight = false;
			break;
		}
		break;
	}
}

void CharacterLuigi::Render()
{
	Character::Render();
}



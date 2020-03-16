#include "SDL.h"
#include "Character.h"
#include "CharacterMario.h"
#include "Sprite.h"
#include "Camera.h"

CharacterMario::CharacterMario(SDL_Renderer* renderer, std::string imagePath, Vector2D startPos, TileMap* map) : Character(renderer, imagePath, startPos, map)
{
	mRenderer = renderer;

	mSingleSpriteWidth = mSprite->GetWidth() / 6;
	mSingleSpriteHeight = mSprite->GetHeight();
	jumpFrame = 6;
	frameCount = 4;
}

CharacterMario::~CharacterMario()
{
	delete mRenderer;
	mRenderer = nullptr;

	delete mSprite;
	mSprite = nullptr;
}

void CharacterMario::Update(float deltaTime, SDL_Event e)
{

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

	Character::Update(deltaTime, e);
}

void CharacterMario::Render()
{
	Character::Render(Camera::GetInstance()->GetPosition().x, Camera::GetInstance()->GetPosition().y);
}



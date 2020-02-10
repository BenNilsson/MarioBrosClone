#include "SDL.h"
#include "Character.h"
#include "CharacterMario.h"

CharacterMario::CharacterMario(SDL_Renderer* renderer, std::string imagePath, Vector2D startPos, LevelMap* map) : Character(renderer, imagePath, startPos, map)
{
	mRenderer = renderer;
}

CharacterMario::~CharacterMario()
{
	delete mRenderer;
	mRenderer = nullptr;

	delete mTexture;
	mTexture = nullptr;
}

void CharacterMario::Update(float deltaTime, SDL_Event e)
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

void CharacterMario::Render()
{
	Character::Render();
}



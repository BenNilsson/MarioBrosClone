#include "GameScreenIntro.h"
#include <iostream>
#include "Texture2D.h"

GameScreenIntro::GameScreenIntro(SDL_Renderer* renderer) : GameScreen(renderer)
{
	SetUpLevel();
}

GameScreenIntro::~GameScreenIntro()
{
	delete mBackgroundTexture;
	mBackgroundTexture = nullptr;
}

void GameScreenIntro::Render()
{
	// Draw the background
	mBackgroundTexture->Render(Vector2D(), SDL_FLIP_NONE);
}

void GameScreenIntro::Update(float deltaTime, SDL_Event e)
{
}

bool GameScreenIntro::SetUpLevel()
{
	// Load the background texture
	mBackgroundTexture = new Texture2D(mRenderer);
	if (!mBackgroundTexture->LoadFromFile("Textures/startMenu.png"))
	{
		std::cout << "Failed to load background texture!";
		return false;
	}

	return true;
}

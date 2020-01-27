#include "GameSceenLevel1.h"
#include <iostream>
#include "Texture2D.h"

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer) : GameScreen(renderer)
{
	SetUpLevel();
}

GameScreenLevel1::~GameScreenLevel1()
{
	delete mBackgroundTexture;
	mBackgroundTexture = nullptr;
}

void GameScreenLevel1::Render()
{
	// Draw the background
	mBackgroundTexture->Render(Vector2D(), SDL_FLIP_NONE);
}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e)
{
}

bool GameScreenLevel1::SetUpLevel()
{
	// Load the background texture
	mBackgroundTexture = new Texture2D(mRenderer);
	if (!mBackgroundTexture->LoadFromFile("Textures/Test.bmp"))
	{
		std::cout << "Failed to load background texture!";
		return false;
	}

	return true;
}

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

	delete myCharacter;
	myCharacter = nullptr;
}

void GameScreenLevel1::Render()
{
	// Draw the background
	mBackgroundTexture->Render(Vector2D(), SDL_FLIP_NONE);

	// Draw the player
	myCharacter->Render();
}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e)
{
	// Update the player
	myCharacter->Update(deltaTime, e);
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

	// Set up the player character
	myCharacter = new Character(mRenderer, "Textures/Mario.png", Vector2D(64, 330));

	return true;
}

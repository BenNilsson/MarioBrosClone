#include "GameSceenLevel1.h"
#include <iostream>
#include "Texture2D.h"
#include "Collisions.h"

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer) : GameScreen(renderer)
{
	SetUpLevel();
}

GameScreenLevel1::~GameScreenLevel1()
{
	delete mBackgroundTexture;
	mBackgroundTexture = nullptr;

	delete characterMario;
	characterMario = nullptr;

	delete characterLuigi;
	characterLuigi = nullptr;
}

void GameScreenLevel1::Render()
{
	// Draw the background
	mBackgroundTexture->Render(Vector2D(), SDL_FLIP_NONE);

	// Draw the player
	characterMario->Render();
	characterLuigi->Render();
}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e)
{
	// Update the player
	characterMario->Update(deltaTime, e);
	characterLuigi->Update(deltaTime, e);

	// Check collisions on players
	if (Collisions::Instance()->Circle(Circle2D(characterMario->GetCollisionRadius(), characterMario->GetPosition()),
		Circle2D(characterLuigi->GetCollisionRadius(), characterLuigi->GetPosition())))
	{
		std::cout << "Players collided with each other" << std::endl;
	}
}

bool GameScreenLevel1::SetUpLevel()
{
	// Load the background texture
	mBackgroundTexture = new Texture2D(mRenderer);
	if (!mBackgroundTexture->LoadFromFile("Textures/BackgroundMB.png"))
	{
		std::cout << "Failed to load background texture!";
		return false;
	}

	// Set up the player character
	characterMario = new CharacterMario(mRenderer, "Textures/Mario.png", Vector2D(64, 330));
	characterLuigi = new CharacterLuigi(mRenderer, "Textures/Luigi.png", Vector2D(364, 330));

	return true;
}

#include "GameScreen.h"

GameScreen::GameScreen(SDL_Renderer* renderer)
{
	mRenderer = renderer;
}

GameScreen::~GameScreen()
{
	mRenderer = nullptr;
}

void GameScreen::Render()
{
}

void GameScreen::Update(float deltaTime, SDL_Event e)
{
}

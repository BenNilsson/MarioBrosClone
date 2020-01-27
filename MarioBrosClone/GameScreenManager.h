#pragma once
#include "SDL.h"
#include "Commons.h"
#include "GameSceenLevel1.h"
#include "GameScreenIntro.h"

class GameScreen;

class GameScreenManager
{
public:
	GameScreenManager(SDL_Renderer* renderer, SCREENS startScreen);
	~GameScreenManager();

	void Render();
	void Update(float deltaTime, SDL_Event e);
	void ChangeScreen(SCREENS newScreen);
private:
	SDL_Renderer* mRenderer;
	GameScreen* mCurrentScreen;
	GameScreenLevel1* tempScreen;
	GameScreenIntro* tempScreenIntro;
};
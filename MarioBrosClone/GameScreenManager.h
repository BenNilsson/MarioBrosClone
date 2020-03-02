#pragma once
#include "SDL.h"
#include "Commons.h"
#include "GameSceenLevel1.h"
#include "GameScreenLevel2.h"
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
	SCREENS GetScreenState() { return mScreenState; }
private:

	SDL_Renderer* mRenderer;
	GameScreen* mCurrentScreen;
	SCREENS mScreenState;
	GameScreenLevel1* tempScreen;
	GameScreenLevel2* tempScreenLvl2;
	GameScreenIntro* tempScreenIntro;
};
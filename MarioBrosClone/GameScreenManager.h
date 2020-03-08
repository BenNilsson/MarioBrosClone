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
	GameScreenManager();
	~GameScreenManager();

	void Render();
	void Update(float deltaTime, SDL_Event e);
	void ChangeScreen(SCREENS newScreen);
	void SetRenderer(SDL_Renderer* renderer);
	SCREENS GetScreenState() { return mScreenState; }
	GameScreen* mCurrentScreen;
private:

	SDL_Renderer* mRenderer;
	SCREENS mScreenState;
	GameScreenLevel1* tempScreen;
	GameScreenLevel2* tempScreenLvl2;
	GameScreenIntro* tempScreenIntro;
};
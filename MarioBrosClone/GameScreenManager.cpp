#include "GameScreenManager.h"
#include "GameScreen.h"
#include "GameSceenLevel1.h"
#include "GameScreenLevel2.h"

GameScreenManager::GameScreenManager(SDL_Renderer* renderer, SCREENS startScreen)
{
	mRenderer = renderer;
	mCurrentScreen = NULL;

	// Ensure the first screen is set up
	//ChangeScreen(startScreen);
}

GameScreenManager::~GameScreenManager()
{
	mRenderer = nullptr;
	delete mCurrentScreen;
	mCurrentScreen = nullptr;
}

void GameScreenManager::Render()
{
	// Draw the current screen
	mCurrentScreen->Render();
}

void GameScreenManager::Update(float deltaTime, SDL_Event e)
{
	mCurrentScreen->Update(deltaTime, e);
}

void GameScreenManager::ChangeScreen(SCREENS newScreen)
{
	// Clear up old screen
	if (mCurrentScreen != NULL)
	{
		delete mCurrentScreen;
	}

	switch (newScreen)
	{
	case SCREEN_INTRO:
		tempScreenIntro = new GameScreenIntro(mRenderer);
		mCurrentScreen = (GameScreen*)tempScreenIntro;
		tempScreenIntro = NULL;
		break;
		
	case SCREEN_LEVEL1:
		tempScreen = new GameScreenLevel1(mRenderer);
		mCurrentScreen = (GameScreen*)tempScreen;
		tempScreen = NULL;
		break;

	case SCREEN_LEVEL2:
		tempScreenLvl2 = new GameScreenLevel2(mRenderer);
		mCurrentScreen = (GameScreen*)tempScreenLvl2;
		tempScreenLvl2 = NULL;
		break;

	case SCREEN_WON:
		tempScreenWon = new GameScreenWon(mRenderer);
		mCurrentScreen = (GameScreen*)tempScreenWon;
		tempScreenWon = NULL;
		break;
	}
}

void GameScreenManager::SetRenderer(SDL_Renderer* renderer)
{
	mRenderer = renderer;
}

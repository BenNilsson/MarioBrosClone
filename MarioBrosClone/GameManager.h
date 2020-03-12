#pragma once
#include "GameScreenManager.h"
#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H

class GameManager
{
	// include gamescreenmanager in game manager which is static
	// there I can change screens everywhere

public:

	static GameManager* GetInstance()
	{
		if (instance == 0)
		{
			instance = new GameManager();
			instance->Init();
		}

		return instance;
	}

	enum class GameState
	{
		INTRO = 0,
		INGAME,
		DEAD,
		GAMEOVER,
	};

	void ChangeState(GameManager::GameState state);

	GameState GetState() { return mCurGameState; }
	GameScreenManager* gameScreenManager;

private:
	static GameManager* instance;
	static GameState mCurGameState;


	void Init()
	{
		//gameScreenManager = new GameScreenManager();
	}
};

#endif

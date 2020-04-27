#pragma once
#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H
#include "GameScreenManager.h"

class GameManager
{

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
		WON
	};

	void ChangeState(GameManager::GameState state);
	int GetScore() { return mScore; }
	int GetHighScore() { return mHighscore; }
	void SetScore(int score);
	void AddScore(int add);

	GameState GetState() { return mCurGameState; }
	GameScreenManager* gameScreenManager;

	UIText* mScoreText;
	UIText* mHighscoreText;

	void SaveHighScore();
	int LoadHighScore();

private:
	static GameManager* instance;
	static GameState mCurGameState;

	int mScore;
	int mHighscore;

	void Init()
	{
		mScore = 0;
		mHighscore = LoadHighScore();
	}
};

#endif


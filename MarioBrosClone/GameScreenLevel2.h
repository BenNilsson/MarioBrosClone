#pragma once
#include "GameScreen.h"
#include "Sprite.h"
#include "Character.h"
#include "CharacterMario.h"
#include "CharacterLuigi.h"
#include "ScreenShake.h"

class TileMap;

class GameScreenLevel2 : GameScreen
{

public:
	GameScreenLevel2(SDL_Renderer* renderer);
	~GameScreenLevel2();

	void Render();
	void Update(float deltaTime, SDL_Event e);
private:
	Sprite* mBackgroundTexture;

	TileMap* tileMap;

	CharacterMario* characterMario;
	CharacterLuigi* characterLuigi;

	bool SetUpLevel();
	void SetUpTileMap();

	void CheckForGameOver();
	void CheckForRestart(SDL_Event e);

	void UpdateEnemies(float deltaTime, SDL_Event e);
	void UpdateQuestionMarkBlocks(float deltaTime, SDL_Event e);
	void UpdateCoins(float deltaTime, SDL_Event e);

	void CreateCoin(Vector2D position);

	int enemyIndexToDelete;
	int coinIndexToDelete;


	ScreenShake* screenShake;

	float mLastCamXPos;

	bool levelIsSetup;
};

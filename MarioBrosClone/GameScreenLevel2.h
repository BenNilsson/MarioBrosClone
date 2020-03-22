#pragma once
#include "GameScreen.h"
#include "Sprite.h"
#include "Character.h"
#include "CharacterMario.h"
#include "CharacterLuigi.h"
#include "ScreenShake.h"
#include "UIText.h"

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

	void CheckForGameOver(float deltaTime);
	void CheckForRestart(SDL_Event e);
	bool CheckFlagCollision();

	void UpdateEnemies(float deltaTime, SDL_Event e);
	void UpdateQuestionMarkBlocks(float deltaTime, SDL_Event e);
	void UpdateCoins(float deltaTime, SDL_Event e);
	void HandleViewportCollision();
	void UpdateCameraPosition();

	bool RenderDeathInfo();
	void UpdateDeathInfo(float deltaTime);

	void CreateCoin(Vector2D position);

	int enemyIndexToDelete;
	int coinIndexToDelete;


	ScreenShake* screenShake;

	float mLastCamXPos;

	bool levelIsSetup;

	Sprite* mLogoSprite;
	Sprite* mCoinBigSprite;
	Sprite* mInfoSprite;

	UIText* mYouLostText;

	const float renderLogoAfterTime = 0.50f;
	const float renderWonTextAfterTime = 1.0f;
	const float renderCoinDataAfterTime = 1.50f;
	const float renderInfoAfterTime = 2.0f;

	float mTimeElapsedAfterDeath;
};

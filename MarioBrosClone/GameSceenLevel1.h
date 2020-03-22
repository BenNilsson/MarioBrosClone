#pragma once
#include "SDL.h"
#include "Commons.h"
#include "GameScreen.h"
#include "Character.h"
#include "CharacterMario.h"
#include "CharacterLuigi.h"
#include "CharacterKoopa.h"
#include "PowBlock.h"
#include "ScreenShake.h"
#include "Coin.h"
#include "Flag.h"
#include <vector>
#include "UIText.h"
#include "BlockQuestionMark.h"

class TileMap;
class Sprite;
class CharacterMario;
class PowBlock;

class GameScreenLevel1 : GameScreen
{
public:
	GameScreenLevel1(SDL_Renderer* renderer);
	~GameScreenLevel1();

	void Render();
	void Update(float deltaTime, SDL_Event e);

private:

	TileMap* tileMap;

	Sprite* mBackgroundTexture;
	CharacterMario* characterMario;
	CharacterLuigi* characterLuigi;

	PowBlock* mPowBlock;

	void CheckForGameOver(float deltaTime);
	void CheckForRestart(SDL_Event e);

	void UpdatePowBlock();


	void UpdateEnemies(float deltaTime, SDL_Event e);
	void CreateKoopa(Vector2D position, FACING direction, float speed);

	void HandleViewportCollision();
	void UpdateCameraPosition();

	void CreateCoin(Vector2D position);
	void UpdateCoins(float deltaTime, SDL_Event e);

	void UpdateQuestionMarkBlocks(float deltaTime, SDL_Event e);

	bool RenderDeathInfo();
	void UpdateDeathInfo(float deltaTime);

	bool CheckFlagCollision();

	int enemyIndexToDelete;
	int coinIndexToDelete;

	void SetUpTileMap();
	bool SetUpLevel();

	float mLastCamXPos;

	Sprite* mLogoSprite;
	Sprite* mCoinBigSprite;
	Sprite* mInfoSprite;

	UIText* mYouLostText;

	const float renderLogoAfterTime = 0.50f;
	const float renderWonTextAfterTime = 1.0f;
	const float renderCoinDataAfterTime = 1.50f;
	const float renderInfoAfterTime = 2.0f;

	float mTimeElapsedAfterDeath;

	ScreenShake* screenShake;
};


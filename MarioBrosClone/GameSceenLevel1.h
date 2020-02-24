#pragma once
#include "SDL.h"
#include "Commons.h"
#include "GameScreen.h"
#include "Character.h"
#include "CharacterMario.h"
#include "CharacterLuigi.h"
#include "CharacterKoopa.h"
#include "LevelMap.h"
#include "PowBlock.h"
#include "ScreenShake.h"
#include "Coin.h"
#include <vector>

class Texture2D;
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
	Texture2D* mBackgroundTexture;
	CharacterMario* characterMario;
	CharacterLuigi* characterLuigi;

	PowBlock* mPowBlock;
	void UpdatePowBlock();

	void UpdateEnemies(float deltaTime, SDL_Event e);
	void CreateKoopa(Vector2D position, FACING direction, float speed);

	void CreateCoin(Vector2D position);
	void UpdateCoins(float deltaTime, SDL_Event e);

	std::vector<CharacterKoopa*> mKoopas;
	int enemyIndexToDelete;

	std::vector<Coin*> mCoins;
	int coinIndexToDelete;

	void SetLevelMap();
	bool SetUpLevel();

	ScreenShake* screenShake;
	LevelMap* mLevelMap;
};


#pragma once
#include "SDL.h"
#include "Commons.h"
#include "GameScreen.h"
#include "Character.h"
#include "CharacterMario.h"
#include "CharacterLuigi.h"
#include "LevelMap.h"
#include "PowBlock.h"
#include "ScreenShake.h"

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

	void SetLevelMap();
	bool SetUpLevel();

	ScreenShake* screenShake;
	LevelMap* mLevelMap;
};


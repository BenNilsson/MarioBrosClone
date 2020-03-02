#pragma once
#include "GameScreen.h"
#include "Sprite.h"
#include "Character.h"
#include "CharacterMario.h"
#include "CharacterLuigi.h"
#include "ScreenShake.h"

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

	bool SetUpLevel();

	CharacterMario* characterMario;
	CharacterLuigi* characterLuigi;

	void SetUpTileMap();

	ScreenShake* screenShake;
};

#pragma once
#include "SDL.h"
#include "Commons.h"
#include "GameScreen.h"

class Sprite;

class GameScreenIntro : GameScreen
{
public:
	GameScreenIntro(SDL_Renderer* renderer);
	~GameScreenIntro();

	void Render();
	void Update(float deltaTime, SDL_Event e);
private:
	Sprite* mBackgroundTexture;

	bool SetUpLevel();
};


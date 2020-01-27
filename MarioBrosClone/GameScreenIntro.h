#pragma once
#include "SDL.h"
#include "Commons.h"
#include "GameScreen.h"

class Texture2D;

class GameScreenIntro : GameScreen
{
public:
	GameScreenIntro(SDL_Renderer* renderer);
	~GameScreenIntro();

	void Render();
	void Update(float deltaTime, SDL_Event e);
private:
	Texture2D* mBackgroundTexture;

	bool SetUpLevel();
};


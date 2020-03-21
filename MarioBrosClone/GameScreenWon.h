#pragma once
#include "GameScreen.h"
#include "SDL.h"
#include "UIText.h"


class GameScreenWon : public GameScreen
{
public:

	GameScreenWon(SDL_Renderer* renderer);
	~GameScreenWon();

	void Render();
	void Update(float deltaTime, SDL_Event e);

private:
	SDL_Renderer* mRenderer;

	Sprite* mLogoSprite;
	Sprite* mCoinBigSprite;
	Sprite* mInfoSprite;

	UIText* mYouWonText;

	const float renderLogoAfterTime = 0.50f;
	const float renderWonTextAfterTime = 1.0f;
	const float renderCoinDataAfterTime = 1.50f;
	const float renderInfoAfterTime = 2.0f;

	float mTimeElapsed;
};


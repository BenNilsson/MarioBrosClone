#include "GameScreenWon.h"
#include "Commons.h"
#include "Constants.h"
#include <iostream>
#include "GameManager.h"

GameScreenWon::GameScreenWon(SDL_Renderer* renderer) : GameScreen(renderer)
{
	mTimeElapsed = 0;
	mRenderer = renderer;

	mLogoSprite = new Sprite(mRenderer);
	mCoinBigSprite = new Sprite(mRenderer);
	mInfoSprite = new Sprite(mRenderer);

	if (!mLogoSprite->LoadFromFile("Textures/logo.png"))
	{
		std::cerr << "Could not load Textures/logo.png" << std::endl;
	}

	if (!mCoinBigSprite->LoadFromFile("Textures/coin-big.png"))
	{
		std::cerr << "Could not load Textures/coin-big.png" << std::endl;
	}

	if (!mInfoSprite->LoadFromFile("Textures/info.png"))
	{
		std::cerr << "Could not load Textures/info.png" << std::endl;
	}

	mYouWonText = new UIText(mRenderer, "YOU WON!", { 255, 255, 255, 255 }, 250, 55);
	mYouWonText->Position = new Vector2D((SCREEN_WIDTH * 0.50f) - (250 * 0.50f), 175);

	if (GameManager::GetInstance()->mScoreText != nullptr)
		GameManager::GetInstance()->mScoreText->Position = new Vector2D(Vector2D((SCREEN_WIDTH * 0.40f) - (mCoinBigSprite->GetWidth() * 0.50f) + 50, 262));
}

GameScreenWon::~GameScreenWon()
{
	delete mRenderer;
	mRenderer = nullptr;

	delete mLogoSprite;
	mLogoSprite = nullptr;

	delete mCoinBigSprite;
	mCoinBigSprite = nullptr;

	delete mYouWonText;
	mYouWonText = nullptr;

	delete mInfoSprite;
	mInfoSprite = nullptr;
}

void GameScreenWon::Render()
{
	// Background color
	SDL_SetRenderDrawColor(mRenderer, 10, 10, 10, 0x00);

	if(mTimeElapsed >= renderLogoAfterTime)
		mLogoSprite->Render(Vector2D((SCREEN_WIDTH * 0.50f) - (mLogoSprite->GetWidth() * 0.50f), 50), SDL_FLIP_NONE);

	if (mTimeElapsed >= renderWonTextAfterTime)
	{
		mYouWonText->Draw();
	}

	if (mTimeElapsed >= renderCoinDataAfterTime)
	{
		mCoinBigSprite->Render(Vector2D((SCREEN_WIDTH * 0.40f) - (mCoinBigSprite->GetWidth() * 0.50f), 250), SDL_FLIP_NONE);
		if (GameManager::GetInstance()->mScoreText != nullptr)
			GameManager::GetInstance()->mScoreText->Draw();
	}

	if (mTimeElapsed >= renderInfoAfterTime)
	{
		mInfoSprite->Render(Vector2D(0, 0), SDL_FLIP_NONE);
	}

}

void GameScreenWon::Update(float deltaTime, SDL_Event e)
{
	mTimeElapsed += deltaTime;

	if (mTimeElapsed >= renderInfoAfterTime)
		GameManager::GetInstance()->ChangeState(GameManager::GameState::WON);
	else if (mTimeElapsed >= 5.0f)
		mTimeElapsed = 5;

	// Handle events
	switch (e.type)
	{
		// KeyUP
	case SDL_KEYUP:
		// Get Key Pressed
		switch (e.key.keysym.sym)
		{
			// If the user hits space
		case(SDLK_SPACE):
			if (GameManager::GetInstance()->GetState() == GameManager::GameState::WON)
			{
				// Reset score
				GameManager::GetInstance()->SetScore(0);

				// Restart Game
				GameManager::GetInstance()->ChangeState(GameManager::GameState::INGAME);
				GameManager::GetInstance()->gameScreenManager->ChangeScreen(SCREEN_LEVEL1);
			}
			break;
		}
		break;
	}
}

#include "GameManager.h"

// Initialise static variables
GameManager* GameManager::instance = 0;
GameManager::GameState GameManager::mCurGameState = GameManager::GameState::INTRO;

void GameManager::ChangeState(GameManager::GameState state)
{
	mCurGameState = state;
}

void GameManager::SetScore(int score)
{
	mScore = score;
	
	if (mScoreText != nullptr)
	{
		std::string str = "Score: " + std::to_string(GetScore());
		const char* score = str.c_str();
		mScoreText->Text = score;

		mScoreText->mSurface = TTF_RenderText_Solid(mScoreText->default_font, mScoreText->Text, mScoreText->mColor);
		mScoreText->mTexture = SDL_CreateTextureFromSurface(mScoreText->mRenderer, mScoreText->mSurface);
	}
}

void GameManager::AddScore(int add)
{
	mScore += add;

	if (mScoreText != nullptr)
	{
		std::string str = "Score: " + std::to_string(GetScore());
		const char* score = str.c_str();
		mScoreText->Text = score;

		mScoreText->mSurface = TTF_RenderText_Solid(mScoreText->default_font, mScoreText->Text, mScoreText->mColor);
		mScoreText->mTexture = SDL_CreateTextureFromSurface(mScoreText->mRenderer, mScoreText->mSurface);
	}
}

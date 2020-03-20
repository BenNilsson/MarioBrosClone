#include "GameManager.h"
#include <fstream>

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

	if (mHighscoreText != nullptr)
	{
		if (mScore > mHighscore)
		{
			mHighscore = mScore;

			std::string hstr = "High Score: " + std::to_string(GetHighScore());
			const char* hscore = hstr.c_str();
			mHighscoreText->Text = hscore;

			mHighscoreText->mSurface = TTF_RenderText_Solid(mHighscoreText->default_font, mHighscoreText->Text, mHighscoreText->mColor);
			mHighscoreText->mTexture = SDL_CreateTextureFromSurface(mHighscoreText->mRenderer, mHighscoreText->mSurface);
		}
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

	if (mHighscoreText != nullptr)
	{
		if (mScore > mHighscore)
		{
			mHighscore = mScore;

			std::string hstr = "High Score: " + std::to_string(GetHighScore());
			const char* hscore = hstr.c_str();
			mHighscoreText->Text = hscore;

			mHighscoreText->mSurface = TTF_RenderText_Solid(mHighscoreText->default_font, mHighscoreText->Text, mHighscoreText->mColor);
			mHighscoreText->mTexture = SDL_CreateTextureFromSurface(mHighscoreText->mRenderer, mHighscoreText->mSurface);
		}
	}
}

void GameManager::SaveScore()
{
	std::ofstream outfile("score.dat");

	// Check whether the file was opened
	if (!outfile.is_open())
	{
		std::cerr << "SAVING SCORE - File Could Not Be Opened" << std::endl;
		std::exit(1);
	}

	// File is open
	outfile << mHighscore;
	// Close the file after use
	outfile.close();
}

int GameManager::LoadScore()
{
	int highScore = 0;

	std::ifstream infile("score.dat");

	// Check if the file was opened
	if (infile.is_open())
	{
		// Loop through the file and save the first line
		while (infile >> highScore)
			mHighscore = highScore;

		// Close file after use
		infile.close();
	}
	else
	{
		std::cerr << "LOADING SCORE - FILE COULD NOT BE OPENED. CREATING FILE" << std::endl;

		// Create the file
		std::ofstream outStream;
		outStream.open("score.dat");
	}

	return highScore;
}

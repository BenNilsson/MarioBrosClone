#include "GameManager.h"

// Initialise static variables
GameManager* GameManager::instance = 0;
GameManager::GameState GameManager::mCurGameState = GameManager::GameState::INTRO;

void GameManager::ChangeState(GameManager::GameState state)
{
	mCurGameState = state;
}

#include "GameSceenLevel1.h"
#include <iostream>
#include "Sprite.h"
#include "Collisions.h"
#include "SoundManager.h"
#include "GameManager.h"
#include <fstream>
#include "Camera.h"
#include "TileMap.h"

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer) : GameScreen(renderer)
{
	if (SetUpLevel())
	{
		// Stop all sound effects from playing
		if (Mix_HaltChannel(-1));

		if (soundmanager::SoundManager::GetInstance()->IsPlaying())
		{
			soundmanager::SoundManager::GetInstance()->StopMusic();
		}
		soundmanager::SoundManager::GetInstance()->PlayMusic("Music/Mario.wav");
	}
}

GameScreenLevel1::~GameScreenLevel1()
{
	delete mBackgroundTexture;
	mBackgroundTexture = nullptr;

	delete characterMario;
	characterMario = nullptr;

	delete characterLuigi;
	characterLuigi = nullptr;

	delete mPowBlock;
	mPowBlock = nullptr;

	delete screenShake;
	screenShake = nullptr;

	delete mLogoSprite;
	mLogoSprite = nullptr;

	delete mCoinBigSprite;
	mCoinBigSprite = nullptr;

	delete mYouLostText;
	mYouLostText = nullptr;

	delete mInfoSprite;
	mInfoSprite = nullptr;
}

void GameScreenLevel1::Render()
{
	// Background color
	SDL_SetRenderDrawColor(mRenderer, 92, 148, 252, 0x00);

	// Draw the background
	mBackgroundTexture->Render(Vector2D(-100 - Camera::GetInstance()->GetPosition().x, screenShake->GetBackgroundYPos()), SDL_FLIP_NONE);

	// Render all tiles
	tileMap->DrawTileMap();

	// Draw the player
	characterMario->Render();
	characterLuigi->Render();

	// Draw Powblock
	if (mPowBlock != nullptr)
		mPowBlock->Render(Camera::GetInstance()->GetPosition().x, Camera::GetInstance()->GetPosition().y);

	if (tileMap->mFlag != nullptr)
		tileMap->mFlag->Render(Camera::GetInstance()->GetPosition().x, Camera::GetInstance()->GetPosition().y);

	// Draw enemies
	for (unsigned int i = 0; i < tileMap->mKoopas.size(); i++)
	{
		tileMap->mKoopas[i]->Render(Camera::GetInstance()->GetPosition().x, Camera::GetInstance()->GetPosition().y);
	}

	// Draw coins
	for (unsigned int i = 0; i < tileMap->mCoins.size(); i++)
	{
		tileMap->mCoins[i]->Render(Camera::GetInstance()->GetPosition().x, Camera::GetInstance()->GetPosition().y);
	}

	// Draw game over info
	if (!RenderDeathInfo())
	{
		// Draw score
		GameManager::GetInstance()->mScoreText->Draw();
		GameManager::GetInstance()->mHighscoreText->Draw();
	}
	
	
}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e)
{

	// Update pow block
	UpdatePowBlock();

	// Update the player
	characterMario->Update(deltaTime, e);
	characterLuigi->Update(deltaTime, e);

	// Update question mark blocks
	UpdateQuestionMarkBlocks(deltaTime, e);

	// Make sure mario stays within the camera's bounds
	HandleViewportCollision();
	
	UpdateCoins(deltaTime, e);
	
	if (tileMap->IsLoaded())
	{
		// Update enemies
		UpdateEnemies(deltaTime, e);
	}

	// Update screenshake, passing the koopa vector in order for the pow block to kill them
	screenShake->Update(deltaTime, tileMap->mKoopas);

	// Update Camera Position
	UpdateCameraPosition();

	// Check to see if the character and flag collide
	if (CheckFlagCollision()) return;

	// Check to see the loss condition
	CheckForGameOver(deltaTime);
	// Check if the game has been restarted
	CheckForRestart(e);
	
}

void GameScreenLevel1::CheckForRestart(SDL_Event e)
{
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
			if (GameManager::GetInstance()->GetState() == GameManager::GameState::GAMEOVER)
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

	return;
}

void GameScreenLevel1::CheckForGameOver(float deltaTime)
{
	// Set game state to game over if mario & luigi is dead
	if (!characterMario->IsAlive() && !characterLuigi->IsAlive())
	{
		if (GameManager::GetInstance()->GetState() == GameManager::GameState::INGAME)
		{
			// Set game state to game over
			GameManager::GetInstance()->ChangeState(GameManager::GameState::GAMEOVER);

			// Save score
			GameManager::GetInstance()->SaveHighScore();

			// Stop music if playing
			if (soundmanager::SoundManager::GetInstance()->IsPlaying())
			{
				soundmanager::SoundManager::GetInstance()->StopMusic();
			}

			// Play sound
			soundmanager::SoundManager::GetInstance()->PlaySFX("SFX/smb_gameover.wav");

			// Reset timer
			mTimeElapsedAfterDeath = 0;
		}

		UpdateDeathInfo(deltaTime);
	}
}

void GameScreenLevel1::UpdatePowBlock()
{
	if (mPowBlock != nullptr)
	{
		// Check if the pow block collides with mario
		if (Collisions::Instance()->Box(characterMario->GetCollisionBox(), mPowBlock->GetCollisionBox()))
		{
			// Check if the pow block is avialable
			if (mPowBlock->IsAvailable())
			{
				// Check if they collided whilst jumping
				if (characterMario->IsJumping())
				{
					screenShake->DoScreenShake();
					mPowBlock->TakeAHit();
					characterMario->CancelJump();
					soundmanager::SoundManager::GetInstance()->PlaySFX("SFX/bump.wav");
				}
			}
		}

		// Check if the pow block collides with luigi
		if (Collisions::Instance()->Box(characterLuigi->GetCollisionBox(), mPowBlock->GetCollisionBox()))
		{
			// Check if the pow block is avialable
			if (mPowBlock->IsAvailable())
			{
				// Check if they collided whilst jumping
				if (characterLuigi->IsJumping())
				{
					screenShake->DoScreenShake();
					mPowBlock->TakeAHit();
					characterLuigi->CancelJump();
					soundmanager::SoundManager::GetInstance()->PlaySFX("SFX/bump.wav");
				}
			}
		}
	}
}

void GameScreenLevel1::UpdateEnemies(float deltaTime, SDL_Event e)
{
	if (tileMap == nullptr) return;

	// Only update enemies if we have any
	if (!tileMap->mKoopas.empty())
	{
		enemyIndexToDelete = -1;
		for (unsigned int i = 0; i < tileMap->mKoopas.size(); i++)
		{
			// Check if mario is nearby, if not, do not update
			if (characterMario != nullptr && tileMap->mKoopas[i]->GetPosition().x - characterMario->GetPosition().x < tileMap->mKoopas[i]->GetUpdateRange() || characterLuigi != nullptr && tileMap->mKoopas[i]->GetPosition().x - characterLuigi->GetPosition().x < tileMap->mKoopas[i]->GetUpdateRange())
			{
				// Update enemy
				tileMap->mKoopas[i]->Update(deltaTime, e);

				if (tileMap->mKoopas[i]->GetPosition().y > 0.0f)
				{
					int centralYPositionInGrid = (int)(tileMap->mKoopas[i]->GetPosition().y + (tileMap->mKoopas[i]->GetHeight() * 0.50f)) / 32;
					int rightSidePositionInGrid = (int)(tileMap->mKoopas[i]->GetPosition().x + tileMap->mKoopas[i]->GetWidth()) / 32;
					int leftSidePositionInGrid = (int)tileMap->mKoopas[i]->GetPosition().x / 32;
					int footPositionInGrid = (int)(tileMap->mKoopas[i]->GetPosition().y + (tileMap->mKoopas[i]->GetWidth())) / 32;
					int groundCheckPositionInGrid = (int)(tileMap->mKoopas[i]->GetPosition().y + (tileMap->mKoopas[i]->GetWidth()) + 16) / 32;

					int headPosition = (int)(tileMap->mKoopas[i]->GetPosition().y + 5);

					// Check if head is hit by mario or luigi when falling
					if (characterMario->IsFalling())
					{
						if (Collisions::Instance()->Box(Rect2D(characterMario->GetPosition().x, characterMario->GetPosition().y + characterMario->GetHeight(), characterMario->GetWidth(), 1), Rect2D(tileMap->mKoopas[i]->GetPosition().x, headPosition - 1, tileMap->mKoopas[i]->GetWidth(), 1)))
						{
							// Make mario do a small jump
							if (characterMario->IsJumping())
								characterMario->CancelJump();
							characterMario->JumpSmall();
							characterMario->SetPosition(Vector2D(characterMario->GetPosition().x, characterMario->GetPosition().y - 5));

							// Play sound
							soundmanager::SoundManager::GetInstance()->PlaySFX("SFX/kick.wav");

							// Check to see if injured
							if (tileMap->mKoopas[i]->IsInjured())
							{
								tileMap->mKoopas[i]->SetAlive(false);
								// Add score
								GameManager::GetInstance()->AddScore(200);
							}
							else
							{
								tileMap->mKoopas[i]->TakeDamage();
							}
						}
					}

					else if (characterLuigi->IsFalling())
					{
						if (Collisions::Instance()->Box(Rect2D(characterLuigi->GetPosition().x, characterLuigi->GetPosition().y + characterLuigi->GetHeight(), characterLuigi->GetWidth(), 1), Rect2D(tileMap->mKoopas[i]->GetPosition().x, headPosition - 1, tileMap->mKoopas[i]->GetWidth(), 1)))
						{
							// Make luigi do a small jump
							if (characterLuigi->IsJumping())
								characterLuigi->CancelJump();
							characterLuigi->JumpSmall();
							characterLuigi->SetPosition(Vector2D(characterLuigi->GetPosition().x, characterLuigi->GetPosition().y - 5));

							soundmanager::SoundManager::GetInstance()->PlaySFX("SFX/kick.wav");

							// Check to see if injured
							if (tileMap->mKoopas[i]->IsInjured())
							{
								tileMap->mKoopas[i]->SetAlive(false);
								// Add score
								GameManager::GetInstance()->AddScore(200);
							}
							else
							{
								tileMap->mKoopas[i]->TakeDamage();
							}
						}
					}

					// Check if the enemy hits an edge
					if (tileMap->mKoopas[i]->GetFacingDirection() == FACING::FACING_RIGHT)
					{
						if (tileMap->GetTileAt(rightSidePositionInGrid, groundCheckPositionInGrid) == nullptr)
						{
							if (tileMap->mKoopas[i]->IsSmart())
							{
								if (!tileMap->mKoopas[i]->IsFalling())
								{
									tileMap->mKoopas[i]->Flip();
									tileMap->mKoopas[i]->SetPosition(Vector2D(tileMap->mKoopas[i]->GetPosition().x - 15, tileMap->mKoopas[i]->GetPosition().y));
								}
							}
						}
						else if (tileMap->GetTileAt(rightSidePositionInGrid, centralYPositionInGrid) != nullptr)
						{
							// If the right side collides with a solid tile, flip koopa
							if (tileMap->GetTileAt(rightSidePositionInGrid, centralYPositionInGrid)->GetCollisionType() == CollisionType::TILE_SOLID)
							{
								tileMap->mKoopas[i]->Flip();
								tileMap->mKoopas[i]->SetPosition(Vector2D(tileMap->mKoopas[i]->GetPosition().x - 15, tileMap->mKoopas[i]->GetPosition().y));
							}
						}
					}
					else if (tileMap->mKoopas[i]->GetFacingDirection() == FACING::FACING_LEFT)
					{
						if (tileMap->GetTileAt(leftSidePositionInGrid, groundCheckPositionInGrid) == nullptr)
						{
							if (tileMap->mKoopas[i]->IsSmart())
							{
								if (!tileMap->mKoopas[i]->IsFalling())
								{
									tileMap->mKoopas[i]->Flip();
									tileMap->mKoopas[i]->SetPosition(Vector2D(tileMap->mKoopas[i]->GetPosition().x + 15, tileMap->mKoopas[i]->GetPosition().y));
								}
							}
						}
						// If the right side collides with a solid tile, flip koopa
						else if (tileMap->GetTileAt(leftSidePositionInGrid, centralYPositionInGrid) != nullptr) {
							if (tileMap->GetTileAt(leftSidePositionInGrid, centralYPositionInGrid)->GetCollisionType() == CollisionType::TILE_SOLID)
							{
								tileMap->mKoopas[i]->Flip();
								tileMap->mKoopas[i]->SetPosition(Vector2D(tileMap->mKoopas[i]->GetPosition().x + 15, tileMap->mKoopas[i]->GetPosition().y));
							}
						}
					}
				}

				// Check if koopas collided with mario
				if (!tileMap->mKoopas[i]->IsInjured())
				{
					if (Collisions::Instance()->Circle(Circle2D(tileMap->mKoopas[i]->GetCollisionRadius(), tileMap->mKoopas[i]->GetPosition()), Circle2D(characterMario->GetCollisionRadius(), characterMario->GetPosition())))
					{
						if(characterMario->IsAlive())
							characterMario->SetAlive(false);
					}

					// Check if koopas collided with luigi
					if (Collisions::Instance()->Circle(Circle2D(tileMap->mKoopas[i]->GetCollisionRadius(), tileMap->mKoopas[i]->GetPosition()), Circle2D(characterLuigi->GetCollisionRadius(), characterLuigi->GetPosition())))
					{
						if (characterLuigi->IsAlive())
							characterLuigi->SetAlive(false);
					}
				}
				else
				{
					// If the koopa is injured, kill it
					if (Collisions::Instance()->Circle(Circle2D(tileMap->mKoopas[i]->GetCollisionRadius(), tileMap->mKoopas[i]->GetPosition()), Circle2D(characterMario->GetCollisionRadius(), characterMario->GetPosition())))
					{
						soundmanager::SoundManager::GetInstance()->PlaySFX("SFX/kick.wav");
						GameManager::GetInstance()->AddScore(100);
						tileMap->mKoopas[i]->SetAlive(false);
					}

					// Check if koopas collided with luigi
					if (Collisions::Instance()->Circle(Circle2D(tileMap->mKoopas[i]->GetCollisionRadius(), tileMap->mKoopas[i]->GetPosition()), Circle2D(characterLuigi->GetCollisionRadius(), characterLuigi->GetPosition())))
					{
						soundmanager::SoundManager::GetInstance()->PlaySFX("SFX/kick.wav");
						GameManager::GetInstance()->AddScore(100);
						tileMap->mKoopas[i]->SetAlive(false);
					}
				}

				// If the enemy is no longer alive, schdule it for deletion
				if (!tileMap->mKoopas[i]->GetAlive())
				{
					enemyIndexToDelete = i;
				}
			}	
		}

		
		// Remove a dead enemy, 1 each update
		if (enemyIndexToDelete != -1)
		{
			tileMap->mKoopas.erase(tileMap->mKoopas.begin() + enemyIndexToDelete);
		}
		
	}
}

void GameScreenLevel1::CreateKoopa(Vector2D position, FACING direction, float speed)
{
	tileMap->mKoopas.push_back(new CharacterKoopa(mRenderer, "Textures/Koopa.png", position, tileMap, speed, direction));
}

void GameScreenLevel1::HandleViewportCollision()
{
	if (characterMario->IsAlive())
	{
		if (characterMario->GetPosition().x <= Camera::GetInstance()->GetCameraBounds().x)
		{
			characterMario->SetPosition(Vector2D(characterMario->GetPosition().x + 1, characterMario->GetPosition().y));
		}
	}
	else if (characterLuigi->IsAlive())
	{
		if (characterLuigi->GetPosition().x <= Camera::GetInstance()->GetCameraBounds().x)
		{
			characterLuigi->SetPosition(Vector2D(characterLuigi->GetPosition().x + 1, characterLuigi->GetPosition().y));
		}
	}
}

void GameScreenLevel1::UpdateCameraPosition()
{
	if (characterMario->IsAlive())
	{
		if ((characterMario->GetPosition().x + 32 / 2) - CAMERA_WIDTH / 2 > mLastCamXPos)
		{
			Camera::GetInstance()->SetPosition(Vector2D((characterMario->GetPosition().x + 32 / 2) - CAMERA_WIDTH / 2, screenShake->GetBackgroundYPos()));
			mLastCamXPos = (characterMario->GetPosition().x + 32 / 2) - CAMERA_WIDTH / 2;
		}
	}
	else if (characterLuigi->IsAlive())
	{
		if (characterLuigi->GetPosition().x <= Camera::GetInstance()->GetCameraBounds().x)
		{
			Camera::GetInstance()->SetPosition(Vector2D((characterLuigi->GetPosition().x + 32 / 2) - CAMERA_WIDTH / 2, screenShake->GetBackgroundYPos()));
			mLastCamXPos = (characterLuigi->GetPosition().x + 32 / 2) - CAMERA_WIDTH / 2;
		}

		if ((characterLuigi->GetPosition().x + 32 / 2) - CAMERA_WIDTH / 2 > mLastCamXPos)
		{
			Camera::GetInstance()->SetPosition(Vector2D((characterLuigi->GetPosition().x + 32 / 2) - CAMERA_WIDTH / 2, screenShake->GetBackgroundYPos()));
			mLastCamXPos = (characterLuigi->GetPosition().x + 32 / 2) - CAMERA_WIDTH / 2;
		}
	}
}

void GameScreenLevel1::CreateCoin(Vector2D position)
{
	tileMap->mCoins.push_back(new Coin(mRenderer, "Textures/coin.png", position));

}

void GameScreenLevel1::UpdateCoins(float deltaTime, SDL_Event e)
{
	if (!tileMap->mCoins.empty())
	{
		coinIndexToDelete = -1;

		// Updates
		for (unsigned int i = 0; i < tileMap->mCoins.size(); i++)
		{
			// Update coins
			tileMap->mCoins[i]->Update(deltaTime, e);

			bool collided = false;

			// Check if mario collides with coin
			if (Collisions::Instance()->Circle(Circle2D(tileMap->mCoins[i]->GetCollisionRadius(), tileMap->mCoins[i]->GetPosition()), Circle2D(characterMario->GetCollisionRadius(), characterMario->GetPosition())))
			{
				collided = true;
			}

			// Check if luigi collides with coin
			if (Collisions::Instance()->Circle(Circle2D(tileMap->mCoins[i]->GetCollisionRadius(), tileMap->mCoins[i]->GetPosition()), Circle2D(characterLuigi->GetCollisionRadius(), characterLuigi->GetPosition())))
			{
				collided = true;
			}

			if (collided)
			{
				// coin collected
				coinIndexToDelete = i;
				soundmanager::SoundManager::GetInstance()->PlaySFX("SFX/coin.wav");
				GameManager::GetInstance()->AddScore(50);
			}
		}

		// Remove a dead enemy, 1 each update
		if (coinIndexToDelete != -1)
		{
			tileMap->mCoins.erase(tileMap->mCoins.begin() + coinIndexToDelete);
		}
	}

	
}

void GameScreenLevel1::UpdateQuestionMarkBlocks(float deltaTime, SDL_Event e)
{
	int marioCentralXPositionInGrid = (int)(characterMario->GetPosition().x + (characterMario->GetWidth()* 0.50f)) / 32;
	int luigiCentralXPositionInGrid = (int)(characterLuigi->GetPosition().x + (characterLuigi->GetWidth() * 0.50f)) / 32;

	int marioHeadPositionInGrid = (int)(characterMario->GetPosition().y) / 32;
	int luigiHeadPositionInGrid = (int)(characterLuigi->GetPosition().y) / 32;

	// Check if mario's head collides with the centre of the queston mark block
	if (tileMap->GetTileAt(marioCentralXPositionInGrid, marioHeadPositionInGrid) != nullptr)
	{
		if (tileMap->GetTileAt(marioCentralXPositionInGrid, marioHeadPositionInGrid)->GetBlock() != nullptr)
		{
			if (tileMap->GetTileAt(marioCentralXPositionInGrid, marioHeadPositionInGrid)->GetBlock()->GetBlockType() == Block::BlockType::BLOCK_QUESTION_MARK)
			{
				if (tileMap->GetTileAt(marioCentralXPositionInGrid, marioHeadPositionInGrid)->GetBlock()->IsAvailable())
				{
					// Collision, spawn coin and set question mark block to unavailable
					tileMap->GetTileAt(marioCentralXPositionInGrid, marioHeadPositionInGrid)->GetBlock()->SetAvailable(false);
					CreateCoin(Vector2D(tileMap->GetTileAt(marioCentralXPositionInGrid, marioHeadPositionInGrid)->GetBlock()->GetPosition().x + 3, tileMap->GetTileAt(marioCentralXPositionInGrid, marioHeadPositionInGrid)->GetBlock()->GetPosition().y - 32));
				}
			}
		}
	// Luigi
	}else if (tileMap->GetTileAt(luigiCentralXPositionInGrid, luigiHeadPositionInGrid) != nullptr)
	{
		if (tileMap->GetTileAt(luigiCentralXPositionInGrid, luigiHeadPositionInGrid)->GetBlock() != nullptr)
		{
			if (tileMap->GetTileAt(luigiCentralXPositionInGrid, luigiHeadPositionInGrid)->GetBlock()->GetBlockType() == Block::BlockType::BLOCK_QUESTION_MARK)
			{
				if (tileMap->GetTileAt(luigiCentralXPositionInGrid, luigiHeadPositionInGrid)->GetBlock()->IsAvailable())
				{
					tileMap->GetTileAt(luigiCentralXPositionInGrid, luigiHeadPositionInGrid)->GetBlock()->SetAvailable(false);
					CreateCoin(Vector2D(tileMap->GetTileAt(luigiCentralXPositionInGrid, luigiHeadPositionInGrid)->GetBlock()->GetPosition().x + 3, tileMap->GetTileAt(luigiCentralXPositionInGrid, luigiHeadPositionInGrid)->GetBlock()->GetPosition().y - 32));
				}
			}
		}
	}
}

bool GameScreenLevel1::RenderDeathInfo()
{
	if (mTimeElapsedAfterDeath == 0) return false;

	if (mTimeElapsedAfterDeath >= renderLogoAfterTime)
		mLogoSprite->Render(Vector2D((SCREEN_WIDTH * 0.50f) - (mLogoSprite->GetWidth() * 0.50f), 50), SDL_FLIP_NONE);

	if (mTimeElapsedAfterDeath >= renderWonTextAfterTime)
	{
		mYouLostText->Draw();
	}

	if (mTimeElapsedAfterDeath >= renderCoinDataAfterTime)
	{
		mCoinBigSprite->Render(Vector2D((SCREEN_WIDTH * 0.40f) - (mCoinBigSprite->GetWidth() * 0.50f), 250), SDL_FLIP_NONE);
		if (GameManager::GetInstance()->mScoreText != nullptr)
		{
			GameManager::GetInstance()->mScoreText->Position = new Vector2D(Vector2D((SCREEN_WIDTH * 0.40f) - (mCoinBigSprite->GetWidth() * 0.50f) + 50, 262));
			GameManager::GetInstance()->mScoreText->Draw();
		}
	}

	if (mTimeElapsedAfterDeath >= renderInfoAfterTime)
	{
		mInfoSprite->Render(Vector2D(0, 0), SDL_FLIP_NONE);
	}

	return true;
}

void GameScreenLevel1::UpdateDeathInfo(float deltaTime)
{
	mTimeElapsedAfterDeath += deltaTime;

	if (mTimeElapsedAfterDeath >= 5.0f)
		mTimeElapsedAfterDeath = 5;
}

bool GameScreenLevel1::CheckFlagCollision()
{
	if (tileMap->mFlag != nullptr)
	{
		bool collided = false;

		if (characterMario->IsAlive() || characterLuigi->IsAlive())
		{
			if (Collisions::Instance()->Box(tileMap->mFlag->GetCollisionBox(), characterMario->GetCollisionBox()))
			{
				collided = true;
			}
			else if (Collisions::Instance()->Box(tileMap->mFlag->GetCollisionBox(), characterLuigi->GetCollisionBox()))
			{
				collided = true;
			}
		}

		if (collided)
		{
			GameManager::GetInstance()->AddScore(1000);
			GameManager::GetInstance()->SaveHighScore();
			GameManager::GetInstance()->gameScreenManager->ChangeScreen(SCREEN_LEVEL2);
			return true;
		}

		return false;
	}

	return false;
}

void GameScreenLevel1::SetUpTileMap()
{
	// Read file
	std::ifstream file("level1.txt");

	// Get column length
	int rows = 0;
	int columns = 0;
	std::string line;
	if (file.is_open())
	{
		while (std::getline(file, line))
			rows++;
	}

	// Assuming the width is fixed, get column count
	for (int x = 0; x < line.length(); x++)
	{
		if (line[x] != ' ')
			columns++;
	}

	char** map;
	map = new char* [rows];

	// Predefine map
	for (unsigned int i = 0; i < rows; i++)
		map[i] = new char[columns];

	// Hop back to the beginning of the file
	file.clear();
	file.seekg(0, std::ios::beg);

	for (int row = 0; row < rows; row++)
	{
		for (int column = 0; column < columns; column++)
		{
			file >> map[row][column];
		}
	}

	file.close();

	// Create new TileMap
	tileMap = new TileMap(mRenderer);
	tileMap->GenerateTileMap(map, rows, columns);

	// Let mario and luigi know what map they're on
	characterMario->mCurrentTileMap = tileMap;
	characterLuigi->mCurrentTileMap = tileMap;

}

bool GameScreenLevel1::SetUpLevel()
{
	// Load the background texture
	mBackgroundTexture = new Sprite(mRenderer);
	if (!mBackgroundTexture->LoadFromFile("Textures/background.png"))
	{
		std::cout << "Failed to load background texture!";
		return false;
	}

	screenShake = new ScreenShake();

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

	mYouLostText = new UIText(mRenderer, "GAME OVER!", { 255, 255, 255, 255 }, 250, 55);
	mYouLostText->Position = new Vector2D((SCREEN_WIDTH * 0.50f) - (250 * 0.50f), 175);

	// Set up the player character
	characterMario = new CharacterMario(mRenderer, "Textures/mario-run.png", Vector2D(272, 230), tileMap);
	characterLuigi = new CharacterLuigi(mRenderer, "Textures/luigi-run.png", Vector2D(364, 230), tileMap);

	SetUpTileMap();

	// Setup score
	std::string str = "Score: " + std::to_string(GameManager::GetInstance()->GetScore());
	const char* score = str.c_str();

	GameManager::GetInstance()->mScoreText = new UIText(mRenderer, str.c_str(), { 255, 255, 255, 0 }, 125);
	GameManager::GetInstance()->mScoreText->Position = new Vector2D(15, 15);

	// Setup high score
	std::string hstr = "High Score: " + std::to_string(GameManager::GetInstance()->GetHighScore());
	const char* hscore = hstr.c_str();

	GameManager::GetInstance()->mHighscoreText = new UIText(mRenderer, hstr.c_str(), { 255, 255, 255, 0 }, 200);
	GameManager::GetInstance()->mHighscoreText->Position = new Vector2D(245, 15);

	// Create PowBlock
	//mPowBlock = new PowBlock(mRenderer);

	// Initialise flag value
	tileMap->mFlag->mCharacter = characterMario;


	return true;
}
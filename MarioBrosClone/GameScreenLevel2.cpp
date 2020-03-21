#include "GameScreenLevel2.h"
#include "SoundManager.h"
#include <fstream>
#include "Camera.h"
#include "TileMap.h"
#include "GameManager.h"
#include "Collisions.h"

GameScreenLevel2::GameScreenLevel2(SDL_Renderer* renderer) : GameScreen(mRenderer)
{
	levelIsSetup = false;
	mRenderer = renderer;

	if (SetUpLevel())
	{
		
		if (soundmanager::SoundManager::GetInstance()->IsPlaying())
		{
			soundmanager::SoundManager::GetInstance()->StopMusic();
		}
		//soundmanager::SoundManager::GetInstance()->PlayMusic("Music/Mario.wav");
		
	}
}

GameScreenLevel2::~GameScreenLevel2()
{
	delete mBackgroundTexture;
	mBackgroundTexture = nullptr;
}

void GameScreenLevel2::Render()
{
	// Background color
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 0x00);

	if (!levelIsSetup) return;

	// Render all tiles
	tileMap->DrawTileMap();

	// Draw the player
	characterMario->Render();
	characterLuigi->Render();

	// Draw coins
	for (unsigned int i = 0; i < tileMap->mCoins.size(); i++)
	{
		tileMap->mCoins[i]->Render(Camera::GetInstance()->GetPosition().x, Camera::GetInstance()->GetPosition().y);
	}

	// Draw enemies
	for (unsigned int i = 0; i < tileMap->mKoopas.size(); i++)
	{
		tileMap->mKoopas[i]->Render(Camera::GetInstance()->GetPosition().x, Camera::GetInstance()->GetPosition().y);
	}

	if (tileMap->mFlag != nullptr)
		tileMap->mFlag->Render(Camera::GetInstance()->GetPosition().x, Camera::GetInstance()->GetPosition().y);

	// Draw score
	GameManager::GetInstance()->mScoreText->Draw();
	GameManager::GetInstance()->mHighscoreText->Draw();

}

void GameScreenLevel2::Update(float deltaTime, SDL_Event e)
{
	if (!levelIsSetup) return;

	// Update the player
	characterMario->Update(deltaTime, e);
	characterLuigi->Update(deltaTime, e);

	UpdateQuestionMarkBlocks(deltaTime, e);

	UpdateCoins(deltaTime, e);

	if (tileMap->IsLoaded())
	{
		// Update enemies
		UpdateEnemies(deltaTime, e);
	}

	// Make sure mario stays within the camera's bounds
	if (characterMario->GetPosition().x <= Camera::GetInstance()->GetCameraBounds().x)
	{
		characterMario->SetPosition(Vector2D(characterMario->GetPosition().x + 1, characterMario->GetPosition().y));
	}

	// Update Camera Position
	if ((characterMario->GetPosition().x + 32 / 2) - CAMERA_WIDTH / 2 > mLastCamXPos)
	{
		Camera::GetInstance()->SetPosition(Vector2D((characterMario->GetPosition().x + 32 / 2) - CAMERA_WIDTH / 2, screenShake->GetBackgroundYPos()));
		mLastCamXPos = (characterMario->GetPosition().x + 32 / 2) - CAMERA_WIDTH / 2;
	}

	// Check to see if the character and flag collide
	if (tileMap->mFlag != nullptr)
	{
		if (Collisions::Instance()->Box(tileMap->mFlag->GetCollisionBox(), characterMario->GetCollisionBox()))
		{
			GameManager::GetInstance()->AddScore(1000);
			GameManager::GetInstance()->SaveScore();
			GameManager::GetInstance()->gameScreenManager->ChangeScreen(SCREEN_WON);
			return;
		}
	}

	// Check to see the loss condition
	CheckForGameOver();
	// Check if the game has been restarted
	CheckForRestart(e);
}

bool GameScreenLevel2::SetUpLevel()
{
	mBackgroundTexture = new Sprite(mRenderer);
	if (!mBackgroundTexture->LoadFromFile("Textures/BackgroundMB.png"))
	{
		std::cout << "Failed to load background texture!";
		return false;
	}

	// Set up the player character
	characterMario = new CharacterMario(mRenderer, "Textures/mario-run.png", Vector2D(272, 200), tileMap);
	characterLuigi = new CharacterLuigi(mRenderer, "Textures/luigi-run.png", Vector2D(164, 200), tileMap);

	screenShake = new ScreenShake();

	SetUpTileMap();

	// Initialise flag value
	tileMap->mFlag->mCharacter = characterMario;

	levelIsSetup = true;
	return true;
}

void GameScreenLevel2::UpdateEnemies(float deltaTime, SDL_Event e)
{
	if (tileMap == nullptr) return;

	// Only update enemies if we have any
	if (!tileMap->mKoopas.empty())
	{
		enemyIndexToDelete = -1;
		for (unsigned int i = 0; i < tileMap->mKoopas.size(); i++)
		{
			// Check if mario is nearby, if not, do not update
			if (characterMario != nullptr && tileMap->mKoopas[i]->GetPosition().x - characterMario->GetPosition().x < tileMap->mKoopas[i]->GetUpdateRange())
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

							soundmanager::SoundManager::GetInstance()->PlaySFX("SFX/kick.wav");

							// Check to see if injured
							if (tileMap->mKoopas[i]->IsInjured())
							{
								tileMap->mKoopas[i]->SetAlive(false);
								// Add score
								GameManager::GetInstance()->AddScore(100);
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
								GameManager::GetInstance()->AddScore(100);
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
						characterMario->SetAlive(false);
					}

					// Check if koopas collided with luigi
					if (Collisions::Instance()->Circle(Circle2D(tileMap->mKoopas[i]->GetCollisionRadius(), tileMap->mKoopas[i]->GetPosition()), Circle2D(characterLuigi->GetCollisionRadius(), characterLuigi->GetPosition())))
					{
						characterLuigi->SetAlive(false);
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

void GameScreenLevel2::CreateCoin(Vector2D position)
{
	tileMap->mCoins.push_back(new Coin(mRenderer, "Textures/coin.png", position));
}

void GameScreenLevel2::UpdateCoins(float deltaTime, SDL_Event e)
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

void GameScreenLevel2::UpdateQuestionMarkBlocks(float deltaTime, SDL_Event e)
{
	int marioCentralXPositionInGrid = (int)(characterMario->GetPosition().x + (characterMario->GetWidth() * 0.50f)) / 32;
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
	}
	else if (tileMap->GetTileAt(luigiCentralXPositionInGrid, luigiHeadPositionInGrid) != nullptr)
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

void GameScreenLevel2::SetUpTileMap()
{
	// Read file
	std::ifstream file("level2.txt");

	// Get column length
	int rows = 0;
	int columns = 0;
	std::string line;
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			rows++;

			// Extract value into map
			
		}
	}

	// Assuming the width is fixed, get column count
	for (int x = 0; x < line.length(); x++)
	{
		if (line[x] != ' ')
			columns++;
	}

	char** map;
	map = new char*[rows];

	// Predefine map
	for (unsigned int i = 0; i < rows; i++)
	{
		map[i] = new char[columns];
	}

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

void GameScreenLevel2::CheckForGameOver()
{
	// Set game state to game over if mario is dead
	if (!characterMario->IsAlive() && GameManager::GetInstance()->GetState() == GameManager::GameState::INGAME)
	{
		// Set game state to game over
		GameManager::GetInstance()->ChangeState(GameManager::GameState::GAMEOVER);

		// Save score
		GameManager::GetInstance()->SaveScore();
	}
}

void GameScreenLevel2::CheckForRestart(SDL_Event e)
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
		case(SDLK_r):
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

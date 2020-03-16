#include "GameSceenLevel1.h"
#include <iostream>
#include "Sprite.h"
#include "Collisions.h"
#include "SoundManager.h"
#include "GameManager.h"
#include <fstream>
#include "Camera.h"

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer) : GameScreen(renderer)
{
	if (SetUpLevel())
	{
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

	mKoopas.clear();
	mCoins.clear();
}

void GameScreenLevel1::Render()
{
	// Draw the background
	//mBackgroundTexture->Render(Vector2D(0, screenShake->GetBackgroundYPos()), SDL_FLIP_NONE);

	// Render all tiles
	tileMap->DrawTileMap();

	// Draw the player
	characterMario->Render();
	characterLuigi->Render();

	// Draw Powblock
	mPowBlock->Render(Camera::GetInstance()->GetPosition().x, Camera::GetInstance()->GetPosition().y);

	flag->Render(Camera::GetInstance()->GetPosition().x, Camera::GetInstance()->GetPosition().y);

	// Draw enemies
	for (unsigned int i = 0; i < mKoopas.size(); i++)
	{
		mKoopas[i]->Render(Camera::GetInstance()->GetPosition().x, Camera::GetInstance()->GetPosition().y);
	}

	// Draw coins
	for (unsigned int i = 0; i < mCoins.size(); i++)
	{
		mCoins[i]->Render(Camera::GetInstance()->GetPosition().x, Camera::GetInstance()->GetPosition().y);
	}

	// Draw score
	GameManager::GetInstance()->mScoreText->Draw();
	
	
}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e)
{
	// Update the player
	characterMario->Update(deltaTime, e);
	characterLuigi->Update(deltaTime, e);
	
	// Update question mark blocks
	UpdateQuestionMarkBlocks(deltaTime, e);

	// Update pow block
	UpdatePowBlock();

	// Update enemies
	UpdateEnemies(deltaTime, e);

	UpdateCoins(deltaTime, e);

	// Update screenshake, passing the koopa vector in order for the pow block to kill them
	screenShake->Update(deltaTime, mKoopas);

	// Update Camera Position
	Camera::GetInstance()->SetPosition(Vector2D((characterMario->GetPosition().x + 32 / 2) - CAMERA_WIDTH / 2, screenShake->GetBackgroundYPos()));

	// Check to see if the character and flag collide
	if (Collisions::Instance()->Box(flag->GetCollisionBox(), characterMario->GetCollisionBox()))
	{
		GameManager::GetInstance()->gameScreenManager->ChangeScreen(SCREEN_LEVEL2);
		return;
	}

}

void GameScreenLevel1::UpdatePowBlock()
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

void GameScreenLevel1::UpdateEnemies(float deltaTime, SDL_Event e)
{
	// Only update enemies if we have any
	if (!mKoopas.empty())
	{
		enemyIndexToDelete = -1;
		for (unsigned int i = 0; i < mKoopas.size(); i++)
		{
			if (mKoopas[i]->GetPosition().y > 0.0f)
			{
				// Is the enemy off screen to the left / right?
				if (mKoopas[i]->GetPosition().x < (float(-mKoopas[i]->GetCollisionBox().width * 0.5f)))
				{
					mKoopas[i]->SetPosition(Vector2D(mKoopas[i]->GetPosition().x + 15, mKoopas[i]->GetPosition().y));
					mKoopas[i]->Flip();
				}
				else if (mKoopas[i]->GetPosition().x > SCREEN_WIDTH - (float)(mKoopas[i]->GetCollisionBox().width * 0.55f))
				{
					mKoopas[i]->SetPosition(Vector2D(mKoopas[i]->GetPosition().x - 15, mKoopas[i]->GetPosition().y));
					mKoopas[i]->Flip();
				}
			}

			// Update enemy
			mKoopas[i]->Update(deltaTime, e);

			// Player collission
			if ((mKoopas[i]->GetPosition().y > 300.0f || mKoopas[i]->GetPosition().y <= 64.0f) && (mKoopas[i]->GetPosition().x < 64.0f || mKoopas[i]->GetPosition().x > SCREEN_WIDTH - 96.0f))
			{
				// Ignore the collisions if the enemy is behind a pipe?
			}
			else
			{
				bool collided = false;
				// Check if koopas collided with mario
				if (Collisions::Instance()->Circle(Circle2D(mKoopas[i]->GetCollisionRadius(), mKoopas[i]->GetPosition()), Circle2D(characterMario->GetCollisionRadius(), characterMario->GetPosition())))
				{
					collided = true;
				}

				// Check if koopas collided with mario
				if (Collisions::Instance()->Circle(Circle2D(mKoopas[i]->GetCollisionRadius(), mKoopas[i]->GetPosition()), Circle2D(characterLuigi->GetCollisionRadius(), characterLuigi->GetPosition())))
				{
					collided = true;
				}

				if (collided)
				{
					// Check to see if injured
					if (mKoopas[i]->IsInjured())
					{
						mKoopas[i]->SetAlive(false);
						// Add score
						GameManager::GetInstance()->AddScore(100);
					}

					// Kill mario
					// TODO, IMPLEMENT CHARACTER STATE MACHINE
					characterMario->SetAlive(false);
				}

				// If the enemy is no longer alive, schdule it for deletion
				if (!mKoopas[i]->GetAlive())
				{
					enemyIndexToDelete = i;
				}
			}
		}

		// Remove a dead enemy, 1 each update
		if (enemyIndexToDelete != -1)
		{
			mKoopas.erase(mKoopas.begin() + enemyIndexToDelete);
		}
	}
}

void GameScreenLevel1::CreateKoopa(Vector2D position, FACING direction, float speed)
{
	mKoopas.push_back(new CharacterKoopa(mRenderer, "Textures/Koopa.png", position, tileMap, speed, direction));
}

void GameScreenLevel1::CreateCoin(Vector2D position)
{
	mCoins.push_back(new Coin(mRenderer, "Textures/Coin.png", position));
}

void GameScreenLevel1::UpdateCoins(float deltaTime, SDL_Event e)
{
	if (!mCoins.empty())
	{
		coinIndexToDelete = -1;

		// Updates
		for (unsigned int i = 0; i < mCoins.size(); i++)
		{
			// Update coins
			mCoins[i]->Update(deltaTime, e);

			bool collided = false;

			// Check if mario collides with coin
			if (Collisions::Instance()->Circle(Circle2D(mCoins[i]->GetCollisionRadius(), mCoins[i]->GetPosition()), Circle2D(characterMario->GetCollisionRadius(), characterMario->GetPosition())))
			{
				collided = true;
			}

			// Check if luigi collides with coin
			if (Collisions::Instance()->Circle(Circle2D(mCoins[i]->GetCollisionRadius(), mCoins[i]->GetPosition()), Circle2D(characterLuigi->GetCollisionRadius(), characterLuigi->GetPosition())))
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
			mCoins.erase(mCoins.begin() + coinIndexToDelete);
		}
	}

	
}

void GameScreenLevel1::UpdateQuestionMarkBlocks(float deltaTime, SDL_Event e)
{
	for (int i = 0; i < tileMap->mTileMap.size(); i++)
	{
		if (tileMap->mTileMap[i]->GetBlock() != nullptr) {

			if (tileMap->mTileMap[i]->GetBlock()->GetBlockType() == Block::BlockType::BLOCK_QUESTION_MARK)
			{
				// Check if block collides with mario & luigi
				bool collided = false;

				if (Collisions::Instance()->Box(characterMario->GetCollisionBox(), Rect2D(tileMap->mTileMap[i]->GetBlock()->GetPosition().x, tileMap->mTileMap[i]->GetBlock()->GetPosition().y + (tileMap->mTileMap[i]->GetBlock()->GetHeight() - 2), tileMap->mTileMap[i]->GetBlock()->GetWidth(), 2)))
				{
					collided = true;
				}

				if (Collisions::Instance()->Box(characterLuigi->GetCollisionBox(), Rect2D(tileMap->mTileMap[i]->GetBlock()->GetPosition().x, tileMap->mTileMap[i]->GetBlock()->GetPosition().y + (tileMap->mTileMap[i]->GetBlock()->GetHeight() - 2), tileMap->mTileMap[i]->GetBlock()->GetWidth(), 2)))
				{
					collided = true;
				}

				if (collided)
				{
					if (tileMap->mTileMap[i]->GetBlock()->IsAvailable())
					{
						tileMap->mTileMap[i]->GetBlock()->SetAvailable(false);
						CreateCoin(Vector2D(tileMap->mTileMap[i]->GetBlock()->GetPosition().x + 3, tileMap->mTileMap[i]->GetBlock()->GetPosition().y - 32));
						break;
					}
					break;
				}
			}
		}
	}
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

	int** map;
	map = new int* [rows];

	// Predefine map
	for (unsigned int i = 0; i < rows; i++)
		map[i] = new int[columns];

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
	tileMap = new TileMap(mRenderer, this);
	tileMap->GenerateTileMap(map, rows, columns);

	// Let mario and luigi know what map they're on
	characterMario->mCurrentTileMap = tileMap;
	characterLuigi->mCurrentTileMap = tileMap;

}

bool GameScreenLevel1::SetUpLevel()
{
	// Load the background texture
	mBackgroundTexture = new Sprite(mRenderer);
	if (!mBackgroundTexture->LoadFromFile("Textures/BackgroundMB.png"))
	{
		std::cout << "Failed to load background texture!";
		return false;
	}

	screenShake = new ScreenShake();

	// Set up the player character
	characterMario = new CharacterMario(mRenderer, "Textures/mario-run.png", Vector2D(64, 330), tileMap);
	characterLuigi = new CharacterLuigi(mRenderer, "Textures/luigi-run.png", Vector2D(364, 330), tileMap);

	SetUpTileMap();

	// Setup score
	std::string str = "Score: " + std::to_string(GameManager::GetInstance()->GetScore());
	const char* score = str.c_str();

	GameManager::GetInstance()->mScoreText = new UIText(mRenderer, str.c_str(), { 255, 255, 255, 0 });
	GameManager::GetInstance()->mScoreText->Position = new Vector2D(15, 15);

	// Create PowBlock
	mPowBlock = new PowBlock(mRenderer);

	// Create Koopas
	CreateKoopa(Vector2D(150, 32), FACING::FACING_RIGHT, 75.0f);
	CreateKoopa(Vector2D(325, 32), FACING::FACING_LEFT, 75.0f);

	// Create flag
	flag = new Flag(mRenderer, Vector2D(6124, 122), characterMario, NULL);


	return true;
}

#include "GameSceenLevel1.h"
#include <iostream>
#include "Texture2D.h"
#include "Collisions.h"

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer) : GameScreen(renderer)
{
	mLevelMap = NULL;
	SetUpLevel();
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
}

void GameScreenLevel1::Render()
{
	// Draw the background
	mBackgroundTexture->Render(Vector2D(0, screenShake->GetBackgroundYPos()), SDL_FLIP_NONE);

	// Draw the player
	characterMario->Render();
	characterLuigi->Render();

	// Draw Powblock
	mPowBlock->Render();

	// Draw enemies
	for (unsigned int i = 0; i < mKoopas.size(); i++)
	{
		mKoopas[i]->Render();
	}
}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e)
{
	// Update the player
	characterMario->Update(deltaTime, e);
	characterLuigi->Update(deltaTime, e);

	// Check collisions on players
	if (Collisions::Instance()->Circle(Circle2D(characterMario->GetCollisionRadius(), characterMario->GetPosition()),
		Circle2D(characterLuigi->GetCollisionRadius(), characterLuigi->GetPosition())))
	{
		std::cout << "Players collided with each other" << std::endl;
	}

	// Update pow block
	UpdatePowBlock();

	// Update enemies
	UpdateEnemies(deltaTime, e);

	// Update screenshake, passing the koopa vector in order for the pow block to kill them
	screenShake->Update(deltaTime, mKoopas);
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
			// Check if enemy is on the bottom row of tiles
			if (mKoopas[i]->GetPosition().y > 0.0f)
			{
				// Is the enemy off screen to the left / right?
				if (mKoopas[i]->GetPosition().x < (float(-mKoopas[i]->GetCollisionBox().width * 0.5f)))
				{
					//mKoopas[i]->SetAlive(false);
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
				// Check if koopas collided with mario
				if (Collisions::Instance()->Circle(Circle2D(mKoopas[i]->GetCollisionRadius(), mKoopas[i]->GetPosition()), Circle2D(characterMario->GetCollisionRadius(), characterMario->GetPosition())))
				{
					// Check to see if injured
					if (mKoopas[i]->IsInjured())
						mKoopas[i]->SetAlive(false);

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
	mKoopas.push_back(new CharacterKoopa(mRenderer, "Textures/Koopa.png", position, mLevelMap, speed, direction));
}

void GameScreenLevel1::SetLevelMap()
{
	// Set up map array
	int map[MAP_HEIGHT][MAP_WIDTH] = { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
										{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
										{ 1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1 },
										{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
										{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
										{ 0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0 },
										{ 1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1 },
										{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
										{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
										{ 1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1 },
										{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
										{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
										{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 } };

	// Clear any old map
	if (mLevelMap != NULL)
		delete mLevelMap;

	// Set new map
	mLevelMap = new LevelMap(map);

	// Create PowBlock
	mPowBlock = new PowBlock(mRenderer, mLevelMap);
}

bool GameScreenLevel1::SetUpLevel()
{
	// Load the background texture
	mBackgroundTexture = new Texture2D(mRenderer);
	if (!mBackgroundTexture->LoadFromFile("Textures/BackgroundMB.png"))
	{
		std::cout << "Failed to load background texture!";
		return false;
	}

	SetLevelMap();

	screenShake = new ScreenShake();

	// Set up the player character
	characterMario = new CharacterMario(mRenderer, "Textures/mario-run.png", Vector2D(64, 330), mLevelMap);
	characterLuigi = new CharacterLuigi(mRenderer, "Textures/luigi-run.png", Vector2D(364, 330), mLevelMap);

	// Create Koopas
	CreateKoopa(Vector2D(150, 32), FACING::FACING_RIGHT, 75.0f);
	CreateKoopa(Vector2D(325, 32), FACING::FACING_LEFT, 75.0f);

	return true;
}

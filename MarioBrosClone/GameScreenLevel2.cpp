#include "GameScreenLevel2.h"


GameScreenLevel2::GameScreenLevel2(SDL_Renderer* renderer) : GameScreen(mRenderer)
{
	levelIsSetup = false;
	mRenderer = renderer;

	SetUpLevel();
}

GameScreenLevel2::~GameScreenLevel2()
{
	delete mBackgroundTexture;
	mBackgroundTexture = nullptr;
}

void GameScreenLevel2::Render()
{
	if (!levelIsSetup) return;

	// Draw the background
	mBackgroundTexture->Render(Vector2D(0, screenShake->GetBackgroundYPos()), SDL_FLIP_NONE);

	// Render all tiles
	tileMap->DrawTileMap();

	// Draw the player
	characterMario->Render();
	characterLuigi->Render();
}

void GameScreenLevel2::Update(float deltaTime, SDL_Event e)
{
	if (!levelIsSetup) return;

	// Update the player
	characterMario->Update(deltaTime, e);
	characterLuigi->Update(deltaTime, e);

	// Update screenshake, passing the koopa vector in order for the pow block to kill them
	//screenShake->Update(deltaTime, mKoopas);
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
	characterMario = new CharacterMario(mRenderer, "Textures/mario-run.png", Vector2D(64, 330), tileMap);
	characterLuigi = new CharacterLuigi(mRenderer, "Textures/luigi-run.png", Vector2D(364, 330), tileMap);

	screenShake = new ScreenShake();

	SetUpTileMap();

	levelIsSetup = true;
	return true;
}

void GameScreenLevel2::SetUpTileMap()
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
										{ 0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1 },
										{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
										{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
										{ 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 } };

	// Create new TileMap
	tileMap = new TileMap(mRenderer);
	//tileMap->GenerateTileMap(map);
	tileMap->GenerateTileMap(map);

	// Let mario and luigi know what map they're on
	characterMario->mCurrentTileMap = tileMap;
	characterLuigi->mCurrentTileMap = tileMap;

}

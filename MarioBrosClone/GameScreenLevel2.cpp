#include "GameScreenLevel2.h"
#include "SoundManager.h"
#include <fstream>


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
		soundmanager::SoundManager::GetInstance()->PlayMusic("Music/Mariov2.wav");
	}
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

	int** map;
	map = new int*[rows];

	// Predefine map
	for (unsigned int i = 0; i < rows; i++)
	{
		map[i] = new int[columns];
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

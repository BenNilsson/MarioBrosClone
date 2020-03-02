#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Constants.h"
#include <iostream>
#include "Sprite.h"
#include "Commons.h"
#include "GameScreenManager.h"
#include "SoundManager.h"

// Globals
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

GameScreenManager* gameScreenManager;
Uint32 gOldTime;

// Function Prototypes
void CloseSDL();
bool InitSDL();
void Render();
bool Update();

int main(int argc, char* args[])
{
	// Check if SDL was set up correctly
	if (InitSDL())
	{
		// Play background music
		soundmanager::SoundManager::GetInstance()->PlayMusic("Music/Mario.wav");

		// Adjust volume
		Mix_Volume(-1, SDL_MIX_MAXVOLUME / 4);

		Mix_VolumeMusic(SDL_MIX_MAXVOLUME / 6);
		
		// Set up GameScreenManager
		gameScreenManager = new GameScreenManager(gRenderer, SCREEN_INTRO);

		// Set start time
		gOldTime = SDL_GetTicks();


		// Flag to check if the user wishes to exit
		bool quit = false;

		// Game Loop
		while(!quit)
		{
			Render();
			quit = Update();
		}
	}

	

	// Close Window and free resources
	CloseSDL();

	return 0;
}

bool InitSDL()
{
	// Setup SDL
	if (SDL_Init(SDL_INIT_VIDEO < 0))
	{
		std::cout << "SDL did not initialise. Error: " << SDL_GetError();
		return false;
	}
	else
	{
		// Attempt to create window
		gWindow = SDL_CreateWindow("Super Mario Bros 2 Clone",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);

		// Check if the window was created
		if (gWindow == NULL)
		{
			// Window was not created
			std::cout << "Window was note created. Error: " << SDL_GetError();
			return false;
		}
		else
		{
			// Window was created
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer != NULL)
			{
				// Initialise PNG Loading
				int imageFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imageFlags) & imageFlags))
				{
					std::cout << "SDL_Image could not initialise. Error: " << IMG_GetError();
					return false;
				}

			}
			else
			{
				std::cout << "Renderer could not initialise. Eroror:" << SDL_GetError();
				return false;
			}
		}
	}

	return true;
}

void Render()
{
	// Clear the screen - Black
	SDL_RenderClear(gRenderer);
	SDL_SetRenderDrawColor(gRenderer, 99, 160, 255, 0x00);

	// Render gameScreenManager
	gameScreenManager->Render();

	// Update screen
	SDL_RenderPresent(gRenderer);
}


void CloseSDL()
{
	// Clear up textures

	// Release the window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	// Destroy game screen manager
	delete gameScreenManager;
	gameScreenManager = NULL;

	// Quit SDL subsystems
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool Update()
{
	// Event Handler
	SDL_Event e;

	// Get events
	SDL_PollEvent(&e);

	// get the new time
	Uint32 newTime = SDL_GetTicks();

	// Handle events
	switch (e.type)
	{
		// Click 'X' to exit
		case SDL_QUIT:
			return true;
			break;

		// KeyUP
		case SDL_KEYUP:
			// Get Key Pressed
			switch(e.key.keysym.sym)
			{
				// If user hits escape
				case(SDLK_ESCAPE):
					return true;
					break;
				// If the user hits space
				case(SDLK_SPACE):
					gameScreenManager->ChangeScreen(SCREEN_LEVEL1);
					break;
			}
			break;

	}

	// Update gameScreenmanager
	gameScreenManager->Update((float)(newTime - gOldTime) / 1000.f, e);

	// set the current time to be the old time
	gOldTime = newTime;

	return false;
}

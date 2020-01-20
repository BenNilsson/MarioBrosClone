#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Constants.h"
#include <iostream>
#include "Texture2D.h"
#include "Commons.h"

// Globals
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
Texture2D* gTexture = NULL;

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
		gWindow = SDL_CreateWindow("Close Game Simulator",
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

				// Load the background texture
				gTexture = new Texture2D(gRenderer);
				if (!gTexture->LoadFromFile("Textures/test.bmp")) return false;

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
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(gRenderer);

	gTexture->Render(Vector2D(), SDL_FLIP_NONE);

	// Update screen
	SDL_RenderPresent(gRenderer);
}


void CloseSDL()
{
	//// Clear up textures
	delete gTexture;
	gTexture = NULL;

	// Release the window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	// Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool Update()
{
	// Event Handler
	SDL_Event e;

	// Get events
	SDL_PollEvent(&e);

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
			}
			break;

	}

	return false;
}

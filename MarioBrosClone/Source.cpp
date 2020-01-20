#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Constants.h"
#include <iostream>

// Globals
SDL_Window* gWindow = NULL;

// Variables
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;

// Function Prototypes
void CloseSDL();
void FreeTexture();
bool InitSDL();
SDL_Texture* LoadTextureFromFile(std::string path);
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

				// Load Texture
				gTexture = LoadTextureFromFile("Textures/test.bmp");
				if (gTexture == NULL) return false;
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

SDL_Texture* LoadTextureFromFile(std::string path)
{
	// Remove memory used for previous texture
	FreeTexture();

	SDL_Texture* pFinalTexutre = NULL;

	// Load Image
	SDL_Surface* pSurface = IMG_Load(path.c_str());
	if (pSurface != NULL)
	{
		// Create the texture from the pixels on the surface
		pFinalTexutre = SDL_CreateTextureFromSurface(gRenderer, pSurface);
		if (pFinalTexutre == NULL)
			std::cout << "Unable to create texture from surface.Error: " << SDL_GetError() << std::endl;
		
		// Free the loaded surface now that we have the texture
		SDL_FreeSurface(pSurface);
	}
	else
	{
		std::cout << "Unable to load image. Error: " << IMG_GetError() << std::endl;
	}

	return pFinalTexutre;
}

void CloseSDL()
{
	// Clear up textures
	FreeTexture();

	// Release the window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	// Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void FreeTexture()
{
	// Check if the texture exists before removing it
	if (gTexture != NULL)
	{
		SDL_DestroyTexture(gTexture);
		gTexture = NULL;
	}
}

void Render()
{
	// Clear the screen
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	// Set where to render the texture
	SDL_Rect renderLocation = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	// Render texture to screen
	SDL_RenderCopyEx(gRenderer, gTexture, NULL, &renderLocation, 0, NULL, SDL_FLIP_NONE);

	// Update screen
	SDL_RenderPresent(gRenderer);
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

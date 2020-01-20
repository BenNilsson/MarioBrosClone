#include "Texture2D.h"
#include <iostream>

Texture2D::Texture2D(SDL_Renderer* renderer)
{
	mRenderer = renderer;
}

Texture2D::~Texture2D()
{
	Free();

	mRenderer = NULL;
}

bool Texture2D::LoadFromFile(std::string path)
{
	// Remove memory used for previous texture
	Free();

	// Load Image
	SDL_Surface* pSurface = IMG_Load(path.c_str());

	if (pSurface != NULL)
	{
		SDL_SetColorKey(pSurface, SDL_TRUE, SDL_MapRGB(pSurface->format, 0, 0xFF, 0xFF));

		// Create the texture from the pixels on the surface
		mTexture = SDL_CreateTextureFromSurface(mRenderer, pSurface);
		if (mTexture == NULL)
			std::cout << "Unable to create texture from surface. Error: " << SDL_GetError() << std::endl;

		mWidth = pSurface->w;
		mHeight = pSurface->h;

		// Free the loaded surface now that we have the texture
		SDL_FreeSurface(pSurface);
	}
	else
	{
		std::cout << "Unable to load image. Error: " << IMG_GetError() << std::endl;
	}

	return mTexture != NULL;
}

void Texture2D::Free()
{
	// Check if the texture exists before removing it
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
	}
}

void Texture2D::Render(Vector2D newPosition, SDL_RendererFlip flip, double angle)
{
	// Clear the screen
	SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(mRenderer);

	// Set where to render the texture
	SDL_Rect renderLocation = { 0, 0, mWidth, mHeight };

	// Render texture to screen
	SDL_RenderCopyEx(mRenderer, mTexture, NULL, &renderLocation, angle, NULL, SDL_FLIP_NONE);

	// Update screen
	SDL_RenderPresent(mRenderer);
}

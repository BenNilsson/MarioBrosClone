#include <iostream>
#include "Sprite.h"

Sprite::Sprite(SDL_Renderer* renderer)
{
	mRenderer = renderer;
}

Sprite::~Sprite()
{
	Free();

	mRenderer = NULL;
}

bool Sprite::LoadFromFile(std::string path)
{
	// Remove memory used for previous texture
	Free();

	// Load Image
	SDL_Surface* pSurface = IMG_Load(path.c_str());

	if (pSurface != NULL)
	{
		SDL_SetColorKey(pSurface, SDL_TRUE, SDL_MapRGB(pSurface->format, 0, 0xFF, 0xFF));

		// Create the texture from the pixels on the surface
		mSprite = SDL_CreateTextureFromSurface(mRenderer, pSurface);
		if (mSprite == NULL)
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

	return mSprite != NULL;
}

void Sprite::Free()
{
	// Check if the texture exists before removing it
	if (mSprite != nullptr)
	{
		SDL_DestroyTexture(mSprite);
		mSprite = NULL;
	}
}

void Sprite::Render(Vector2D newPosition, SDL_RendererFlip flip, double angle)
{
	// Set where to render the texture
	SDL_Rect renderLocation = { newPosition.x, newPosition.y, mWidth, mHeight };

	// Render texture to screen
	SDL_RenderCopyEx(mRenderer, mSprite, NULL, &renderLocation, angle, NULL, flip);
}

void Sprite::Render(SDL_Rect srcRect, SDL_Rect destRect, SDL_RendererFlip flip, double angle)
{
	// Render to screen
	SDL_RenderCopyEx(mRenderer, mSprite, &srcRect, &destRect, angle, NULL, flip);
}

void Sprite::Render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Rect renderLocation = { x, y, mWidth, mHeight };

	if (clip != NULL)
	{
		mWidth = clip->w;
		mHeight = clip->h;
	}

	SDL_RenderCopyEx(mRenderer, mSprite, clip, &renderLocation, angle, center, flip);
}

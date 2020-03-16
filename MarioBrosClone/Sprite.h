#pragma once
#ifndef _SPRITE_H
#define _SPRITE_H

#include "SDL.h"
#include <string>
#include "Commons.h"
#include <SDL_image.h>

class Sprite
{
public:
	Sprite(SDL_Renderer* renderer);
	~Sprite();

	bool LoadFromFile(std::string path);
	void Free();
	void Render(Vector2D newPosition, SDL_RendererFlip flip, double angle = 0.0);
	void Render(SDL_Rect srcRect, SDL_Rect destRect, SDL_RendererFlip flip, double angle = 0.0);
	void Render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip);

	int GetWidth() { return mWidth; }
	int GetHeight() { return mHeight; }

private:
	SDL_Renderer* mRenderer;
	SDL_Texture* mSprite;

	int mWidth;
	int mHeight;
};

#endif // _SPRITE_

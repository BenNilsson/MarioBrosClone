#pragma once

#include "SDL.h"
#include "SDL_ttf.h"
#include "Commons.h"
#include <string>

class UIText
{
public:
	UIText(SDL_Renderer* renderer);
	UIText(SDL_Renderer* renderer, const char*, SDL_Color color, int width = 100, int height = 25);
	~UIText();

	void Draw();
	const char* Text;
	Vector2D* Position;

	TTF_Font* default_font;
	SDL_Surface* mSurface;
	SDL_Texture* mTexture;
	SDL_Color mColor;
	SDL_Renderer* mRenderer;

private:
	SDL_Rect mRect;
};


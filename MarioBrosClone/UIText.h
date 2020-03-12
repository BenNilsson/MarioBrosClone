#pragma once

#include "SDL.h"
#include "SDL_ttf.h"
#include "Commons.h"
#include <string>

class UIText
{
public:
	UIText(SDL_Renderer* renderer);
	UIText(SDL_Renderer* renderer, const char*, SDL_Color color);
	~UIText();

	void Draw(Vector2D position);
	const char* Text;


private:
	TTF_Font* default_font;
	SDL_Surface* mSurface;
	SDL_Rect mRect;
	SDL_Texture* mTexture;
	SDL_Renderer* mRenderer;
};


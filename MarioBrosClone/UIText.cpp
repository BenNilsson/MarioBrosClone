#include "UIText.h"



UIText::UIText(SDL_Renderer* renderer)
{
	mRenderer = renderer;
	default_font = TTF_OpenFont("Fonts/04B_30__.ttf", 24);
}

UIText::UIText(SDL_Renderer* renderer, const char* text, SDL_Color color)
{
	mRenderer = renderer;
	default_font = TTF_OpenFont("Fonts/04B_30__.ttf", 24);
	Text = text;
	mColor = color;

	mSurface = TTF_RenderText_Solid(default_font, Text, mColor);
	mTexture = SDL_CreateTextureFromSurface(mRenderer, mSurface);
}


UIText::~UIText()
{
	delete Text;
	Text = nullptr;

	delete default_font;
	default_font = nullptr;

	SDL_FreeSurface(mSurface);
	delete mSurface;
	mSurface = nullptr;

	delete mTexture;
	mTexture = nullptr;

	delete mRenderer; 
	mRenderer = nullptr;
}

void UIText::Draw()
{
	//mSurface = TTF_RenderText_Solid(default_font, Text, mColor);
	//mTexture = SDL_CreateTextureFromSurface(mRenderer, mSurface);

	mRect.x = Position->x;
	mRect.y = Position->y;
	mRect.w = 100;
	mRect.h = 25;

	SDL_RenderCopy(mRenderer, mTexture, NULL, &mRect);
}

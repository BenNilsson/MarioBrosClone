#include "UIText.h"



UIText::UIText(SDL_Renderer* renderer)
{
	mRenderer = renderer;
}

UIText::UIText(SDL_Renderer* renderer, const char* text, SDL_Color color)
{
	mRenderer = renderer;
	Text = text;
	mSurface = TTF_RenderText_Solid(default_font, text, color);
	mTexture = SDL_CreateTextureFromSurface(mRenderer, mSurface);
}


UIText::~UIText()
{
	delete Text;
	Text = nullptr;

	delete default_font;
	default_font = nullptr;

	delete mSurface;
	mSurface = nullptr;

	delete mTexture;
	mTexture = nullptr;

	delete mRenderer; 
	mRenderer = nullptr;
}

void UIText::Draw(Vector2D position)
{
	mRect.x = position.x;
	mRect.y = position.y;
	mRect.w = mSurface->w;
	mRect.h = mSurface->h;

	SDL_RenderCopy(mRenderer, mTexture, NULL, &mRect);
}

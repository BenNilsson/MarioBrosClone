#include "Flag.h"
#include <iostream>
#include "Collisions.h"


Flag::Flag(SDL_Renderer* renderer, Vector2D position, Character* character, SCREENS* screenToChangeTo)
{
	mRenderer = renderer;
	mPosition = position;
	mCharacter = character;
	mScreenToChangeTo = screenToChangeTo;

	//mGameScreenManager = new GameScreenManager(mRenderer, SCREEN_INTRO);

	mSprite = new Sprite(mRenderer);

	if (!mSprite->LoadFromFile("Textures/flag.png"))
		std::cout << "Could not load flag image file";

	mCollisionBox = Rect2D(mPosition.x + (mSprite->GetWidth() * 0.50f), mPosition.y, 5, mSprite->GetHeight());
}


Flag::~Flag()
{
	delete mSprite;
	mSprite = NULL;

	delete mRenderer;
	mRenderer = NULL;

	delete mScreenToChangeTo;
	mScreenToChangeTo = NULL;
}

void Flag::Update()
{
	
}

void Flag::Render(int camX, int camY)
{
	mSprite->Render(mPosition.x - camX, mPosition.y - camY, nullptr, 0.0, nullptr, SDL_FLIP_NONE);
}

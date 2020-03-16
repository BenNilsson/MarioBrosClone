#pragma once
#include "Sprite.h"
#include "Character.h"

class Flag
{
public:
	Flag(SDL_Renderer* renderer, Vector2D position, Character* character, SCREENS* screenToChangeTo);
	~Flag();

	const Rect2D& GetCollisionBox() { return mCollisionBox; }
	const Vector2D& GetPosition() { return mPosition; }

	void Update();
	void Render(int camX, int camY);

private:
	SDL_Renderer* mRenderer;
	Sprite* mSprite;
	Rect2D mCollisionBox;
	Vector2D mPosition;

	SCREENS* mScreenToChangeTo;
	Character* mCharacter;
};


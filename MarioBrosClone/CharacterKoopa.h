#pragma once
#include "SDL.h"
#include "Character.h"
#include "Commons.h"

class CharacterKoopa : public Character
{
public:
	CharacterKoopa(SDL_Renderer* renderer, std::string imagePath, Vector2D startPos, TileMap* map, float speed, FACING direction, bool smart = false);
	~CharacterKoopa();

	void TakeDamage();
	void Jump();

	bool IsInjured() { return mInjured; }

	void Update(float deltaTime, SDL_Event e);
	void Render(int camX, int camY);

	float GetUpdateRange() { return mUpdateRange; }

	bool IsSmart() { return mSmartKoopa; }

	void Flip();

private:

	float mUpdateRange;
	bool mSmartKoopa;
	bool mInjured;
	float mInjuredTime;
};


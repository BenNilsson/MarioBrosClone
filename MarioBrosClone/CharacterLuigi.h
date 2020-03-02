#pragma once
#include "SDL.h"
#include "Character.h"
#include "Commons.h"

class CharacterLuigi : public Character
{
public:
	CharacterLuigi(SDL_Renderer* renderer, std::string imagePath, Vector2D startPos, TileMap* map);
	~CharacterLuigi();

	void Update(float deltaTime, SDL_Event e);
	void Render();

};

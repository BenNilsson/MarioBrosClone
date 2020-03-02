#pragma once
#include "SDL.h"
#include "Character.h"
#include "Commons.h"

class CharacterMario : public Character
{
public:
	CharacterMario(SDL_Renderer* renderer, std::string imagePath, Vector2D startPos, TileMap* map);
	~CharacterMario();

	void Update(float deltaTime, SDL_Event e);
	void Render();

};

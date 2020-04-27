#pragma once

#include "Block.h"

class BlockQuestionMark : public Block
{
public:

	BlockQuestionMark(SDL_Renderer* renderer, std::string imagePath, BlockType type, Vector2D pos);
	~BlockQuestionMark();

	void Update(float deltaTime, SDL_Event e);
	void Render(int camX, int camY);

	

private:

	bool coinSpawned;

};


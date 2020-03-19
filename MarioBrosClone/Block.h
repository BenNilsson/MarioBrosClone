#pragma once
#include <SDL.h>
#include "Sprite.h"
#include "Commons.h"

class Block
{
public:

	enum class BlockType
	{
		BLOCK_AIR,
		BLOCK_PLATFORM,
		BLOCK_FLOOR,
		BLOCK_QUESTION_MARK,
		BLOCK_STEP,
		BLOCK_PIPE
	};

	Block(SDL_Renderer* renderer, std::string imagePath, BlockType type, Vector2D pos);
	~Block();

	virtual void Update(float deltaTime, SDL_Event e);
	virtual void Render(int camX, int camY);

	Rect2D GetCollisionBox() { return Rect2D(mPosition.x, mPosition.y, mSingleSpriteWidth, mSingleSpriteHeight); }

	BlockType GetBlockType() { return mBlockType; }

	bool IsAvailable() { return mAvailable; }
	void SetAvailable(bool available);

	float GetWidth();
	float GetHeight();

	Vector2D GetPosition() { return mPosition; }
	void SetPosition(Vector2D pos);

protected:
	SDL_Renderer* mRenderer;
	Sprite* mSprite;
	Vector2D mPosition;

	BlockType mBlockType;

	bool mAvailable;

	float mSingleSpriteWidth;
	float mSingleSpriteHeight;
};


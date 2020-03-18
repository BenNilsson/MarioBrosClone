#pragma once
#include "Sprite.h"
#include "Commons.h"
#include "Block.h"

class Tile
{
public:

	Tile(Block* block, CollisionType type);
	~Tile();

	static const int width = 32;
	static const int height = 32;

	const CollisionType& GetCollisionType() const;

	const void Render(int camX, int camY) const;

	Block* GetBlock() { return mBlock; }

private:
	Block* mBlock;
	CollisionType mCollisionType;
};


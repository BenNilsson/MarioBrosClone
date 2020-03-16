#include "Tile.h"

Tile::Tile(Block* block, CollisionType type)
{
	if (block != nullptr) mBlock = block;
	else block = nullptr;
	mCollisionType = type;
}

Tile::~Tile()
{
	delete mBlock;
	mBlock = nullptr;
}

const CollisionType& Tile::GetCollisionType() const
{
	return mCollisionType;
}

const void Tile::Render(int camX, int camY) const
{
	if(mBlock != nullptr)
		mBlock->Render(camX, camY);
}

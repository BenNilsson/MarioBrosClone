#include "Tile.h"

Tile::Tile(Vector2D pos, Sprite* sprite, CollisionType type)
{
	mPosition = pos;
	mSprite = sprite;
	mCollisionType = type;
}

Tile::~Tile()
{
	delete mSprite;
	mSprite = nullptr;
}

const Vector2D& Tile::GetPosition() const
{
	return mPosition;
}

const Sprite* Tile::GetSprite() const
{
	return mSprite;
}

const CollisionType& Tile::GetCollisionType() const
{
	return mCollisionType;
}

const void Tile::Render() const
{
	if(mSprite != nullptr)
		mSprite->Render(GetPosition(), SDL_FLIP_NONE);
}

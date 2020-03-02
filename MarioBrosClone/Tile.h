#pragma once
#include "Sprite.h"
#include "Commons.h"

class Tile
{
public:
	Tile(Vector2D pos, Sprite* sprite, CollisionType type);
	~Tile();

	static const int width = 32;
	static const int height = 32;

	const Vector2D& GetPosition() const;
	const Sprite* GetSprite() const;
	const CollisionType& GetCollisionType() const;

	const void Render() const;

private:
	Sprite* mSprite;
	Vector2D mPosition;
	CollisionType mCollisionType;

};


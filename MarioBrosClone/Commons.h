#pragma once

struct Vector2D
{
	float x;
	float y;

	Vector2D()
	{
		x = 0.0f;
		y = 0.0f;
	}

	Vector2D(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
};

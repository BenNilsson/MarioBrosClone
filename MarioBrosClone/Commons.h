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

	Vector2D(float _x, float _y)
	{
		x = _x;
		y = _y;
	}
};

struct Rect2D
{
	Rect2D()
	{
		x = 0.0f;
		y = 0.0f;
		width = 0.0f;
		height = 0.0f;
	}

	Rect2D(float X, float Y, float Width, float Height)
	{
		x = X;
		y = Y;
		width = Width;
		height = Height;
	}

	float x;
	float y;
	float width;
	float height;
};

struct Circle2D
{
	Circle2D()
	{
		circleRadius = 0.0f;
		position = Vector2D(0.0f, 0.0f);
	}

	Circle2D(float radius, Vector2D vector)
	{
		circleRadius = radius;
		position = vector;
	}

	float circleRadius;
	Vector2D position;
};

enum SCREENS
{
	SCREEN_INTRO = 0,
	SCREEN_MENU,
	SCREEN_LEVEL1,
	SCREEN_LEVEL2,
	SCREEN_GAMEOVER,
	SCREEN_HIGHSCORES
};

enum FACING
{
	FACING_LEFT = 0,
	FACING_RIGHT
};

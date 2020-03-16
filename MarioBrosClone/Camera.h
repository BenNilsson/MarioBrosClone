#pragma once
#ifndef _CAMERA_H
#define _CAMERA_H

#include "SDL.h"
#include "Commons.h"

class Camera
{

#define CAMERA_WIDTH 512
#define CAMERA_HEIGHT 412

public:

	static Camera* GetInstance()
	{
		if (instance == 0)
		{
			instance = new Camera();
			instance->Init();
		}

		return instance;
	}

	void SetPosition(Vector2D pos);
	Vector2D GetPosition() { return Vector2D(mCamera.x, mCamera.y); }

	SDL_Rect GetCameraBounds() { return mCamera; }

private:
	static Camera* instance;

	static SDL_Rect mCamera;

	void Init()
	{
		mCamera.w = CAMERA_WIDTH;
		mCamera.h = CAMERA_HEIGHT;
	}

	~Camera();
};

#endif // !_CAMERA_H


#include "Camera.h"

// Initialise static variables
Camera* Camera::instance = 0;
SDL_Rect Camera::mCamera = { 0, 0, 0, 0 };


void Camera::SetPosition(Vector2D pos)
{
	mCamera.x = pos.x;
	mCamera.y = pos.y;
}


Camera::~Camera()
{
	delete instance;
	instance = nullptr;
}

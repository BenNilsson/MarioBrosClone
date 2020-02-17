#include "ScreenShake.h"
#include "SDL.h"
#include <iostream>

ScreenShake::ScreenShake()
{
	mScreenshake = false;
	mBackgroundYPos = 0.0f;
}

ScreenShake::~ScreenShake()
{
}

void ScreenShake::DoScreenShake()
{
	mScreenshake = true;
	mScreenshakeTime = SCREENSHAKE_DURATION;
	mWobble = 0.0f;
}

void ScreenShake::Update(float deltaTime)
{
	// Shake screen if required
	if (mScreenshake)
	{
		mScreenshakeTime -= deltaTime;
		mWobble++;
		mBackgroundYPos = sin(mWobble);
		mBackgroundYPos *= 3.0f;
		// End screen shake after duration
		if (mScreenshakeTime <= 0.0f)
		{
			mScreenshake = false;
			mBackgroundYPos = 0.0f;
		}
	}
}

#include "ScreenShake.h"
#include "SDL.h"
#include <iostream>
#include "CharacterKoopa.h"

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

void ScreenShake::Update(float deltaTime, std::vector<CharacterKoopa*> _koopas)
{
	// Shake screen if required
	if (mScreenshake)
	{
		mScreenshakeTime -= deltaTime;
		mWobble++;
		mBackgroundYPos = sin(mWobble);
		mBackgroundYPos *= 3.0f;

		// Damage all koopas
		for (unsigned int i = 0; i < _koopas.size(); i++)
		{
			_koopas[i]->TakeDamage();
		}

		// End screen shake after duration
		if (mScreenshakeTime <= 0.0f)
		{
			mScreenshake = false;
			mBackgroundYPos = 0.0f;
		}
	}
}

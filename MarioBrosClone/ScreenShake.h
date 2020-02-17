#include "Constants.h"
#pragma once

class ScreenShake
{
public:
	ScreenShake();
	~ScreenShake();

	void DoScreenShake();
	void Update(float deltaTime);

	float GetBackgroundYPos() { return mBackgroundYPos; }

private:
	bool mScreenshake;
	float mScreenshakeTime;
	float mWobble;
	float mBackgroundYPos;
};


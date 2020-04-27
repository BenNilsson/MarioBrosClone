#pragma once
#include "Constants.h"
#include "CharacterKoopa.h"

#include <vector>

class ScreenShake
{
public:
	ScreenShake();
	~ScreenShake();

	void DoScreenShake();
	void Update(float deltaTime, std::vector<CharacterKoopa*> _koopas);

	float GetBackgroundYPos() { return mBackgroundYPos; }

private:
	bool mScreenshake;
	float mScreenshakeTime;
	float mWobble;
	float mBackgroundYPos;
};


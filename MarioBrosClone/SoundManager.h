#pragma once
#ifndef _SOUNDMANAGER_H
#define _SOUNDMANAGER_H

#include "SDL.h"
#include "SDL_mixer.h"
#include <string>
#include <iostream>

namespace soundmanager {

	class SoundManager
	{
	public:

		static SoundManager* GetInstance()
		{
			if (instance == 0)
			{
				instance = new SoundManager();
				SoundManager::Init();
			}

			return instance;
		}

		void PlayMusic(std::string path);
		void StopMusic();
		void PlaySFX(std::string path);

		inline bool IsStopped() const { return currentState == STOPPED; }
		inline bool IsPlaying() const { return currentState == PLAYING; }

	private:
		static SoundManager* instance;

		enum AudioState
		{
			ERROR = 0,
			WAITING,
			STOPPED,
			PLAYING
		};

		static AudioState currentState;

		static void Init()
		{
			// Initialise Audio Device
			if (SDL_Init(SDL_INIT_AUDIO) != -1)
			{
				// Initialise the Mixer
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					std::cout << "Mixer could not initialise. Error: " << Mix_GetError();
					currentState = ERROR;
				}
				else
				{
					// Exit error state
					currentState = WAITING;
				}
			}
			else
			{
				std::cerr << "Error initialising SDL audio";
				currentState = ERROR;
			}
		}

		SoundManager() {}

		~SoundManager() { Mix_CloseAudio(); }
	};




}

#endif


#include "SoundManager.h"
#include "SDL_mixer.h"
#include <string>

using namespace soundmanager;

// Initialise static variables
SoundManager* SoundManager::instance = 0;
SoundManager::AudioState SoundManager::currentState = ERROR;

void SoundManager::PlayMusic(std::string path)
{
	if (currentState != ERROR)
	{
		// Check if any music is playing
		if (Mix_PlayingMusic() == 0)
		{
			// Load Music
			Mix_Music* music = Mix_LoadMUS(path.c_str());

			if (music == NULL)
			{
				std::cerr << "Error loading music file: " << path << std::endl;
			}
			else
			{
				// Play music
				Mix_PlayMusic(music, -1);
				currentState = PLAYING;
			}
		}
		else
		{
			// If music is playing, stop it
			this->StopMusic();
		}
	}
}

void SoundManager::StopMusic()
{
	if (currentState != ERROR)
	{
		Mix_HaltMusic();
		currentState = STOPPED;
	}
}

void SoundManager::PlaySFX(std::string path)
{
	if (currentState != ERROR)
	{
		Mix_Chunk* fx = Mix_LoadWAV(path.c_str());

		if (fx == NULL)
		{
			std::cerr << "Error loading music file: " << path << std::endl;
		}
		else
		{
			Mix_PlayChannel(-1, fx, 0);
		}
	}
}

void soundmanager::SoundManager::PauseMusic()
{
	if (currentState != ERROR)
	{
		if (currentState == PLAYING)
		{
			Mix_PauseMusic();
			currentState = PAUSED;
		}
	}
}

void soundmanager::SoundManager::ResumeMusic()
{
	if (currentState != ERROR)
	{
		if (currentState == PAUSED)
		{
			Mix_ResumeMusic();
			currentState = PLAYING;
		}
	}
}



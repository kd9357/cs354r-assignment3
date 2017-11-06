#include "SoundManager.h"

Mix_Music* SoundManager::bgMusic;
Mix_Chunk* SoundManager::fire;
Mix_Chunk* SoundManager::explode;

SoundManager::SoundManager()
{

}

void SoundManager::initSoundManager()
{
	SDL_Init(SDL_INIT_AUDIO);

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512);

    bgMusic = Mix_LoadMUS("assets/bgMusic.wav");
    Mix_VolumeMusic(40);
    fire = Mix_LoadWAV("assets/8-bit-bounce.wav");
    explode = Mix_LoadWAV("assets/explode.wav");
}

void SoundManager::playMusic()
{
	Mix_PlayMusic(bgMusic, -1);
}

void SoundManager::playSoundEffect(Ogre::String arg)
{
	if(arg == "fire")
		Mix_PlayChannel(-1, fire, 0);
	if(arg == "explode")
		Mix_PlayChannel(-1, explode, 0);
}
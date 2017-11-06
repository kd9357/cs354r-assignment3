#include <Ogre.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include <string>

class SoundManager
{
	public:
		static Mix_Music* bgMusic;
		static Mix_Chunk* fire;
		static Mix_Chunk* explode;

		SoundManager();
		~SoundManager() {}

		static void initSoundManager();
		static void playMusic();
		static void playSoundEffect(Ogre::String arg);
};

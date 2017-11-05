#include <Ogre.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include <string>

class SoundManager
{
	public:
		static Mix_Music* bgMusic;
		static Mix_Chunk* score;
		static Mix_Chunk* fire;

		SoundManager();
		~SoundManager() {}

		static void initSoundManager();
		static void playMusic();
		static void playSoundEffect(Ogre::String arg);
};

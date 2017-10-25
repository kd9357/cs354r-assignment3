#include <Ogre.h>
#include "GameObject.h"

//To include or not to include?
//#include "BaseApplication.h"

class PlayingField : public GameObject
{
	protected:
		// float height;
		// float length;
		// float width;
	public:
		PlayingField(Ogre::SceneManager* scnMgr, Simulator* sim, Ogre::Real width, Ogre::Real height, Ogre::String n);
		~PlayingField();
};

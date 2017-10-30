#include <Ogre.h>
#include "GameObject.h"

class Enemy : public GameObject
{
	public:
		Enemy(Ogre::SceneManager* scnMgr, Simulator* sim, Ogre::String n);
		~Enemy();
};
#include <Ogre.h>
#include "GameObject.h"

class Enemy : public GameObject
{
	protected:
		bool active;
		float timer;
	public:
		Enemy(Ogre::SceneManager* scnMgr, Simulator* sim, Ogre::String n);
		~Enemy();
		void setActive(bool active);
		bool isActive();
		void reset();
		void spawn();
    	void update(float elapsedTime);
};
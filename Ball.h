#include <Ogre.h>
#include "GameObject.h"
// #include <BaseApplication.h>
// #include <OgreEntity.h>
// #include <OgreSceneManager.h>

class Ball : public GameObject
{
	protected:
		Ogre::Real bRadius;
	public:
		Ball(Ogre::SceneManager* scnMgr, Simulator* sim, Ogre::String n);
		~Ball();
		// Ogre::SceneNode* getNode() { return rootNode; }
};
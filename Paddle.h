#include <Ogre.h>
#include "GameObject.h"

class Paddle : public GameObject
{
  protected:
    // float height;
    // float length;
    // float width;
  public:
    Paddle(Ogre::SceneManager* scnMgr, Simulator* sim, Ogre::Real width, Ogre::Real height, Ogre::String n);
    ~Paddle();
};

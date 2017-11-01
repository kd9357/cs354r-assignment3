#include <Ogre.h>
#include "GameObject.h"

class Projectile : public GameObject
{
  protected:
    Ogre::Real bRadius;
    float lastTime;
    int* score;
  public:
    Projectile(Ogre::SceneManager* scnMgr, Simulator* sim, int* score, Ogre::String n, bool isClient);
    ~Projectile();
    void update(float elapsedTime);
      void reset();
};
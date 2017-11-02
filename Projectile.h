#include <Ogre.h>
#include "GameObject.h"

class Projectile : public GameObject
{
  protected:
    Ogre::Real bRadius;
    float lastTime;

    bool isClient;
    int* score;
    bool active;

  public:
    Projectile(Ogre::SceneManager* scnMgr, Simulator* sim, int* score, Ogre::String n, bool isClient);
    ~Projectile();

    bool isActive();
    void setActive(bool b);
    void update(float elapsedTime);
    void reset();
};
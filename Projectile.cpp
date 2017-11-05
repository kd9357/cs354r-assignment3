#include "Projectile.h"
#include "Simulator.h"
#include <iostream>

Projectile::Projectile(Ogre::SceneManager* scnMgr, Simulator* sim, int* s, Ogre::String n, bool isClient) 
  : GameObject(scnMgr, sim, n)
{
  //client and server have different colored projectiles/different scores to update
  Ogre::Entity* ball = scnMgr->createEntity(n, "sphere.mesh");
  ball->setMaterialName("Examples/SphereMappedRustySteel");
  ball->setCastShadows(true);
  rootNode = scnMgr->getRootSceneNode()->createChildSceneNode();
  rootNode->attachObject(ball);
  rootNode->scale(0.01f, 0.01f, 0.01f);
  bRadius = 1.0f;
  mass = 10;
  shape = new btSphereShape(bRadius);
  score = s;

  active = false;
  this->isClient = isClient;
}

Projectile::~Projectile()
{

}

bool Projectile::isActive()
{
  return active;
}

void Projectile::setActive(bool b)
{
  active = b;
}

//Specific game object update routine
void Projectile::update(float elapsedTime) {
  lastTime += elapsedTime;
  simulator->getDynamicsWorld()->contactTest(body, *callback);
  if (context->hit && (lastTime > 0.1 || (context->lastBody != context->body && lastTime > 0.05))) {
    //Handle the hit
    GameObject * collision = callback->ctxt.theObject;
    Ogre::String objName = collision->getName();

    //If hit with enemy, kill self, kill enemy, play sound, update specific player's score
    if(objName.substr(0, 4) == "ogre")
    {
      //Play sound effect
      //Update score
      std::cout << "hit ogre\n";

      reset();
      collision->reset();
    }
    std::cout << "hit something\n";
    lastTime = 0.0f;
  }
  context->hit = false;
}

void Projectile::reset() {
  active = false;
  setVelocity(0, 0, 0);
  getRootNode()->setPosition(-25, -100, 0);
  updateTransform();
  updateWorldTransform();
}

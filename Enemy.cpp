#include "Enemy.h"
#include "Simulator.h"

Enemy::Enemy(Ogre::SceneManager* scnMgr, Simulator* sim, Ogre::String n) : GameObject(scnMgr, sim, n)
{
	Ogre::Entity* enemy = scnMgr->createEntity(n, "ogrehead.mesh");
	rootNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	rootNode->attachObject(enemy);
	rootNode->scale(0.4f, 0.4f, 0.4f);
	rootNode->pitch(Ogre::Degree(90));
	rootNode->setPosition(Ogre::Vector3(0.0f, 10.0f, 0.0f));
  mass = 1;
	shape = new btSphereShape(10.0f);
}

void Enemy::setActive(bool a)
{
  active = a;
}

bool Enemy::isActive()
{
  return active;
}

void Enemy::spawn()
{
  setActive(true);
  timer = 0.0f;
  Ogre::Real x = Ogre::Math::RangeRandom(-20.0f, 20.0f);
  Ogre::Real y = 1000.0f;
  Ogre::Real z = Ogre::Math::RangeRandom(-20.0f, 20.0f);

  getRootNode()->setPosition(x, y, z);
  setVelocity(0, 0, 0);
  updateTransform();
  updateWorldTransform();
}

void Enemy::update(float elapsedTime)
{
  lastTime += elapsedTime;
  timer += elapsedTime;
  if(timer >= 10.0f && isActive())
  {
    reset();
    timer = 0.0f;
    return;
  }
  simulator->getDynamicsWorld()->contactTest(body, *callback);
  if(context->hit && (lastTime > 0.1f || (context->lastBody != context->body && lastTime > 0.05f)))
  {
    Ogre::String objName = callback->ctxt.theObject->getName();
    // std::cout << getName() << " collided with " << objName << std::endl;
    // if(objName.substr(0, 4) != "ogre")
    //   reset();
    lastTime = 0.0f;
  }
  context->hit = false;
}

void Enemy::reset()
{
  //timer = 0;
  setActive(false);
  rootNode->setPosition(0, -1000, 0);
  setVelocity(0, 0, 0);
  updateTransform();
  updateWorldTransform();
}

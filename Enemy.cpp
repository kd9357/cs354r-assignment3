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

void Enemy::update(float elapsedTime)
{
  lastTime += elapsedTime;
  simulator->getDynamicsWorld()->contactTest(body, *callback);
  if(context->hit && (lastTime > 0.1f || (context->lastBody != context->body && lastTime > 0.05f)))
  {
    Ogre::String objName = callback->ctxt.theObject->getName();
    std::cout << getName() << " collided with " << objName << std::endl;
    lastTime = 0.0f;
  }
  context->hit = false;
}
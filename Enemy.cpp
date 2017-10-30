#include "Enemy.h"

Enemy::Enemy(Ogre::SceneManager* scnMgr, Simulator* sim, Ogre::String n) : GameObject(scnMgr, sim, n)
{
	Ogre::Entity* enemy = scnMgr->createEntity(n, "ogrehead.mesh");
	rootNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	rootNode->attachObject(enemy);
	rootNode->scale(0.4f, 0.4f, 0.4f);
	rootNode->pitch(Ogre::Degree(90));
	rootNode->setPosition(Ogre::Vector3(0.0f, 10.0f, 0.0f));

	shape = new btSphereShape(1.0f);
}
// #include <OgreEntity.h>
// #include <OgreSceneManager.h>
#include "GameObject.h"
#include "Simulator.h"
#include <iostream>

GameObject::GameObject(Ogre::SceneManager* scnMgr, Simulator* sim, Ogre::String n)
{
	scnMgr = scnMgr;
	simulator = sim;
	shape = NULL;
	motionState = NULL;
	mass = 0;
	restitution = 1;
	friction = 0;
	inertia.setZero();
	tr.setIdentity();
	context = NULL;
	callback = NULL;
	name = n;
}

void GameObject::updateTransform(){
	Ogre::Vector3 pos = rootNode ->getPosition();
	tr.setOrigin(btVector3(pos.x, pos.y, pos.z));
	Ogre::Quaternion qt = rootNode ->getOrientation();
	tr.setRotation(btQuaternion(qt.x, qt.y, qt.z, qt.w));

	if (motionState) motionState -> updateTransform(tr);
}

void GameObject::addToSimulator(){
	updateTransform();
	motionState = new OgreMotionState(tr, rootNode);
	if(mass != 0.0f) shape -> calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, inertia);
	body = new btRigidBody(rbInfo);
	body->setRestitution(restitution);
	body->setFriction(friction);

	body->setUserPointer(this);
	context = new CollisionContext();
	callback = new BulletContactCallback(*body, *context);

	simulator->addObject(this);
}

void GameObject::setVelocity(Ogre::Real x, Ogre::Real y, Ogre::Real z) {
	body->setLinearVelocity(btVector3(x, y, z));
}

void GameObject::update(float elapsedTime)
{
	lastTime += elapsedTime;
	simulator->getDynamicsWorld()->contactTest(body, *callback);
	if(context->hit && (lastTime > 0.1f || (context->lastBody != context->body && lastTime > 0.05f)))
	{
		std::cout << "collision occurred" << std::endl;
		std::cout << "my name: " << getName() << std::endl;
		std::cout << "collision name: " << callback->ctxt.theObject->getName() << std::endl;
		lastTime = 0.0f;
	}
	context->hit = false;
}
#include <iostream>
#include "Simulator.h"

Simulator::Simulator()
{
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,
												overlappingPairCache,
												solver,
												collisionConfiguration);
  // dynamicsWorld->setGravity(btVector3(0.0, -9.8, 0.0));
  dynamicsWorld->setGravity(btVector3(0.0, -50, 0.0));
}

void Simulator::addObject(GameObject* o)
{
	objList.push_back(o);
	dynamicsWorld->addRigidBody(o->getBody());
}

bool Simulator::removeObject(GameObject* o)
{
	for(int i = 0; i < objList.size(); ++i)
	{
		if(objList[i] == o)
		{
			dynamicsWorld->removeRigidBody(o->getBody());
			objList.erase(objList.begin() + i);
			break;
		}
	}
}

btAlignedObjectArray<btCollisionShape*> Simulator::getCollisionShapes()
{
	return this->collisionShapes;
}

btDiscreteDynamicsWorld* Simulator::getDynamicsWorld()
{
	return this->dynamicsWorld;
}

void Simulator::stepSimulation(const Ogre::Real elapsedTime, int maxSubSteps, const Ogre::Real fixedTimestep)
{
	getDynamicsWorld()->stepSimulation(elapsedTime, maxSubSteps, fixedTimestep);
	for(unsigned int i = 0; i < objList.size(); ++i)
	{
		objList[i]->update(elapsedTime);
	}
}
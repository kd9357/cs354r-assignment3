#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <Ogre.h>
#include <deque>
#include <btBulletDynamicsCommon.h>
#include "GameObject.h"

class Simulator
{
	protected:
		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btBroadphaseInterface* overlappingPairCache;
		btSequentialImpulseConstraintSolver* solver;
		btDiscreteDynamicsWorld* dynamicsWorld;
		btAlignedObjectArray<btCollisionShape*> collisionShapes;
		std::deque<GameObject*> objList;

	public:
		Simulator();
		~Simulator();

		void addObject(GameObject* o);
		bool removeObject(GameObject* o);
		void stepSimulation(const Ogre::Real elapsedTime, int maxSubSteps = 1, 
								const Ogre::Real fixedTimestep = 1.0f/60.0f);
		btAlignedObjectArray<btCollisionShape*> getCollisionShapes(void);
		btDiscreteDynamicsWorld* getDynamicsWorld(void);
		std::deque<GameObject*> getGameObjects(void){return objList;}
};

#endif
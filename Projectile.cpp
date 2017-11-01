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
  rootNode->scale(0.05f, 0.05f, 0.05f);
  bRadius = 5.0f;
  mass = 1;
  shape = new btSphereShape(bRadius);
  score = s;
}

//Specific game object update routine
void Projectile::update(float elapsedTime) {
  lastTime += elapsedTime;
  simulator->getDynamicsWorld()->contactTest(body, *callback);
  if (context->hit && (context->velNorm > 1.0 || context->velNorm < -1.0) 
    && (lastTime > 0.1 || (context->lastBody != context->body && lastTime > 0.05))) {
    //Handle the hit
    Ogre::String objName = callback->ctxt.theObject->getName();

    //If hit with enemy, kill self, kill enemy, play sound, update specific player's score

    //Should kill self after certain time or traveled certain distance (how to track either? collision with invisible back wall instead?)


    lastTime = 0.0f;
  }
  context->hit = false;
}

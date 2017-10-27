#include "Paddle.h"

Paddle::Paddle(Ogre::SceneManager* scnMgr, Simulator* sim, 
               Ogre::Real width, Ogre::Real height, Ogre::String n, bool isClient) 
              : GameObject(scnMgr, sim, n)
{
    Ogre::Entity* paddle = scnMgr->createEntity(n, "RZR-002.mesh");
    if(isClient)
      paddle->setMaterialName("RZR-002");
    else
      paddle->setMaterialName("Examples/SphereMappedRustySteel");
    paddle->setCastShadows(true);
    rootNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    rootNode -> attachObject(paddle);
    rootNode->pitch(Ogre::Degree(90));
    rootNode->scale(0.6f, 0.6f, -0.6f);
    mass = 0.0f;
    shape = new btSphereShape(1.0f);
}
#include "Paddle.h"

Paddle::Paddle(Ogre::SceneManager* scnMgr, Simulator* sim, 
               Ogre::Real width, Ogre::Real height, Ogre::String n) 
              : GameObject(scnMgr, sim, n)
{
  // Creating an upright plane for our paddle
  Ogre::Plane plane(Ogre::Vector3::UNIT_Z, 0);
  Ogre::MeshManager::getSingleton().createPlane("paddle", 
    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 
    width, height, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_X);

  Ogre::Entity* paddleEntity = scnMgr->createEntity("paddle");
  paddleEntity->setCastShadows(true);
  paddleEntity->setMaterialName("Examples/Rockwall");

  rootNode = scnMgr->getRootSceneNode()->createChildSceneNode();
  rootNode->attachObject(paddleEntity);
  //TODO: Find the relationship between plane width/height and btboxshape width/height
  shape = new btBoxShape(btVector3(width / 2.0, 1, height / 2.0));
}
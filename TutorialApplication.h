/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_

#include "BaseApplication.h"
#include "Simulator.h"
#include "Ball.h"
#include "Paddle.h"
#include "NetManager.h"
#include "Enemy.h"
#include "Projectile.h"
#include "SoundManager.h"

//---------------------------------------------------------------------------

class TutorialApplication : public BaseApplication
{
public:
    TutorialApplication(void);
    virtual ~TutorialApplication(void);

protected:
    virtual void createScene(void);
    virtual void createCamera(void);
    virtual void createViewports(void);
    // OIS::MouseListener
    virtual bool mouseMoved(const OIS::MouseEvent& me);
    // Ogre::KeyListener
    virtual bool processUnbufferedInput(const Ogre::FrameEvent& fe);
    virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);
    // Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    //helper functions
    void startNetworking(bool isClient);
    void stopNetworking();

    void fireProjectile();
    Ogre::String createMessage();

    //Simulator and Game objects
    Simulator * sim;
    Ball * ball;
    Paddle * paddle;
    Paddle * paddle2;
    Enemy * enemy;
    Projectile * proj;
    int maxEnemies;
    int maxProjectiles;
    float timer;
    float maxTime;

    const float bound = 20.0f;
    
    std::vector<Projectile*> serverProjectiles;
    std::vector<Projectile*> clientProjectiles;

    //Enemies
    std::vector<Enemy*> enemies;

    //Camera Parameters
    Ogre::SceneNode * mCamNode;
    Ogre::Vector3 mDirection;
    Ogre::Real mMove;
    Ogre::Real mRotate;

    //keyboard Parameters
    bool spacePressed;

    //Networking Parameters
    NetManager netManager;
    bool mNetworkingStarted;
    int mPortNumber;
    char* mIPAddress;
    bool mIsClient;
    bool mMultiplayer;

    //Sound Parameters
    SoundManager soundManager;
};

//---------------------------------------------------------------------------

#endif // #ifndef __TutorialApplication_h_

//---------------------------------------------------------------------------

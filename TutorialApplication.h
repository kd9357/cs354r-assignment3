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
    // Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    //helper functions
    void startNetworking(bool isClient);
    void stopNetworking();

    //Simulator and Game objects
    Simulator * sim;
    Ball * ball;
    Paddle * paddle;
    Paddle * paddle2;
    Enemy * enemy;
    //Camera Parameters
    Ogre::SceneNode * mCamNode;
    Ogre::Vector3 mDirection;
    Ogre::Real mMove;
    Ogre::Real mRotate;

    //Networking Parameters
    NetManager netManager;
    bool mNetworkingStarted;
    int mPortNumber;
    char* mIPAddress;
    bool mIsClient;
    bool mMultiplayer;
};

//---------------------------------------------------------------------------

#endif // #ifndef __TutorialApplication_h_

//---------------------------------------------------------------------------

/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
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
#include <iostream>
#include <sstream>
#include <string>
#include "TutorialApplication.h"

//---------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void)
{
    //Start as single player
    mMultiplayer = false;
    mPortNumber = 51215;
    mNetworkingStarted = false;
    numEnemies = 0;
    maxProjectiles = 5;


    //Hardcoded IP number
    mIPAddress = "128.83.139.157";
}
//---------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
}

//---------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{
    // Create your scene here :)
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.4, 0.4, 0.4));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
    //mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);
    Ogre::Plane plane;
    plane.d = 1000;
    plane.normal = Ogre::Vector3::NEGATIVE_UNIT_Y;
    mSceneMgr->setSkyPlane(true, plane, "Examples/SpaceSkyPlane", 800, 75);

    // Lights setup
    Ogre::Light* pointLight = mSceneMgr->createLight("PointLight");
    pointLight->setType(Ogre::Light::LT_POINT);
    pointLight->setDiffuseColour(1.0, 1.0, 1.0);
    pointLight->setSpecularColour(1.0, 1.0, 1.0);
    pointLight->setPosition(0, 0, 200);

    //Physics setup
    sim = new Simulator();

    //For now, it's positioning will match that of the camera
    paddle = new Paddle(mSceneMgr, sim, 25, 25, "paddle", false);   //client
    paddle->getRootNode()->setPosition(20, 25, 0);

    paddle2 = new Paddle(mSceneMgr, sim, 25, 25, "paddle2", true);  //server
    paddle2->getRootNode()->setPosition(-20, 25, 0);

    paddle->addToSimulator();
    paddle2->addToSimulator();

    enemy = new Enemy(mSceneMgr, sim, "ogre" + Ogre::StringConverter::toString(numEnemies));
    ++numEnemies;
    enemy->getRootNode()->setPosition(0, 1000, 0);
    enemy->addToSimulator();


    //Create projectiles
    int temp;
    for(int i = 0; i < maxProjectiles; ++i)
    {
        Ogre::String sName = "serverProjectile" + Ogre::StringConverter::toString(i);
        Ogre::String cName = "clientProjectile" + Ogre::StringConverter::toString(i);
        Projectile * sProjectile = new Projectile(mSceneMgr, sim, &temp, sName, false);
        Projectile * cProjectile = new Projectile(mSceneMgr, sim, &temp, cName, true);
        serverProjectiles.push_back(sProjectile);
        clientProjectiles.push_back(cProjectile);
        sProjectile->getRootNode()->setPosition(0, -100, 0);
        cProjectile->getRootNode()->setPosition(0, -100, 0);
    }
}
//---------------------------------------------------------------------------
void TutorialApplication::createCamera(void)
{
    mCamera = mSceneMgr->createCamera("PlayerCam");
    // mCamera->lookAt(Ogre::Vector3(0, 25, 0));
    mCamera->setNearClipDistance(5);
    mCamNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode");
    mCamNode->setPosition(Ogre::Vector3(0, -30, 0));
    mCamNode->attachObject(mCamera);
    mCamNode->pitch(Ogre::Degree(90));
    mMove = 0.2;
    mRotate = 0.05;
}
//---------------------------------------------------------------------------
void TutorialApplication::createViewports(void)
{
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}

//---------------------------------------------------------------------------
//Keyboard input
bool TutorialApplication::processUnbufferedInput(const Ogre::FrameEvent& fe)
{
    static bool keyPressed = false;

    //Busy check gameobjects to be deleted

    //Camera controls
    //static bool buttonPressed = false;
    //For now, also move paddle
    Ogre::Vector3 dirVec = mCamera->getPosition();
    if(mKeyboard->isKeyDown(OIS::KC_W))
        dirVec.y += mMove;
    if(mKeyboard->isKeyDown(OIS::KC_S))
        dirVec.y -= mMove;
    if(mKeyboard->isKeyDown(OIS::KC_A))
        dirVec.x -= mMove;
    if(mKeyboard->isKeyDown(OIS::KC_D))
        dirVec.x += mMove;
    if(mKeyboard->isKeyDown(OIS::KC_Q))
    {
        mCamNode->yaw(Ogre::Degree(mRotate), Ogre::Node::TS_WORLD);
        paddle->getRootNode()->yaw(Ogre::Degree(mRotate), Ogre::Node::TS_WORLD);
    }
    if(mKeyboard->isKeyDown(OIS::KC_E))
    {
        mCamNode->yaw(Ogre::Degree(-mRotate), Ogre::Node::TS_WORLD);
        paddle->getRootNode()->yaw(Ogre::Degree(-mRotate), Ogre::Node::TS_WORLD);
    }
    if(mKeyboard->isKeyDown(OIS::KC_I) && !mNetworkingStarted)  //start as server
    {
        startNetworking(false);
    }
    if(mKeyboard->isKeyDown(OIS::KC_O) && !mNetworkingStarted)  //start as client
    {
        startNetworking(true);
        std::cout << "Hostname: " << netManager.getHostname() << std::endl;
    }
    if(mKeyboard->isKeyDown(OIS::KC_P) && mNetworkingStarted)   //end session
    {
        stopNetworking();
    }
    if(mKeyboard->isKeyDown(OIS::KC_L) && mNetworkingStarted ) //send message
    {   
        if(mIsClient)
        {
            Ogre::String message = "helloserver ";
            netManager.messageServer(PROTOCOL_UDP, message.c_str(), message.length());
        }
        else if(netManager.getClients() > 0)
        {
            Ogre::String message = "helloclients ";
            netManager.messageClients(PROTOCOL_UDP, message.c_str(), message.length());
        }
    }
    if(mKeyboard->isKeyDown(OIS::KC_SPACE))
    {
        int temp;
        keyPressed = true;
        if(!mIsClient)
        {
            for(int i = 0; i < maxProjectiles; ++i)
            {
                if(!serverProjectiles[i]->isActive)
                {
                    serverProjectiles[i]->isActive = true;
                    break;
                }
            }
            // Ogre::String name = "serverProjectile" + Ogre::StringConverter::toString(numServerProjectiles);
            // ++numServerProjectiles;
            // Projectile * serverProjectile = new Projectile(mSceneMgr, sim, &temp, name, false);
            // serverProjectile->getRootNode()->setPosition(paddle->getRootNode()->getPosition().x,
            //                                              paddle->getRootNode()->getPosition().y + 20,
            //                                              paddle->getRootNode()->getPosition().z );

            // serverProjectile->addToSimulator();
            // serverProjectile->setVelocity(0, 1000, 0);
        }
        else 
        {
            // Ogre::String name = "ClientProjectile" + Ogre::StringConverter::toString(numClientProjectiles);
            // ++numClientProjectiles;
            // Projectile * ClientProjectile = new Projectile(mSceneMgr, sim, &temp, name, true);
            // ClientProjectile->getRootNode()->setPosition(paddle2->getRootNode()->getPosition().x,
            //                                              paddle2->getRootNode()->getPosition().y + 20,
            //                                              paddle2->getRootNode()->getPosition().z );

            // ClientProjectile->addToSimulator();
            // ClientProjectile->setVelocity(0, 1000, 0);
        }
    }

    //mCamNode->translate(dirVec, Ogre::Node::TS_LOCAL);
    if(mNetworkingStarted)
    {
        if(!mIsClient)
            paddle->getRootNode()->translate(dirVec, Ogre::Node::TS_LOCAL);
        else
            paddle2->getRootNode()->translate(dirVec, Ogre::Node::TS_LOCAL);
    }
    return true;
}

//---------------------------------------------------------------------------
//Mouse movement listener (implement later)
bool TutorialApplication::mouseMoved(const OIS::MouseEvent& me) {
    //How do we translate mouse coordinates to object/world coordinates?
    // if(me.state.X.rel > 0)
    //     std::cout << "xrel positive\n";
    return true;
}

//---------------------------------------------------------------------------
//Button call to determine which kind of netmanager to start
void TutorialApplication::startNetworking(bool isClient) {
    mNetworkingStarted = true;
    mIsClient = isClient;
    mMultiplayer = true;
    netManager.initNetManager();
    if(!isClient)   //is Server
    {
        netManager.addNetworkInfo(PROTOCOL_UDP, NULL, mPortNumber);
        netManager.startServer();
        netManager.acceptConnections();
        std::cout << "Started Server\n";
    }
    else {
        netManager.addNetworkInfo(PROTOCOL_UDP, mIPAddress, mPortNumber);
        netManager.startClient();
        std::cout << "Started Client\n";
    }
}

//---------------------------------------------------------------------------
void TutorialApplication::stopNetworking() {
    netManager.close();
    mNetworkingStarted = false;
    std::cout << "Networking stopped\n";
}

//---------------------------------------------------------------------------
bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
	   return false;
    if(mShutDown)
	   return false;

    mKeyboard->capture();
    mMouse->capture();

    if(sim)
    {
        sim->stepSimulation(evt.timeSinceLastFrame);
    }

    //Send message
    if(mNetworkingStarted)
    {
        Ogre::String message = "";
        message += Ogre::StringConverter::toString(paddle->getRootNode()->getPosition().x) + " ";
        message += Ogre::StringConverter::toString(paddle->getRootNode()->getPosition().y) + " ";

        message += Ogre::StringConverter::toString(paddle2->getRootNode()->getPosition().x) + " ";
        message += Ogre::StringConverter::toString(paddle2->getRootNode()->getPosition().y) + " ";


        if(mIsClient)
        {
            netManager.messageServer(PROTOCOL_UDP, message.c_str(), message.length());
        }
        else if(netManager.getClients() > 0)
        {
            netManager.messageClients(PROTOCOL_UDP, message.c_str(), message.length());
        }
    }
    //Receive message
    if(mNetworkingStarted)
    {
        if(mIsClient)
        {
            if(netManager.pollForActivity(1))
            {
                std::cout << "received message from server" << std::endl;
               
                std::istringstream stream(netManager.udpServerData[0].output);
                std::string s;
                stream >> s;
                std::cout << "message from server: " << s << std::endl;
                float x = atof(s.c_str());
                stream >> s;
                float y = atof(s.c_str());
                float z = paddle->getRootNode()->getPosition().z;
                
                stream >> s;
                float x2 = atof(s.c_str());
                stream >> s;
                float y2 = atof(s.c_str());
                float z2 = paddle2->getRootNode()->getPosition().z;

                paddle->getRootNode()->setPosition(x, y, z);
                paddle2->getRootNode()->setPosition(x2, y2, z2);
            }
        }
        else /*if(netManager.getClients() > 0)*/
        {
            if(netManager.pollForActivity(1))
            {
                std::cout << "received message from client" << std::endl;

                std::istringstream stream(netManager.udpClientData[0]->output);
                std::string s;
                stream >> s;
                std::cout << "message from client: " << s << std::endl;
                float x = atof(s.c_str());
                stream >> s;
                float y = atof(s.c_str());
                float z = paddle->getRootNode()->getPosition().z;
                
                stream >> s;
                float x2 = atof(s.c_str());
                stream >> s;
                float y2 = atof(s.c_str());
                float z2 = paddle2->getRootNode()->getPosition().z;

                paddle->getRootNode()->setPosition(x, y, z);
                paddle2->getRootNode()->setPosition(x2, y2, z2);
            }
        }
    }

    if(!processUnbufferedInput(evt))
        return false;

    return true;
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        TutorialApplication app;

        try {
            app.go();
        } catch(Ogre::Exception& e)  {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occurred: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------

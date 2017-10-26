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
    mSceneMgr->setSkyBox(true, "Examples/MorningSkyBox", 5000, false);

    // Lights setup
    Ogre::Light* pointLight = mSceneMgr->createLight("PointLight");
    pointLight->setType(Ogre::Light::LT_POINT);
    pointLight->setDiffuseColour(1.0, 1.0, 1.0);
    pointLight->setSpecularColour(1.0, 1.0, 1.0);
    pointLight->setPosition(0, 200, 0);

    Ogre::Light* spotLight = mSceneMgr->createLight("SpotLight");
    spotLight->setDiffuseColour(0, 0, 1.0);
    spotLight->setSpecularColour(0, 0, 1.0);
    spotLight->setType(Ogre::Light::LT_SPOTLIGHT);
    spotLight->setDirection(-1, -1, -1);
    spotLight->setPosition(50, 50, 50);
    spotLight->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(50));
  
    //Physics setup
    sim = new Simulator();
    
    field = new PlayingField(mSceneMgr, sim, 100, 100, "field");
    field->addToSimulator();

    ball = new Ball(mSceneMgr, sim, "ball");
    ball->getRootNode()->setPosition(0, 50, 0);
    ball->addToSimulator();
    ball->setVelocity(0, 0, 50);

    //For now, it's positioning will match that of the camera
    paddle = new Paddle(mSceneMgr, sim, 25, 25, "paddle");
    paddle->getRootNode()->setPosition(20, 25, 50);
    paddle->addToSimulator();
}
//---------------------------------------------------------------------------
void TutorialApplication::createCamera(void)
{
    mCamera = mSceneMgr->createCamera("PlayerCam");
    // mCamera->lookAt(Ogre::Vector3(0, 25, 0));
    mCamera->setNearClipDistance(5);
    mCamNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode");
    mCamNode->setPosition(Ogre::Vector3(0, 25, 150));
    mCamNode->attachObject(mCamera);
    mMove = 0.1;
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
    if(mKeyboard->isKeyDown(OIS::KC_SPACE) && mNetworkingStarted)   //get networking info
    {
        std::cout << "number of clients: " << netManager.getClients() << "\n";
    }

    mCamNode->translate(dirVec, Ogre::Node::TS_LOCAL);
    paddle->getRootNode()->translate(dirVec, Ogre::Node::TS_LOCAL);
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
    //Ogre::Vector3 dirVec = mCamera->getPosition();

    //Send message
    if(mNetworkingStarted)
    {
        Ogre::String message = "";
        message += Ogre::StringConverter::toString(paddle->getRootNode()->getPosition().x) + " ";

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
                float y = paddle->getRootNode()->getPosition().y;
                float z = paddle->getRootNode()->getPosition().z;
                paddle->getRootNode()->setPosition(x, y, z);
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
                float y = paddle->getRootNode()->getPosition().y;
                float z = paddle->getRootNode()->getPosition().z;
                paddle->getRootNode()->setPosition(x, y, z);
            }
        }
    }

    if(!processUnbufferedInput(evt))
        return false;
    // mCamNode->translate(dirVec, Ogre::Node::TS_LOCAL);
    // paddle->getRootNode()->translate(dirVec, Ogre::Node::TS_LOCAL);

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

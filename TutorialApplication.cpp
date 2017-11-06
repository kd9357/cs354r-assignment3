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
    mGameOver = false;
    mMultiplayer = false;
    mPortNumber = 51215;
    mNetworkingStarted = false;
    mIsClient = false;
    maxProjectiles = 3;
    maxEnemies = 8;
    timer = 0;
    maxTime = 1;

    gameStarted = false;
    spacePressed = false;
    SoundManager::initSoundManager();
}
//---------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
    for(int i = 0; i < maxProjectiles; ++i)
    {
        if(serverProjectiles[i] != NULL)
            delete serverProjectiles[i];
        if(clientProjectiles[i] != NULL)
            delete clientProjectiles[i];
    }

    // delete sim;
    // delete paddle;
    // delete paddle2;
    //Delete all enemies list eventually
    //delete enemy;
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
    gui = new GUIManager();

    gui->createMenus();
    //register event listeners
    registerEvents();

    //For now, it's positioning will match that of the camera
    paddle = new Paddle(mSceneMgr, sim, 25, 25, "paddle", false);   //client
    paddle->getRootNode()->setPosition(20, 25, 0);

    paddle2 = new Paddle(mSceneMgr, sim, 25, 25, "paddle2", true);  //server
    paddle2->getRootNode()->setPosition(-20, 25, 0);

    paddle->addToSimulator();
    paddle2->addToSimulator();

    paddle->updateTransform();
    paddle2->updateTransform();

    for(int i = 0; i < maxEnemies; ++i)
    {
        Ogre::String name = "ogre" + Ogre::StringConverter::toString(i);
        Enemy* enemy = new Enemy(mSceneMgr, sim, name);
        enemy->setActive(false);
        enemy->getRootNode()->setPosition(0, -1000, 0);
        enemy->addToSimulator();
        enemies.push_back(enemy);
    }    

    //Create projectiles
    int temp;
    proj = new Projectile(mSceneMgr, sim, &temp, "projectile", false);
    proj->getRootNode()->setPosition(0, -100, 0);
    proj->addToSimulator();
    proj->updateTransform();
    for(int i = 0; i < maxProjectiles; ++i)
    {
        Ogre::String sName = "serverProjectile" + Ogre::StringConverter::toString(i);
        Ogre::String cName = "clientProjectile" + Ogre::StringConverter::toString(i);
        Projectile * sProjectile = new Projectile(mSceneMgr, sim, &temp, sName, false);
        Projectile * cProjectile = new Projectile(mSceneMgr, sim, &temp, cName, true);
        serverProjectiles.push_back(sProjectile);
        clientProjectiles.push_back(cProjectile);
        sProjectile->getRootNode()->setPosition(25, -100 * (i + 1), 0);
        cProjectile->getRootNode()->setPosition(-25, -100 * (i + 1), 0);
        sProjectile->addToSimulator();
        cProjectile->addToSimulator();
    }
    SoundManager::playMusic();
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
    mMove = 0.05;
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
    if(mKeyboard->isKeyDown(OIS::KC_W) && gameStarted)
        dirVec.y += mMove;
    if(mKeyboard->isKeyDown(OIS::KC_S) && gameStarted)
        dirVec.y -= mMove;
    if(mKeyboard->isKeyDown(OIS::KC_A) && gameStarted)
        dirVec.x -= mMove;
    if(mKeyboard->isKeyDown(OIS::KC_D) && gameStarted)
        dirVec.x += mMove;

    if(gameStarted)
    {
        if(!mIsClient)
        {
            paddle->getRootNode()->translate(dirVec, Ogre::Node::TS_LOCAL);
            paddle->updateTransform();
        }
        else
        {
            paddle2->getRootNode()->translate(dirVec, Ogre::Node::TS_LOCAL);
            paddle2->updateTransform();
        }
    }
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
    }
    else {
        netManager.addNetworkInfo(PROTOCOL_UDP, mIPAddress, mPortNumber);
        netManager.startClient();
    }
}

//---------------------------------------------------------------------------
void TutorialApplication::stopNetworking() {
    netManager.close();
    mNetworkingStarted = false;
}

CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;
 
    case OIS::MB_Right:
        return CEGUI::RightButton;
 
    case OIS::MB_Middle:
        return CEGUI::MiddleButton;
 
    default:
        return CEGUI::LeftButton;
    }
}

bool TutorialApplication::mouseMoved(const OIS::MouseEvent& arg) {
    CEGUI::System &sys = CEGUI::System::getSingleton();
    sys.getDefaultGUIContext().injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
    // Scroll wheel.
    if (arg.state.Z.rel)
        sys.getDefaultGUIContext().injectMouseWheelChange(arg.state.Z.rel / 120.0f);
    return true;
}

bool TutorialApplication::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id));
    return true;
}

bool TutorialApplication::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id));
    return true;
}

bool TutorialApplication::keyPressed( const OIS::KeyEvent &arg)
{
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.injectKeyDown((CEGUI::Key::Scan)arg.key);
    context.injectChar((CEGUI::Key::Scan)arg.text);

    if (arg.key == OIS::KC_ESCAPE) {
        if(!gui->mainmenu->isVisible()){
            gui->pauseMenu();
        } else {
            gui->showScore();
        }
    }

    if(arg.key == OIS::KC_SPACE && !spacePressed)
    {
        spacePressed = true;
        fireProjectile();
    }
    return BaseApplication::keyPressed(arg);
}

bool TutorialApplication::keyReleased( const OIS::KeyEvent &arg )
{
    CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)arg.key);
    if(arg.key == OIS::KC_SPACE) {
        spacePressed = false;
    }
    return BaseApplication::keyReleased(arg);
}

//Button handlers
void TutorialApplication::registerEvents(){
    CEGUI::Window *quit = gui->mainmenu->getChildRecursive("quitBtn");
    quit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::quit, this));

    CEGUI::Window *host = gui->lobby->getChildRecursive("host");
    host->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::host, this));

    CEGUI::Window *join = gui->lobby->getChildRecursive("join");
    join->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::join, this));

    CEGUI::Window *start = gui->lobby->getChildRecursive("start");
    start->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::start, this));

    CEGUI::Window *p2ready = gui->lobby->getChildRecursive("p2ready");
    p2ready->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&TutorialApplication::clientReady, this));

    CEGUI::Window *SEToggle = gui->settings->getChildRecursive("SEToggle");
    SEToggle->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&TutorialApplication::pauseSoundEffects, this));

    CEGUI::Window *MToggle = gui->settings->getChildRecursive("MToggle");
    MToggle->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&TutorialApplication::pauseMusic, this));
    
    CEGUI::Window *singlePlayer = gui->guiRoot->getChildRecursive("singleplayerBtn");
    singlePlayer->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::startSinglePlayer, this));

}

void TutorialApplication::clientReady(){
    if(mIsClient){
        Ogre::String message = "client_ready ";
        netManager.messageServer(PROTOCOL_UDP, message.c_str(), message.length());
    }
}

void TutorialApplication::start(){
    CEGUI::ToggleButton *p2ready = static_cast<CEGUI::ToggleButton*>(gui->lobby->getChildRecursive("p2ready"));
    if(!mIsClient && p2ready->isSelected()){
        Ogre::String message = "start_game ";
        netManager.messageClients(PROTOCOL_UDP, message.c_str(), message.length());
        gameStarted = true;
        gui->showScore();
    }
}

void TutorialApplication::startSinglePlayer()
{
    gameStarted = true;
    mMultiplayer = false;
    mNetworkingStarted = false;
    gui->showScore();
    paddle2->getRootNode()->setPosition(100, -500, 0);
}

void TutorialApplication::host(){
    startNetworking(false);
    CEGUI::Window *serverhostname = gui->lobby->getChildRecursive("serverhostname");
    serverhostname -> setText("You're the new host!");
    CEGUI::Window *player1 = gui->lobby->getChildRecursive("player1");
    player1->show();
    CEGUI::Window *p2ready = gui->lobby->getChildRecursive("p2ready");
    p2ready->setDisabled(true);

}

void TutorialApplication::join(){
    CEGUI::Window *hostname = gui->lobby->getChildRecursive("hostname");
    CEGUI::Window *start = gui->lobby->getChildRecursive("start");
    CEGUI::Window *serverhostname = gui->lobby->getChildRecursive("serverhostname");
    CEGUI::Window *host = gui->lobby->getChildRecursive("host");
    CEGUI::Window *player1 = gui->lobby->getChildRecursive("player1");
    CEGUI::Window *player2 = gui->lobby->getChildRecursive("player2");
    CEGUI::Window *p2ready = gui->lobby->getChildRecursive("p2ready");
    player1->show();
    player2->show();
    p2ready->show();

    mIPAddress = hostname->getText().c_str();
    host->setDisabled(true);
    start->setDisabled(true);

    startNetworking(true);

    serverhostname -> setText("You've joined " + netManager.getHostname());
    Ogre::String message = "client_joined ";
    netManager.messageServer(PROTOCOL_UDP, message.c_str(), message.length());
}

void TutorialApplication::fireProjectile()
{
    int temp;
    if(!mIsClient)
    {
        for(int i = 0; i < maxProjectiles; ++i)
        {
            if(!serverProjectiles[i]->isActive())
            {
                serverProjectiles[i]->setActive(true);
                serverProjectiles[i]->getRootNode()->setPosition(paddle->getRootNode()->getPosition().x,
                                                                 paddle->getRootNode()->getPosition().y + 20,
                                                                 paddle->getRootNode()->getPosition().z);

                // serverProjectiles[i]->addToSimulator();
                serverProjectiles[i]->setVelocity(0, 1000, 0);
                serverProjectiles[i]->updateTransform();
                serverProjectiles[i]->updateWorldTransform();
                SoundManager::playSoundEffect("fire");
                break;
            }
        }
    }
    else 
    {
        for(int i = 0; i < maxProjectiles; ++i)
        {
            if(!clientProjectiles[i]->isActive())
            {
                clientProjectiles[i]->setActive(true);
                clientProjectiles[i]->getRootNode()->setPosition(paddle2->getRootNode()->getPosition().x,
                                                                 paddle2->getRootNode()->getPosition().y + 20,
                                                                 paddle2->getRootNode()->getPosition().z);
                // clientProjectiles[i]->addToSimulator();
                clientProjectiles[i]->setVelocity(0, 1000, 0);
                clientProjectiles[i]->updateTransform();
                clientProjectiles[i]->updateWorldTransform();
                SoundManager::playSoundEffect("fire");
                break;
            }
        }
    } 
}


void TutorialApplication::pauseMusic(){
    if(Mix_VolumeMusic(-1) == 0){
        Mix_VolumeMusic(40);
    } else {
        Mix_VolumeMusic(0);
    }
}

void TutorialApplication::pauseSoundEffects(){
    if(Mix_Volume(-1,-1) == 0){
        Mix_Volume(-1, 128);
    } else {
        Mix_Volume(-1, 0);
    }
}

void TutorialApplication::recycleProjectiles()
{
    for(int i = 0; i < maxProjectiles; ++i)
    {
        //Check if projectile out of bounds, if yes then deactivate
        if(serverProjectiles[i]->getRootNode()->getPosition().y > 1500 && serverProjectiles[i]->isActive())
        {
             serverProjectiles[i]->reset();
        }

        if(clientProjectiles[i]->getRootNode()->getPosition().y > 1500 && clientProjectiles[i]->isActive())
        {
             clientProjectiles[i]->reset();
        }
    }
}

void TutorialApplication::recycleEnemies(float time)
{
    int size = enemies.size();
    timer += time;
    bool startSpawn = false;
    if(timer >= maxTime)
    {
        timer = 0.0f;
        startSpawn = true;
    }
    for(int i = 0; i < size; ++i)
    {
        if(enemies[i]->getRootNode()->getPosition().y <= 0 && enemies[i]->getRootNode()->getPosition().y >= -100)
        {
            mGameOver = true;
            break;
        }
        if(startSpawn && !enemies[i]->isActive())
        {
            enemies[i]->spawn();
            startSpawn = false;
        }
    }
}

void TutorialApplication::gameReset()
{
    paddle->getRootNode()->setPosition(20, 25, 0);
    if(mMultiplayer)
        paddle2->getRootNode()->setPosition(-20, 25, 0);

    int size = serverProjectiles.size();
    for(int i = 0; i < size; ++i)
    {
        serverProjectiles[i]->reset();
        clientProjectiles[i]->reset();
    }

    for(int i = 0; i < maxEnemies; ++i)
    {
        enemies[i]->reset();
    }
    mGameOver = false;
    gameStarted = true;
    GameObject::score = 0;
}

void TutorialApplication::quit(){
    mShutDown = true;
}

Ogre::String TutorialApplication::createMessage()
{
    Ogre::String message = "";

    if(!mIsClient) 
    {
        message += Ogre::StringConverter::toString(paddle->getRootNode()->getPosition().x) + " ";
        message += Ogre::StringConverter::toString(paddle->getRootNode()->getPosition().y) + " ";
        message += Ogre::StringConverter::toString(paddle->getRootNode()->getPosition().z) + " ";
        //Check projectile status
        for(int i = 0; i < maxProjectiles; ++i)
        {
            message += Ogre::StringConverter::toString(serverProjectiles[i]->getRootNode()->getPosition().x) + " ";
            message += Ogre::StringConverter::toString(serverProjectiles[i]->getRootNode()->getPosition().y) + " ";
            message += Ogre::StringConverter::toString(serverProjectiles[i]->getRootNode()->getPosition().z) + " ";
        }

        int size = enemies.size();
        for(int i = 0; i < size; ++i)
        {
            if(mGameOver)
            {
                message += "GAMEOVER ";
                break;
            }
            message += Ogre::StringConverter::toString(enemies[i]->getRootNode()->getPosition().x) + " ";
            message += Ogre::StringConverter::toString(enemies[i]->getRootNode()->getPosition().y) + " ";
            message += Ogre::StringConverter::toString(enemies[i]->getRootNode()->getPosition().z) + " ";
        }
        message += Ogre::StringConverter::toString(GameObject::score) + " ";
    }
    else //otherwise only send client projectile data
    {
        message += Ogre::StringConverter::toString(paddle2->getRootNode()->getPosition().x) + " ";
        message += Ogre::StringConverter::toString(paddle2->getRootNode()->getPosition().y) + " ";
        message += Ogre::StringConverter::toString(paddle2->getRootNode()->getPosition().z) + " ";
        for(int i = 0; i < maxProjectiles; ++i)
        {
            //Check if projectile out of bounds, if yes then deactivate
            if(clientProjectiles[i]->getRootNode()->getPosition().y > 1500 && clientProjectiles[i]->isActive())
            {
                 clientProjectiles[i]->getRootNode()->setPosition(0, -100, 0);
                 clientProjectiles[i]->setVelocity(0, 0, 0);
                 clientProjectiles[i]->setActive(false);
                 clientProjectiles[i]->updateTransform();
                 clientProjectiles[i]->updateWorldTransform();
            }
            message += Ogre::StringConverter::toString(clientProjectiles[i]->getRootNode()->getPosition().x) + " ";
            message += Ogre::StringConverter::toString(clientProjectiles[i]->getRootNode()->getPosition().y) + " ";
            message += Ogre::StringConverter::toString(clientProjectiles[i]->getRootNode()->getPosition().z) + " ";
        }
    }
    return message;
}

//---------------------------------------------------------------------------
bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    float x, y, z;
    if(mWindow->isClosed())
	   return false;
    if(mShutDown)
	   return false;

    mKeyboard->capture();
    mMouse->capture();

    if(mGameOver)
    {
        gameReset();
        return true;
    }

    if(sim && gameStarted)
        sim->stepSimulation(evt.timeSinceLastFrame);

    if(gameStarted)
    {
        recycleProjectiles();
        recycleEnemies(evt.timeSinceLastFrame);
    }

    //pregame startup
    if(mNetworkingStarted && !gameStarted)
    {
        if(netManager.pollForActivity(1))
        {
            if(mIsClient){
                std::istringstream stream(netManager.udpServerData[0].output);
                std::string s;
                //Update server ship
                stream >> s;
                if(s.compare("server_ready") == 0){
                    CEGUI::ToggleButton *p1ready = static_cast<CEGUI::ToggleButton*>(gui->guiRoot->getChildRecursive("p1ready"));
                    p1ready->setSelected(true);
                }
                
                if(s.compare("start_game") == 0){
                    gameStarted = true;
                    gui->showScore();
                }
            } else {
                std::istringstream stream(netManager.udpClientData[0]->output);
                std::string s;
                //Update server ship
                stream >> s;
                if(s.compare("client_ready") == 0){
                    CEGUI::ToggleButton *p2ready = static_cast<CEGUI::ToggleButton*>(gui->guiRoot->getChildRecursive("p2ready"));
                    p2ready->setSelected(true);
                    CEGUI::Window *start = gui->guiRoot->getChildRecursive("start");
                    start->setDisabled(false);

                }
                if(s.compare("client_joined") == 0){
                    CEGUI::Window *player2 = gui->guiRoot->getChildRecursive("player2");
                    CEGUI::Window *p2ready = gui->guiRoot->getChildRecursive("p2ready");
                    player2->show();
                    p2ready->show();
                }
            }
            
        }
    }

    //Send message
    if(mNetworkingStarted && gameStarted)
    {
        Ogre::String message = createMessage();
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
    if(mNetworkingStarted && gameStarted)
    {
            if(mIsClient)
            {
                if(netManager.pollForActivity(1))
                {
                    std::istringstream stream(netManager.udpServerData[0].output);
                    std::string s;

                    //Update server ship
                    stream >> s;
                    x = atof(s.c_str());
                    stream >> s;
                    y = atof(s.c_str());
                    stream >> s;
                    z = atof(s.c_str());
                    paddle->getRootNode()->setPosition(x, y, z);

                    //Update server projectiles
                    for(int i = 0; i < maxProjectiles; ++i)
                    {
                        stream >> s;
                        x = atof(s.c_str());
                        stream >> s;
                        y = atof(s.c_str());
                        stream >> s;
                        z = atof(s.c_str());
                        serverProjectiles[i]->getRootNode()->setPosition(x, y, z);
                        serverProjectiles[i]->updateTransform();
                        serverProjectiles[i]->updateWorldTransform();
                    }

                    for(int i = 0; i < maxEnemies; ++i)
                    {
                        stream >> s;
                        if(s.compare("GAMEOVER") == 0)
                        {
                            mGameOver = true;
                            break;
                        }
                        x = atof(s.c_str());
                        stream >> s;
                        y = atof(s.c_str());
                        stream >> s;
                        z = atof(s.c_str());
                        enemies[i]->getRootNode()->setPosition(x, y, z);
                        enemies[i]->updateTransform();
                        enemies[i]->updateWorldTransform();
                    }
                    stream >> s;
                    int score = atoi(s.c_str());
                    GameObject::score = score;
                }
            }
            else
            {
                if(netManager.pollForActivity(1))
                {
                    std::istringstream stream(netManager.udpClientData[0]->output);
                    std::string s;

                    //Update client ship
                    stream >> s;
                    x = atof(s.c_str());
                    stream >> s;
                    y = atof(s.c_str());
                    stream >> s;
                    z = atof(s.c_str());
                    paddle2->getRootNode()->setPosition(x, y, z);

                    //Update server projectiles
                    for(int i = 0; i < maxProjectiles; ++i)
                    {
                        stream >> s;
                        x = atof(s.c_str());
                        stream >> s;
                        y = atof(s.c_str());
                        stream >> s;
                        z = atof(s.c_str());
                        clientProjectiles[i]->getRootNode()->setPosition(x, y, z);
                        clientProjectiles[i]->updateTransform();
                        clientProjectiles[i]->updateWorldTransform();
                    }
                }
            }
    
    }

    if(!processUnbufferedInput(evt))
        return false;



    CEGUI::Window *scorelabel = gui->score->getChildRecursive("scorelabel");
    scorelabel->setText(Ogre::StringConverter::toString(GameObject::score).c_str());

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

#include "GUIManager.h"


GUIManager::GUIManager(){
    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
    CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
    
    CEGUI::SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("Vanilla-Images/MouseArrow");
}

void GUIManager::createMenus(){
    guiRoot = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("mainmenu.layout"); 
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(guiRoot);

    lobby = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("lobby.layout"); 
    lobby->hide();
    guiRoot->addChild(lobby);

    settings = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("settings.layout");
    settings->hide();
    guiRoot->addChild(settings);

    //Main Menu buttons
    CEGUI::Window *singleplayerBtn = guiRoot->getChildRecursive("singleplayerBtn");
    singleplayerBtn->setDisabled(true);

    CEGUI::Window *player1 = guiRoot->getChildRecursive("player1");
    player1->hide();
    CEGUI::Window *player2 = guiRoot->getChildRecursive("player2");
    player2->hide();
    CEGUI::Window *p2ready = guiRoot->getChildRecursive("p2ready");
    p2ready->hide();

    CEGUI::Window *multiplayerBtn = guiRoot->getChildRecursive("multiplayerBtn");
    multiplayerBtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::showLobby, this));

    CEGUI::Window *settingsBtn = guiRoot->getChildRecursive("settingsBtn");
    settingsBtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::showSettings, this));

    //Lobby buttons -> quit is in TutorialApplication.cpp
    CEGUI::Window *serverhostname = guiRoot->getChildRecursive("serverhostname");
    serverhostname->setDisabled(true);
    serverhostname->setTooltipText("If you wish to host a game, press Host");

    CEGUI::Window *lobbyBackBtn = guiRoot->getChildRecursive("lobbyBackBtn");
    lobbyBackBtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::showMainMenu, this));

    //Settings buttons
    CEGUI::Window *settingsBackBtn = guiRoot->getChildRecursive("settingsBack");
    settingsBackBtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::showMainMenu, this));
}

void GUIManager::showMainMenu(){
    lobby->hide();
    settings->hide();
    guiRoot->show();
    guiRoot->activate();
}

void GUIManager::showSettings(){
    lobby->hide();
    settings->show();
    settings->activate();
}

void GUIManager::showLobby(){
    settings->hide();
    lobby->show();
    lobby->activate();
}

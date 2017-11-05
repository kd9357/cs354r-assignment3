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


    guiRoot = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow","root" );
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(guiRoot);

    mainmenu = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("mainmenu.layout"); 
    guiRoot->addChild(mainmenu);

    lobby = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("lobby.layout"); 
    lobby->hide();
    guiRoot->addChild(lobby);

    settings = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("settings.layout");
    settings->hide();
    guiRoot->addChild(settings);

    score = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("score.layout");
    score->hide();
    guiRoot->addChild(score);

    //Main Menu buttons
    
    CEGUI::Window *multiplayerBtn = mainmenu->getChildRecursive("multiplayerBtn");
    multiplayerBtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::showLobby, this));

    CEGUI::Window *settingsBtn = mainmenu->getChildRecursive("settingsBtn");
    settingsBtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::showSettings, this));

    //Lobby buttons -> quit is in TutorialApplication.cpp
    CEGUI::Window *serverhostname = lobby->getChildRecursive("serverhostname");
    serverhostname->setDisabled(true);

    CEGUI::Window *player1 = lobby->getChildRecursive("player1");
    player1->hide();
    
    CEGUI::Window *player2 = lobby->getChildRecursive("player2");
    player2->hide();

    CEGUI::Window *p2ready = lobby->getChildRecursive("p2ready");
    p2ready->hide();

    CEGUI::Window *start = lobby->getChildRecursive("start");
    start->setDisabled(true);

    CEGUI::Window *lobbyBackBtn = lobby->getChildRecursive("lobbyBackBtn");
    lobbyBackBtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::showMainMenu, this));

    //Settings buttons
    CEGUI::Window *settingsBackBtn = settings->getChildRecursive("settingsBack");
    settingsBackBtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::showMainMenu, this));

    CEGUI::ToggleButton *SEToggle = static_cast<CEGUI::ToggleButton*>(settings->getChildRecursive("SEToggle"));
    CEGUI::ToggleButton *MToggle = static_cast<CEGUI::ToggleButton*>(settings->getChildRecursive("MToggle"));

    SEToggle->setSelected(true);
    MToggle->setSelected(true);
}

void GUIManager::showMainMenu(){
    lobby->hide();
    settings->hide();
    score->hide();
    mainmenu->show();
    mainmenu->activate();
}

void GUIManager::showSettings(){
    lobby->hide();
    mainmenu->hide();
    score->hide();
    settings->show();
    settings->activate();
}

void GUIManager::showLobby(){
    settings->hide();
    mainmenu->hide();
    score->hide();
    lobby->show();
    lobby->activate();
}

void GUIManager::showScore(){
    settings->hide();
    mainmenu->hide();
    lobby->hide();
    score->show();
}

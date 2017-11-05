#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <iostream>

class TutorialApplication;

class GUIManager{
public:
    CEGUI::Window *guiRoot;
    CEGUI::Window *lobby;
    CEGUI::Window *settings;
    CEGUI::Window *mainmenu;
    CEGUI::Window *score;

    GUIManager(void);
    void createMenus(void);
    void showMainMenu(void);
    void showSettings(void);
    void showLobby(void);
    void showScore(void);

protected:
    CEGUI::OgreRenderer* mRenderer;
};

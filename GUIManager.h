#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <iostream>

class TutorialApplication;

class GUIManager{
public:
    GUIManager(void);
    void createMenus(void);
	CEGUI::Window *guiRoot;
	CEGUI::Window *lobby;
	CEGUI::Window *settings;
    void showMainMenu(void);
    void showSettings(void);
    void showLobby(void);

protected:
    CEGUI::OgreRenderer* mRenderer;
    TutorialApplication* app;
};

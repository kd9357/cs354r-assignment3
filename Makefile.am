noinst_HEADERS = BaseApplication.h TutorialApplication.h Simulator.h BulletContactCallback.h GameObject.h Ball.h PlayingField.h Paddle.h

CEGUI_CFLAGS = -pthread -I/lusr/opt/cegui-0.8.7/include/cegui-0 -I/usr/include/OGRE
CEGUI_LIBS = -L/lusr/opt/cegui-0.8.7/lib -lCEGUIOgreRenderer-0 -lOgreMain -lpthread -lCEGUIBase-0

bin_PROGRAMS = assignment2
assignment2_CPPFLAGS = -I$(top_srcdir)
assignment2_SOURCES = BaseApplication.cpp TutorialApplication.cpp Simulator.cpp GameObject.cpp Ball.cpp PlayingField.cpp Paddle.cpp
assignment2_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) $(sdl_CFLAGS) $(SDLMIXER_CFLAGS) $(CEGUI_CFLAGS)
assignment2_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(sdl_LIBS) $(SDLMIXER_LIBS) $(CEGUI_LIBS)
assignment2_LDFLAGS = -lOgreOverlay -lboost_system -R/lusr/opt/cegui-0.8.7

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign

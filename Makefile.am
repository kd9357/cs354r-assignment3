noinst_HEADERS = BaseApplication.h TutorialApplication.h Simulator.h BulletContactCallback.h GameObject.h Ball.h Paddle.h NetManager.h Enemy.h Projectile.h

CEGUI_CFLAGS = -pthread -I/lusr/opt/cegui-0.8.7/include/cegui-0 -I/usr/include/OGRE
CEGUI_LIBS = -L/lusr/opt/cegui-0.8.7/lib -lCEGUIOgreRenderer-0 -lOgreMain -lpthread -lCEGUIBase-0

bin_PROGRAMS = assignment3
assignment3_CPPFLAGS = -I$(top_srcdir)
assignment3_SOURCES = BaseApplication.cpp TutorialApplication.cpp Simulator.cpp GameObject.cpp Ball.cpp Paddle.cpp NetManager.cpp Enemy.cpp Projectile.cpp
assignment3_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) $(sdl_CFLAGS) $(SDLMIXER_CFLAGS) $(CEGUI_CFLAGS) $(SDLNET_CFLAGS)
assignment3_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(sdl_LIBS) $(SDLMIXER_LIBS) $(CEGUI_LIBS) $(SDLNET_LIBS)
assignment3_LDFLAGS = -lOgreOverlay -lboost_system -R/lusr/opt/cegui-0.8.7

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign

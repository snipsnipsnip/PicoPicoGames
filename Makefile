OUT = picopico.exe
OBJDIR = obj~
DISTDIR = dist~
 
LIBS = *.lib advapi32.lib kernel32.lib gdi32.lib user32.lib opengl32.lib glu32.lib winmm.lib shlwapi.lib
INCLUDE = $(INCLUDE)
CFLAGS_COMMON = /nologo /W4 /WX /MD /wd4100 /wd4127 /wd4505 /wd4210 /wd4142 /wd4530 /wd4204 /wd4819 /D_CRT_SECURE_NO_DEPRECATE /wd4244 /wd4018 /wd4701 /wd4068 /wd4800 /wd4805 /wd4700 /wd4702 /wd4611 /wd4389 /wd4102 /wd4706
LINKFLAGS = /nologo /manifest /nodefaultlib:libcmt.lib
O = obj
CC_OBJ_OUT_FLAG = -Fo
CC = cl
 
CFLAGS_COMMON = $(CFLAGS_COMMON) /Ot /Ox /DNDEBUG /DRELEASE
LINKFLAGS = $(LINKFLAGS) /subsystem:windows
 
CXXFLAGS = $(CFLAGS_COMMON) /Fp$(PCH_CXX) $(CXXFLAGS)
CFLAGS = $(CFLAGS_COMMON) /Fp$(PCH_C) $(CFLAGS)

OBJS = \
  $(OBJDIR)/PPGameButton.$(O) \
  $(OBJDIR)/PPGameMapData.$(O) \
  $(OBJDIR)/PPGamePoly.$(O) \
  $(OBJDIR)/PPGameRunlength.$(O) \
  $(OBJDIR)/PPGameSound.$(O) \
  $(OBJDIR)/PPGameSplite.$(O) \
  $(OBJDIR)/QBGame.$(O) \
  $(OBJDIR)/QBMenu.$(O) \
  $(OBJDIR)/QBSound.$(O) \
  $(OBJDIR)/QBSoundWin.$(O) \
  $(OBJDIR)/QBUtil.$(O) \
  $(OBJDIR)/main.$(O) \
  $(OBJDIR)/PPGameUtilWin.$(O) \
  $(OBJDIR)/PPGameTextWin.$(O) \
  $(OBJDIR)/PPGameMapWin.$(O) \
  $(OBJDIR)/QBCommonGrobal.$(O) \
  $(OBJDIR)/QBNodeSystemCore.$(O) \
  $(OBJDIR)/QBNodeSystemValue.$(O) \
  $(OBJDIR)/QBNodeSystemUtil.$(O) \
  $(OBJDIR)/QBNodeSystemPlist.$(O) \
  $(OBJDIR)/QBNodeBase64.$(O) \
  $(OBJDIR)/QBTankGame.$(O) \
  $(OBJDIR)/QBGameInstant.$(O)
 
run: all
	$(OUT) $(ARGS)
 
mute: all
	$(OUT) -m $(ARGS)
 
all: $(OUT)
 
$(OUT): $(OBJDIR) $(OBJS)
	@link /out:$(OUT) $(LINKFLAGS) $(OBJS) $(LIBS)
 
$(OBJDIR):
	mkdir $(OBJDIR)
 
$(DISTDIR):
	mkdir $(DISTDIR)
 
clean:
	for %i in ($(OUT)) do if exist %i del %i
	if exist $(OBJDIR) del /q $(OBJDIR) && rmdir $(OBJDIR)

CFLAGS_INCLUDE_DIRS = \
  -I./MacOSX/PPAppSystem \
  -I./Windows/QBNode \
  -I./Windows/png \
  -I./Windows/png/contrib/gregbook \
  -I./Windows/png/contrib/pngminim/decoder \
  -I./Windows/png/contrib/pngminim/encoder \
  -I./Windows/png/contrib/pngminim/preader \
  -I./Windows/png/contrib/visupng \
  -I./Windows/zlib \
  -I./Windows/zlib/contrib/blast \
  -I./Windows/zlib/contrib/infback9 \
  -I./Windows/zlib/contrib/iostream \
  -I./Windows/zlib/contrib/iostream2 \
  -I./Windows/zlib/contrib/iostream3 \
  -I./Windows/zlib/contrib/minizip \
  -I./Windows/zlib/contrib/puff \
  -I./Windows/zlib/examples \
  -I./iPhone/Classes \
  -I./iPhone/PPGame/PPGameSound \
  -I./iPhone/PPGame/PPGameSystem \
  -I./iPhone/PPGame/PPGameUtil \
  -I./iPhone/PPGame/PPGameView \
  -I./iPhone/QBGameSystem \
  -I./iPhone/QBGames/BlueBall \
  -I./iPhone/QBGames/CarRace \
  -I./iPhone/QBGames/GrafSample \
  -I./iPhone/QBGames/MissilePanic \
  -I./iPhone/QBGames/PicoHero \
  -I./iPhone/QBGames/PicoPicoQuest \
  -I./iPhone/QBGames/TankGame \
  -I./iPhone/QBGames/TowerOfShadow \
  -I./iPhone/QBGames/_Template \
  -I./iPhone/QBImages
 
./iPhone/PPGame/PPGameSound/QBSoundMac.h: ./iPhone/PPGame/PPGameSound/QBSound.h
./iPhone/PPGame/PPGameSound/QBSoundWin.h: ./iPhone/PPGame/PPGameSound/QBSound.h
./iPhone/PPGame/PPGameUtil/PPGameButton.h: ./iPhone/PPGame/PPGameView/PPGameDef.h
./iPhone/PPGame/PPGameUtil/PPGameMap.h: ./iPhone/PPGame/PPGameUtil/PPGameMapData.h
./iPhone/PPGame/PPGameView/PPGameSplite.h: ./iPhone/PPGame/PPGameView/PPGameDef.h ./iPhone/PPGame/PPGameView/PPGamePoly.h
./iPhone/PPGame/PPGameView/PPGameText.h: ./iPhone/PPGame/PPGameView/PPGameSplite.h
./iPhone/QBGameSystem/QBGame.h: ./iPhone/PPGame/PPGameUtil/PPGameMapData.h ./iPhone/PPGame/PPGameUtil/PPGameUtil.h ./iPhone/PPGame/PPGameView/PPGameDef.h ./iPhone/PPGame/PPGameView/PPGamePoly.h ./iPhone/PPGame/PPGameView/PPGameText.h ./iPhone/QBGameSystem/QBMenu.h ./iPhone/QBGameSystem/QBUtil.h
./iPhone/QBGames/CarRace/carrace.h: ./iPhone/PPGame/PPGameView/PPGameDef.h
./iPhone/QBImages/font.h: ./iPhone/PPGame/PPGameView/PPGameDef.h
./iPhone/QBImages/font2.h: ./iPhone/PPGame/PPGameView/PPGameDef.h
./iPhone/QBImages/key.h: ./iPhone/PPGame/PPGameView/PPGameDef.h
 
./Windows/main.cpp: ./iPhone/PPGame/PPGameSound/QBSound.h ./iPhone/PPGame/PPGameSound/QBSoundWin.h ./iPhone/PPGame/PPGameUtil/PPGameButton.h ./iPhone/PPGame/PPGameView/PPGameDef.h ./iPhone/PPGame/PPGameView/PPGameSplite.h ./iPhone/QBGameSystem/QBGame.h ./iPhone/QBGames/CarRace/carrace.h ./iPhone/QBImages/font.h ./iPhone/QBImages/font2.h ./iPhone/QBImages/key.h
./iPhone/PPGame/PPGameSound/PPGameSound.cpp: ./iPhone/PPGame/PPGameSound/PPGameSound.h ./iPhone/PPGame/PPGameSound/QBSound.h
./iPhone/PPGame/PPGameSound/QBSound.cpp: ./iPhone/PPGame/PPGameSound/QBSound.h ./iPhone/PPGame/PPGameSound/QBSoundMac.h ./iPhone/PPGame/PPGameSound/QBSoundWin.h
./iPhone/PPGame/PPGameSound/QBSoundMac.cpp: ./iPhone/PPGame/PPGameSound/QBSoundMac.h
./iPhone/PPGame/PPGameSound/QBSoundWin.cpp: ./iPhone/PPGame/PPGameSound/QBSoundWin.h
./iPhone/PPGame/PPGameUtil/PPGameButton.cpp: ./iPhone/PPGame/PPGameUtil/PPGameButton.h
./iPhone/PPGame/PPGameUtil/PPGameMapData.cpp: ./iPhone/PPGame/PPGameUtil/PPGameMapData.h ./iPhone/PPGame/PPGameUtil/PPGameRunlength.h
./iPhone/PPGame/PPGameUtil/PPGameRunlength.c: ./iPhone/PPGame/PPGameUtil/PPGameRunlength.h
./iPhone/PPGame/PPGameView/PPGamePoly.cpp: ./iPhone/PPGame/PPGameView/PPGameDef.h ./iPhone/PPGame/PPGameView/PPGamePoly.h
./iPhone/PPGame/PPGameView/PPGameSplite.cpp: ./iPhone/PPGame/PPGameUtil/PPGameUtil.h ./iPhone/PPGame/PPGameView/PPGameSplite.h
./iPhone/QBGameSystem/QBGame.cpp: ./iPhone/PPGame/PPGameSound/PPGameSound.h ./iPhone/PPGame/PPGameUtil/PPGameMap.h ./iPhone/PPGame/PPGameUtil/PPGameUtil.h ./iPhone/QBGameSystem/QBGame.h
./iPhone/QBGameSystem/QBMenu.cpp: ./iPhone/PPGame/PPGameSound/PPGameSound.h ./iPhone/QBGameSystem/QBGame.h ./iPhone/QBGameSystem/QBMenu.h
./iPhone/QBGameSystem/QBUtil.cpp: ./iPhone/QBGameSystem/QBUtil.h
 
$(OBJDIR)/PPGameButton.$(O): ./iPhone/PPGame/PPGameUtil/PPGameButton.cpp
	$(CC) $(CXXFLAGS) $(CFLAGS_INCLUDE_DIRS) $(CC_OBJ_OUT_FLAG)$@ -c $?
 
$(OBJDIR)/PPGameMapData.$(O): ./iPhone/PPGame/PPGameUtil/PPGameMapData.cpp
	$(CC) $(CXXFLAGS) $(CFLAGS_INCLUDE_DIRS) $(CC_OBJ_OUT_FLAG)$@ -c $?
 
$(OBJDIR)/PPGamePoly.$(O): ./iPhone/PPGame/PPGameView/PPGamePoly.cpp
	$(CC) $(CXXFLAGS) $(CFLAGS_INCLUDE_DIRS) $(CC_OBJ_OUT_FLAG)$@ -c $?
 
$(OBJDIR)/PPGameRunlength.$(O): ./iPhone/PPGame/PPGameUtil/PPGameRunlength.c
	$(CC) $(CFLAGS) $(CFLAGS_INCLUDE_DIRS) $(CC_OBJ_OUT_FLAG)$@ -c $?
 
$(OBJDIR)/PPGameSound.$(O): ./iPhone/PPGame/PPGameSound/PPGameSound.cpp
	$(CC) $(CXXFLAGS) $(CFLAGS_INCLUDE_DIRS) $(CC_OBJ_OUT_FLAG)$@ -c $?
 
$(OBJDIR)/PPGameSplite.$(O): ./iPhone/PPGame/PPGameView/PPGameSplite.cpp
	$(CC) $(CXXFLAGS) $(CFLAGS_INCLUDE_DIRS) $(CC_OBJ_OUT_FLAG)$@ -c $?
 
$(OBJDIR)/QBGame.$(O): ./iPhone/QBGameSystem/QBGame.cpp
	$(CC) $(CXXFLAGS) $(CFLAGS_INCLUDE_DIRS) $(CC_OBJ_OUT_FLAG)$@ -c $?
 
$(OBJDIR)/QBMenu.$(O): ./iPhone/QBGameSystem/QBMenu.cpp
	$(CC) $(CXXFLAGS) $(CFLAGS_INCLUDE_DIRS) $(CC_OBJ_OUT_FLAG)$@ -c $?
 
$(OBJDIR)/QBSound.$(O): ./iPhone/PPGame/PPGameSound/QBSound.cpp
	$(CC) $(CXXFLAGS) $(CFLAGS_INCLUDE_DIRS) $(CC_OBJ_OUT_FLAG)$@ -c $?
 
$(OBJDIR)/QBSoundMac.$(O): ./iPhone/PPGame/PPGameSound/QBSoundMac.cpp
	$(CC) $(CXXFLAGS) $(CFLAGS_INCLUDE_DIRS) $(CC_OBJ_OUT_FLAG)$@ -c $?
 
$(OBJDIR)/QBSoundWin.$(O): ./iPhone/PPGame/PPGameSound/QBSoundWin.cpp
	$(CC) $(CXXFLAGS) $(CFLAGS_INCLUDE_DIRS) $(CC_OBJ_OUT_FLAG)$@ -c $?
 
$(OBJDIR)/QBUtil.$(O): ./iPhone/QBGameSystem/QBUtil.cpp
	$(CC) $(CXXFLAGS) $(CFLAGS_INCLUDE_DIRS) $(CC_OBJ_OUT_FLAG)$@ -c $?
 
$(OBJDIR)/main.$(O): ./Windows/main.cpp
	$(CC) $(CXXFLAGS) $(CFLAGS_INCLUDE_DIRS) $(CC_OBJ_OUT_FLAG)$@ -c $?
 
$(OBJDIR)/PPGameUtilWin.$(O): ./Windows/PPGameUtilWin.cpp
	$(CC) $(CXXFLAGS) $(CFLAGS_INCLUDE_DIRS) $(CC_OBJ_OUT_FLAG)$@ -c $?
 
$(OBJDIR)/PPGameTextWin.$(O): ./Windows/PPGameTextWin.cpp
	$(CC) $(CXXFLAGS) $(CFLAGS_INCLUDE_DIRS) $(CC_OBJ_OUT_FLAG)$@ -c $?
 
$(OBJDIR)/PPGameMapWin.$(O): ./Windows/PPGameMapWin.cpp
	$(CC) $(CXXFLAGS) $(CFLAGS_INCLUDE_DIRS) $(CC_OBJ_OUT_FLAG)$@ -c $?
 
$(OBJDIR)/QBCommonGrobal.$(O): ./Windows/QBCommonGrobal.cpp
	$(CC) $(CXXFLAGS) $(CFLAGS_INCLUDE_DIRS) $(CC_OBJ_OUT_FLAG)$@ -c $?
 
$(OBJDIR)/QBTankGame.$(O): ./iPhone/QBGames/TankGame/QBTankGame.cpp
	$(CC) $(CXXFLAGS) $(CFLAGS_INCLUDE_DIRS) $(CC_OBJ_OUT_FLAG)$@ -c $?
 
$(OBJDIR)/QBGameInstant.$(O): ./iPhone/QBGameSystem/QBGameInstant.cpp
	$(CC) $(CXXFLAGS) $(CFLAGS_INCLUDE_DIRS) $(CC_OBJ_OUT_FLAG)$@ -c $?
 
$(OBJDIR)/QBNodeSystemCore.$(O): ./Windows/QBNode/QBNodeSystemCore.c
	$(CC) $(CXXFLAGS) $(CFLAGS_INCLUDE_DIRS) $(CC_OBJ_OUT_FLAG)$@ -c $?
 
$(OBJDIR)/QBNodeSystemValue.$(O): ./Windows/QBNode/QBNodeSystemValue.c
	$(CC) $(CXXFLAGS) $(CFLAGS_INCLUDE_DIRS) $(CC_OBJ_OUT_FLAG)$@ -c $?
 
$(OBJDIR)/QBNodeBase64.$(O): ./Windows/QBNode/QBNodeBase64.c
	$(CC) $(CXXFLAGS) $(CFLAGS_INCLUDE_DIRS) $(CC_OBJ_OUT_FLAG)$@ -c $?
 
$(OBJDIR)/QBNodeSystemPlist.$(O): ./Windows/QBNode/QBNodeSystemPlist.c
	$(CC) $(CXXFLAGS) $(CFLAGS_INCLUDE_DIRS) $(CC_OBJ_OUT_FLAG)$@ -c $?
 
$(OBJDIR)/QBNodeSystemUtil.$(O): ./Windows/QBNode/QBNodeSystemUtil.c
	$(CC) $(CXXFLAGS) $(CFLAGS_INCLUDE_DIRS) $(CC_OBJ_OUT_FLAG)$@ -c $?
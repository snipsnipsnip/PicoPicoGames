/*
  PicoPicoGames

  Copyright (c) 2009, Hiromitsu Yamaguchi, All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

  1. Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer. 

  2. Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution. 

  3. Neither the name of the Yamagame nor the names of its contributors
  may be used to endorse or promote products derived from this software
  without specific prior written permission. 

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __QBPICOHERO2_H__
#define __QBPICOHERO2_H__

#include "QBGame.h"
#include "PPGameMapData.h"

enum {
	QBCHAR_SHADOW,
	QBCHAR_FONT,
	QBCHAR_KEY,
//	QBCHAR_TEXT,
};

#define QBSHADOW_ITEM_MAX 9
#define QBSHADOW_MONSTER 1000
#define QBSHADOW_MONSTER_KIND 33

class QBPicoHero2 : public QBGame {
public:
	QBPicoHero2();
	virtual ~QBPicoHero2();

	//virtual bool KeyWait();
	//virtual unsigned long TouchIdle(bool touch,int x,int y);
	virtual int Idle();

	//ステップ処理関連//
	virtual int Step();
	virtual int SetStep(int step);
	virtual int NextStep();

	void FontPut(int x,int y,int pat,int scale);
	void TPrint(const char* format,...);

	virtual void PutFont(int x,int y,int pat,unsigned char r,unsigned char g,unsigned char b,unsigned char a);
	virtual void Put(int x,int y,int pat);
	virtual void Putx3(int x,int y,int pat);

	virtual PPGameTextureInfo* TextureInfo();
	virtual int KeyTextureID() {return QBCHAR_KEY;}
	virtual int KeySize();
	
	virtual bool ScrollMap();
	virtual bool ShadowEnding();
	virtual const char* MapDataName();
	virtual const char* SaveDataName();
	virtual void DrawItemIcon();
	virtual void InitPlayer();
	virtual void LevelUp();
	virtual void* EnemyData();
	virtual bool Continue();
	
	virtual void InitBGMSetting();
	
	//virtual int fontTextureID() {return 2;}
	//virtual int fontTextureID() {return 1;}
	//virtual int charTextureID() {return 1;}

	unsigned short m[256][256*2];
	int it[9+1];
	int pmax,plife,pstr;
	int pdef;
	int pexp,pnexp;

	unsigned short bg[256][256*2];
	int ex[QBSHADOW_MONSTER], ey[QBSHADOW_MONSTER], el[QBSHADOW_MONSTER];
	int eds[QBSHADOW_MONSTER_KIND], edd[QBSHADOW_MONSTER_KIND], edl[QBSHADOW_MONSTER_KIND], ede[QBSHADOW_MONSTER_KIND];
	int mx,my;
	int mpx,mpy;
	int tmpx,tmpy;
	int timer;
	//int fgt;
	int e_num;
	int boss;
	int px,py;
	int fskip;
	int ei;
	int ec;
	int dam;
	int prestep;
	int subcount;
	int et;
	
	int debug;
	
	int level;
	int statusx,statusy;
	int itemx,itemy;
	//int frameTimer;
	int continueCount;
	bool dark;

	void Define();
	void StartProgram();
	void SystemInit();
	void MainLoop();
	void ScrollLoop();
	void WaitDisp();
	void Ending();
	void GameOver();
	void FightPlayer();
	void FightPlayerSub();
	void FightEnemy();
	void FightEnemySub();
	void InitEnemy();
	int EnemyMove(int s);
	void GetItem();
	void MoveRock(int dx,int dy);
	void MoveWall(int dx,int dy);
	void MoveGrave(int dx,int dy);
	void MoveTree(int dx,int dy);
	void OpenDoor();
	void DrawItem();
	void DrawMap();
	void WaitVSync();
	void kEne(int ec,int ei);
	void Setting();
	
	void Save(const char* filepath);
	void LoadGame();
	void Load(const char* filepath);
	
	int mapWidth;
	int mapHeight;
	
	//unsigned long oldKey;
	
	int step;
};

#endif

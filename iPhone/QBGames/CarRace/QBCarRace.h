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

#ifndef __QBCARRACE_H__
#define __QBCARRACE_H__

#include "QBGame.h"
//#include "CData.h"

#define QBCARRACE_MAX_ENEMY 10
#define QBCARRACE_MAX_MARK 100

enum {
	QBCHAR_CARRACE,
	QBCHAR_FONT,
	QBCHAR_KEY,
//	QBCHAR_TEXT,
};

class QBCarRaceEnemy {
public:
	void init() {
		flag = 0;
		speed = 0;
	};
	
	float x,y;
	float speed;
	int flag;
	int hiddenCount;
};

class QBCarRace : public QBGame {
public:
	QBCarRace();
	virtual ~QBCarRace();

	//ステップ処理関連//
	virtual int Step();
	virtual int SetStep(int step);
	virtual int NextStep();

	//virtual bool KeyWait();
	//virtual unsigned long TouchIdle(bool touch,int x,int y);
	virtual int Idle();

	virtual int ExitGame();

	//virtual void Put(int x,int y,int pat,int type=0,int scale=1);
	virtual void PutFont(int x,int y,int pat,unsigned char r,unsigned char g,unsigned char b,unsigned char a);
	virtual void Put(int x,int y,int pat);
	
	virtual int KeyTextureID() {return QBCHAR_KEY;}

	//virtual int fontTextureID() {return 2;}
	//virtual int fontTextureID() {return 1;}
	//virtual int charTextureID() {return 0;}

	virtual PPGameTextureInfo* TextureInfo();
	virtual int KeySize();

	void Init();
	void InitScr();
	void Restart();
	void Title();
	void MainLoop();
	void GameOver();
	void DispScr(int title=0);
	void Scroll();
	void LoadStage(const char* f);
	void Setting();

	//char stage[500][256];
	int bg0[10+1][20+1];
	//int bg1[8+1][10+1];
	int mx;
	int my;
	int tm;
	int st;
	int sc;
	int speed;
	float scrct;
	int ac;
	int acTimer;
	int sndf;
	int loop;
	int mark[QBCARRACE_MAX_MARK];
	int markCount[QBCARRACE_MAX_MARK];
	
	float mapdy;
	int area;
	int arealoop;
	int areaMax;
	
	int stage;
	int stagedisp;
	
	int gameoverCount;
	
	int timer;
	int lapTime;
	int clearTimer;
	
	int hiscore;
	int frameTimer;
	
	int prebkey;
	
	int dispHiScore;
	
	QBCarRaceEnemy enemy[QBCARRACE_MAX_ENEMY];
	
	void initCar();
	void setCar(int sety=0);
	void idleCar();
	void dispCar();
	
	int enemySetTimer;
	int enemySetInterval;
	
	void Save(const char* filepath);
	void LoadGame();
	void Load(const char* filepath);
	
	int sdtimer;
	
	//CData d;//(PPGameResourcePath("Ganon.ogg"));
	//QBPCM pcm;
	
	unsigned long oldKey;
	
	int step;
};

#endif

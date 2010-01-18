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

#ifndef __QBTSHADOW_H__
#define __QBTSHADOW_H__

#include "QBGame.h"

#define mapw 127
#define maph 127
#define	kmapw ((mapw - 8) / 16 - 1)
#define	kmaph ((maph - 8) / 16 - 1)
#define MAX_POLY 256

enum {
	QBCHAR_TSHADOW_TSHADOWCHAR,
	QBCHAR_TSHADOW_FONT,
	QBCHAR_TSHADOW_KEY,
//	QBCHAR_TSHADOW_TEXT,
};

typedef struct _QBTShadow_POLY {
	int group;
	int color;
	int x,y;
	int w,h;
	unsigned long pat;
} QBTShadow_POLY;

class QBTShadow : public QBGame {
public:
	QBTShadow();
	virtual ~QBTShadow();

	//ステップ処理関連//
	virtual int Step();
	virtual int SetStep(int step);
	virtual int NextStep();

	void Scale(int x,int y=1);
	void PrintOffset(int dx=0,int dy=0);
	void TPrint(const char* format,...);
	virtual void Print(const char* format,...);

	virtual bool CKeyWait();
	virtual bool KeyWaitWithArrowKey();
	//virtual unsigned long TouchIdle(bool touch,int x,int y);
	virtual int Idle();
	virtual int IdleCore();
	virtual int PSET(int x,int y,int color);
	//virtual void DrawSpecial(GLGameSplite* g);
	virtual void CLS();
	virtual void Put(int x,int y,int pat,int type=QBCHAR_TSHADOW_TSHADOWCHAR,int scale=1);
	virtual void CLEAR();
	virtual void LINE(int x1,int y1,int x2,int y2,int color);
	virtual void FILL(int x1,int y1,int x2,int y2,int color);
	virtual void BOX(int x1,int y1,int x2,int y2,int color);
	virtual int PLAY(const char* mml);
	virtual int STOP();
	virtual void Fill(int x1,int y1,int x2,int y2,int color);
	virtual void Box(int x1,int y1,int x2,int y2,int color);

	virtual void FontPut(int x,int y,int pat,int scale=1);

	virtual void PutFont(int x,int y,int pat,unsigned char r,unsigned char g,unsigned char b,unsigned char a);

	virtual PPGameTextureInfo* TextureInfo();
	virtual int KeyTextureID() {return QBCHAR_TSHADOW_KEY;}
	
	//virtual int fontTextureID() {return 2;}
	//virtual int fontTextureID() {return 1;}
	//virtual int charTextureID() {return 2;}

	int poly_que;
	int poly_st;
	int poly_count[100];
	QBTShadow_POLY poly_buff[100][MAX_POLY];
	int key_wait[100];
	int ckey;
	
	int bg0[mapw+1][maph+1];						//扉、アイテム、トラップ等
	int bg1[mapw+1][maph+1];						//壁
	int bg2[kmapw + 1+1][kmaph + 1+1];				//マップ作成用
	int rx[kmapw+1][kmaph+1],ry[kmapw+1][kmaph+1];	//部屋の位置
	int rw[kmapw+1][kmaph+1],rh[kmapw+1][kmaph+1];	//部屋のサイズ
	int itm[4+1], bgdata[6+1], nx[9+1], ny[9+1];
	int tx[16+1], ty[16+1], tf[16+1];
	int ex[32+1], ey[32+1], el[32+1], ek[32+1], edl[7+1], eds[7+1], edd[7+1], ede[7+1];
	char edata[15+1][2+1];
	const char* its[18+1];
	int eNum;
	int texId;
	int endingStep;
	int endingCounter;
	
	unsigned char _pixel[256*256*4];
	
	int floorNum;
	int kfloorNum;

	int plife,pmax;
	int pstr,pdef,pfood,pfmax;
	int pkey,psel,pct,pdoku,pexp,pnexp;
	int startx,starty;
	int rmx,rmy;
	int mx,my;
	int px,py;
	int x,y;
	int dir;
	int doorx,doory;
	int doorRoom;
	int exitf;
	int exitx,exity;
	int keyx,keyy;
	int megusuri;
	int prek,k;
	int pskip;
	int dam;
	int nnum;
	int tnum;
	int eex,eey;
	int estr;
	int ox,oy;
	int ax,ay;
	int dx,dy;
	int esx,esy;
	int yax,yay;
	int sx,sy;
	int num0;
	int num1;
	int num2;
	
	int frameskip;
	
	int oldmx;
	int oldmy;
	int q;
	int j;
	
	int frameTimer;
	//bool hiraDispFlag;

	const char* stS;

//	int directonChange;

	void InitPowerOn();
	void GameStart();
	void MapCreate();
	void MainLoop();
	void Title();
	void GameOver();
	void Ending();
	void DirecChange();
	void PrevFloor();
	void NextFloor();
	void PlayerFight();
	void kEne(int j);
	void lUp();
	void EneDam();
	void ItemBreak();
	void pDamAnim();
	void EnemyFight();
	void EnemyDisp();
	void EnemyMove();
	void EnemyShot(int j);
	void UseItem();
	void EatFood();
	void ThrowYa();
	void Yakusou();
	void Dokukesi();
	void Megusuri();
	void Needle();
	int Trap();
	int GetPosXY(int trap=0);
	int GetPosXYWithoutEnemy(int enemynum,int trap=0);
	void GetRoom();
	int GetPos(int trap=0);
	void MapAllDisp();
	void DispScr();
	void MakeRoad(int x1,int y1,int x2,int y2);
	void AreaInc();
	void AreaIncSub1();
	void WaitVSync();
	void HiraDisp(int disp=true);
	void Setting();

	int _fx,_fy;
	
	int _printOffsetX;
	int _printOffsetY;
	
	//unsigned long oldKey;

	bool touchCheckWithPoly(float* keyPos,float dx,float dy);
	
	int truckStep;
	float truckTouchY;
	float truckTouchX;

	int selectItem;
	int preSelectItem;
	int directonChange;

	int step;
};

#endif

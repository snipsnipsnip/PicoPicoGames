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

#ifndef __QBMISSILEPANIC_H__
#define __QBMISSILEPANIC_H__

#include "QBGame.h"

enum {
	QBCHAR_MISSILEPANIC,
	QBCHAR_FONT,
	QBCHAR_FONT2,
	QBCHAR_KEY,
};

#define QBMISSILEPANIC_OBJ_MAX    256
#define QBMISSILEPANIC_MAP_WIDTH  256
#define QBMISSILEPANIC_MAP_HEIGHT 256
#define QBMISSILEPANIC_SMOKE_MAX 256

class QBMissilePanic : public QBGame {
public:
	QBMissilePanic();
	virtual ~QBMissilePanic();
	virtual int Idle();
	
	//ステップ処理関連//
	virtual int Step();
	virtual int SetStep(int step);
	virtual int NextStep();

	virtual void PutFont(int x,int y,int pat,unsigned char r,unsigned char g,unsigned char b,unsigned char a);
	virtual void Put(int x,int y,int pat);
	void PutHalf(int x,int y,int pat);
	void FontPut(int x,int y,int pat,int scale);
	void TPrint(const char* format,...);

	virtual int KeyTextureID() {return QBCHAR_KEY;}

	virtual PPGameTextureInfo* TextureInfo();
	virtual int KeySize();
	virtual unsigned long VisibleButton();
	
	int missile_x,missile_y;
	int missile_energy;
	int missile_energy_max;
	int missile_dir;
	int missile_speed;
	int missile_start_speed;
	int missile_speed_up_count;
	int missile_count;

	int smoke_start_c;
	int smoke_c;
	int smoke_x[QBMISSILEPANIC_SMOKE_MAX];
	int smoke_y[QBMISSILEPANIC_SMOKE_MAX];
	
	int target_count;
	int flag_count;
	int energy_count;
	
	int key_wait;
	
	int scr_x,scr_y;
	int scr_w,scr_h;
	int scr_offset_x,scr_offset_y;
	
	int play_start_time;
	int play_time;
	
	unsigned long bang_count;
	unsigned long bang_timer;
	
	int flash_count;

	unsigned long hi_score;
	unsigned long score;
	int stage;
	
	int touchChange;
	
	int count;
	
	int map_w,map_h;
	unsigned char bg[2][QBMISSILEPANIC_MAP_HEIGHT][QBMISSILEPANIC_MAP_WIDTH];

	bool bigKeyArea;
	bool key_disp;
	
	int ox[QBMISSILEPANIC_OBJ_MAX];
	int oy[QBMISSILEPANIC_OBJ_MAX];
	int otype[QBMISSILEPANIC_OBJ_MAX];
	int ochar[QBMISSILEPANIC_OBJ_MAX];
	int ost[QBMISSILEPANIC_OBJ_MAX];
	int oct[QBMISSILEPANIC_OBJ_MAX];
	
	int SetObj(int type,int x,int y);
	void StartGame();
	void DrawBG();
	void DrawInfo();
	void DrawObj();
	void DrawBang();
	void DrawFlash();
	void MoveObj();
	void MoveMissle();
	void CalcScreenPos();
	void HitCheck(int f);
	void StageClear();
	void DrawScore();
	void DrawHiScore();

	void drawTitle();
	void doTitle(int touch);
	void doSetting();
	
	int CalcPosX(int x);
	int CalcPosY(int y);

	int LimitX(int x);
	int LimitY(int y);
	
	int step;
};

#endif

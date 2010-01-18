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

#ifndef __QBBLUEBALL_H__
#define __QBBLUEBALL_H__

#include "QBGame.h"

#define QBBLUEBALL_W_SIZE 16
#define QBBLUEBALL_H_SIZE 13
#define QBBLUEBALL_STAGEMAX 64

enum {
	QBCHAR_BLUEBALL,
	QBCHAR_FONT,
	QBCHAR_KEY,
};

class QBBlueBall : public QBGame {
public:
	QBBlueBall();
	virtual ~QBBlueBall();

	//ステップ処理関連//
	virtual int Step();
	virtual int SetStep(int step);
	virtual int NextStep();

	virtual int InitGame();
	virtual int ExitGame();
	virtual int Idle();
	
	virtual void Put(int x,int y,int pat);
	virtual void PutFont(int x,int y,int pat,unsigned char r,unsigned char g,unsigned char b,unsigned char a);

	virtual PPGameTextureInfo* TextureInfo();
	virtual int KeyTextureID() {return QBCHAR_KEY;}
	virtual int KeySize();
	virtual unsigned long WindowLayout();
	virtual unsigned long VisibleButton();
	
	void SelectStage();
	void RestartGame();
	void Setting();
	void MainLoop();
	void Ending();
	
	void GoStep(int _step);
	void GoNextStep();
	
	void DrawLogo();
	
	//void Title();
	void DrawStageSelect();
	void DrawStage();
	void PutChar(short x,short y,int pat);
	void ResetStage(void);
	void StageGet(void);
	void MoveBlock(short x,short y,short dx,short dy);
	
	//int Locale();

	unsigned char bg[QBBLUEBALL_W_SIZE][QBBLUEBALL_H_SIZE];
	int gStage;
	int clearflag;
	
	int subStep;

	int parts;
	int temp_parts_flag;
	int temp_parts;
	int touchChange;
	
	int SelectOK[64];
	char ClearData[64];
	
	int selectStage;
	int frameTimer;
	
	unsigned long oldKey;

	bool buttonVisible;
	bool abButtonVisible;

	int truckStep;
	float truckTouchY;
	float truckTouchX;

	int step;
};

#endif

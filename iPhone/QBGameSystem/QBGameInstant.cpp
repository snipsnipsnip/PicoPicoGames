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

#include "QBGameInstant.h"
#include <stdio.h>
#include <stdlib.h>
#include "font.h"
#include "font2.h"
#include "key.h"

enum {
	QBCHAR_FONT,
	QBCHAR_FONT2,
	QBCHAR_KEY,
};

enum {
	STEP_INIT,
	STEP_TITLE,
	STEP_OPTION,
	STEP_GAME
};

static PPGameTextureInfo texList[] = {
	{"font.png",font_group,false},
	{"font2.png",font2_group,false},
	{"key.png",key_group,false},
	{NULL,NULL,false}
};

QBGameInstant::QBGameInstant()
{
	mainStep = 0;
	mainSubStep = 0;
}

QBGameInstant::~QBGameInstant()
{
}

PPGameTextureInfo* QBGameInstant::TextureInfo()
{
	return texList;
}

void QBGameInstant::PutFont(int x,int y,int pat,unsigned char r,unsigned char g,unsigned char b,unsigned char a)
{
	QBGame::Put(x,y,pat,PG_FONT,QBCHAR_FONT,r,g,b,a);
}

int QBGameInstant::KeyTextureID()
{
	return QBCHAR_KEY;
}

int QBGameInstant::Idle()
{
	Clear();
	switch (mainStep) {
	case STEP_INIT:
		gameInit();
		mainStep ++;
	case STEP_TITLE:
		doTitle();
		break;
	case STEP_OPTION:
		doOption();
		break;
	case STEP_GAME:
		if (gameIdle() !=0) {
			goStep(STEP_TITLE);
		}
		break;
	}
	return 0;
}

void QBGameInstant::goStep(int s,int sub)
{
	mainStep = s;
	mainSubStep = sub;
}

void QBGameInstant::FontPut(int x,int y,int pat,int scale)
{
	int i = polyCount;
	unsigned char r,g,b;
	
	GetRGBColor(color,&r,&g,&b);

	poly[i].r = r;
	poly[i].g = g;
	poly[i].b = b;
	poly[i].alpha = 255;
	poly[i++].splite(x,y,pat,PG_FONT2,QBCHAR_FONT2,scale);
	
	polyCount++;
}

void QBGameInstant::Print2(const char* format,...)
{
	int x,y;
	x = locatex;
	y = locatey;
	{
		int len = 0;
		va_list args;
		va_start(args,format);
		{
			len = vsnprintf(NULL,0,format,args)+1;
			if (len > 0) {
				char* str = (char*)calloc(1,len);
				if (str) {
					vsnprintf(str,len,format,args);
					for (int i=0;i<len-1;i++) {
						FontPut(x,y,str[i],2);
						x += 32;
					}
					free(str);
				}
			}
		}
	}
	locatex = x;
	locatey = y;
}

void QBGameInstant::drawTitle()
{
	Color(12);
	Offset(0+4,3*16+4);
	Print2("SAMPLEGAME");
	Color(15);
	Offset(0-4+4,3*16+4-4);
	Print2("SAMPLEGAME");
}

void QBGameInstant::doTitle()
{
	drawTitle();
	Color(15);
	switch (mainSubStep) {
	case 0:
		curMenu = 0;
		menu[curMenu].Init();
		mainSubStep ++;
	case 1:
		menu[curMenu].x = 4;
		menu[curMenu].y = 9;
		menu[curMenu].w = 12;
		menu[curMenu].h = 12;
		menu[curMenu].string[0] = "START";
		menu[curMenu].string[1] = "CONTINUE";
		menu[curMenu].string[2] = "OPTION";
		menu[curMenu].string[3] = "EXIT";
		menu[curMenu].draw(this);

		{
			menu[curMenu].touchOK = true;
			if (KeyWait(PAD_UP)) {
				menu[curMenu].up();
			} else
			if (KeyWait(PAD_DOWN)) {
				menu[curMenu].down();
			}
			if (KeyWait(PAD_A,true) || KeyWait(PAD_B,true) || menu[curMenu].select) {
				if (menu[curMenu].cur == 0) {
					gameStart();
					goStep(STEP_GAME);
				} else
				if (menu[curMenu].cur == 1) {
					gameContinue();
					goStep(STEP_GAME);
				} else
				if (menu[curMenu].cur == 2) {
					menu[curMenu].play();
					goStep(STEP_OPTION);
				} else {
					//exitGame = true;
					Exit();
				}
			}
		}
		break;
	}
}

void QBGameInstant::doOption()
{
	drawTitle();
	Color(15);
	switch (mainSubStep) {
	case 0:
		curMenu = 1;
		menu[curMenu].Init();
		mainSubStep ++;
	case 1:
		menu[curMenu].x = 3;
		menu[curMenu].y = 9;
		menu[curMenu].w = 14;
		menu[curMenu].h = 13;
		menu[curMenu].string[0] = "EXIT";
		menu[curMenu].string[1] = "BGM PLAY";
		menu[curMenu].string[2] = "BGM ENDING";
		menu[curMenu].string[3] = "BGM RESET";
		menu[curMenu].string[4] = NULL;
		menu[0].draw(this);
		menu[curMenu].draw(this);

		if (KeyWait(PAD_UP)) {
			menu[curMenu].up();
		} else
		if (KeyWait(PAD_DOWN)) {
			menu[curMenu].down();
		}
		if (KeyWait(PAD_RIGHT) || KeyWait(PAD_LEFT)) {
			if (menu[curMenu].cur > 0 && menu[curMenu].cur < 4) {
				PlayBGM(menu[curMenu].cur);
				menu[curMenu].play();
			}
		}
		if (KeyWait(PAD_A,true) || KeyWait(PAD_B,true) || menu[curMenu].select) {
			switch (menu[curMenu].cur) {
			case 0:			//EXIT
				curMenu = 0;
				menu[curMenu].play();
				goStep(STEP_TITLE,1);
				StopBGM();
				break;
			case 3:			//RESET
				curMenu = 0;
				ResetBGM();
				StopBGM();
				menu[curMenu].playDispose();
				goStep(STEP_TITLE,1);
				break;
			default:		//OTHER
				SelectBGM(menu[curMenu].cur);
				menu[curMenu].play();
				menu[curMenu].cur ++;
				break;
			}
		}
		break;
	}
}

void QBGameInstant::gameInit()
{
	InitBGM(1,"QBGameInstantPlay");
	InitBGM(2,"QBGameInstantEnding");
}

void QBGameInstant::gameStart()
{
}

void QBGameInstant::gameContinue()
{
	gameStart();
}

int QBGameInstant::gameIdle()
{
	return 0;
}

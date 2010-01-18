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

#include "QBBlueBall.h"
#include "key.h"
#include "font2.h"
#include "QBBlueBallStageData.h"
#define _PG_GLOBAL_
#include "BlueBallChar.h"

static PPGameTextureInfo texList[] = {
	{"BlueBallChar.png",BlueBallChar_group,false},
	{"font2.png",font2_group,false},
	{"key.png",key_group,false},
	{NULL,NULL,false}
};

#define S_W_SIZE QBBLUEBALL_W_SIZE
#define S_H_SIZE QBBLUEBALL_H_SIZE
#define SPMAX (QBBLUEBALL_W_SIZE*QBBLUEBALL_H_SIZE)

enum {
	STEP_INIT,
	STEP_TITLE,
	STEP_TITLE_INIT,
	STEP_SETTING,
	STEP_MAINLOOP,
	STEP_RESTART,
	STEP_ENDING,
};

enum {
	C_PLATE,
	C_BLOCK,
	C_HOLE,				//カギ
	C_SPACE,			//水路
	C_BALL,				//兵士
	C_RED_BLOCK,		//つぼ
	C_BLUE_BLOCK,		//モンスタ
	C_BLACK,
	C_HIBI_1,
	C_HIBI_2,
	C_UP,
	C_DOWN,
	C_LEFT,
	C_RIGHT,
	C_BALL_HOLE,		//落とし穴
	C_PARTS,			//剣
	C_YELLOW_BLOCK,		//青い玉

	C_NUM_0,
	C_NUM_1,
	C_NUM_2,
	C_NUM_3,
	C_NUM_4,
	C_NUM_5,
	C_NUM_6,
	C_NUM_7,
	C_NUM_8,
	C_NUM_9,
	C_WAKU,
	C_SELECT,
	
	C_SELECT_TITLE,
	C_CONGRATURATION,
	C_STAGECLEAR,
	
	C_SWORD				//兵士の持つ剣
};

typedef struct TRect {
	int left;
	int top;
	int width;
	int height;
} TRect;

static TRect PatData[] = {
	{128,192,32,32},	//plate
	{288,192,32,32},	//block	
	{160,192,32,32},	//hole
	{256,192,32,32},	//space
	{0,192,32,32},		//ball	
	{32,192,32,32},		//red block
	{192,192,32,32},	//blue block	
	{32,0,40,40},		//black	
	{96,192,32,32},		//hibi brack 1
	{224,192,32,32},	//hibi brack 1

	{384,192,32,32},	//up brack
	{384+32,192,32,32},	//down brack
	{384+64,192,32,32},	//left brack
	{384+96,192,32,32},	//right brack

	{0,224,32,32},		//ball hole
	{352,192,32,32},	//parts
	{32,224,32,32},		//yellow block

	{ 0+128,224,10,24},	//0
	{10+128,224,10,24},	//1
	{20+128,224,10,24},	//2
	{30+128,224,10,24},	//3
	{40+128,224,10,24},	//4
	{50+128,224,10,24},	//5
	{60+128,224,10,24},	//6
	{70+128,224,10,24},	//7
	{80+128,224,10,24},	//8
	{90+128,224,10,24},	//9
	{256,224,32,32},	//waku
	{256+32,224,32,32},	//select
	
	{96,0,384,64},		//select title
	{0,64,256,128},		//congraturation
	{256,64,256,128},	//stage clear
	
	{0,192-32,32,32},	//sword
};

static unsigned char stage[] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x01,0x02,0x03,0x80,0x40,0x06,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x20
};

#define MAXPATTERN (sizeof(PatData)/sizeof(TRect))

static int CharData[MAXPATTERN];

typedef struct TSprite {
	int x;
	int y;
	int char_id;
	int speed;
} TSprite;

static TSprite SP_Data[SPMAX];

QBBlueBall::QBBlueBall()
{
	for (int i=0;i<MAXPATTERN;i++) {
		CharData[i] = PatData[i].top/32*(512/32) + PatData[i].left/32;
	}
	parts = 0;
	gStage = 0;
	subStep = 0;
	buttonVisible = false;
	abButtonVisible = false;
	touchChange = 0;
	oldKey = 0;
	truckStep = 0;
	step = 0;
	InitBGM(1,"QBBlueBallBGM");
	InitBGM(2,"QBBlueBallEnding");
	InitGame();
}

QBBlueBall::~QBBlueBall()
{
	ExitGame();
}

PPGameTextureInfo* QBBlueBall::TextureInfo()
{
	return texList;
}

int QBBlueBall::KeySize()
{
	if (Step() == 4) {
		return 96*96;
	}
	return 64*64;
}

unsigned long QBBlueBall::WindowLayout()
{
	return QBGAME_CONTROLLER_TYPE_H_LEFT;
}

unsigned long QBBlueBall::VisibleButton()
{
	if (abButtonVisible) return QBGAME_CONTROLLER_ENABLE_ARROW | QBGAME_CONTROLLER_ENABLE_A;
	if (!buttonVisible) return 0;
	return QBGAME_CONTROLLER_ENABLE_ARROW;
}

int QBBlueBall::InitGame()
{
	for (int i=0;i<64;i++) {
		SelectOK[i] = 0;
		ClearData[i] = 0;
	}
	for (int i=0;i<8;i++) {
		SelectOK[i] = 1;
	}
	{
		int size;
		unsigned char* data = GetData("QBBlueBallSelectOK",&size);
		if (data) {
			memcpy(SelectOK,data,sizeof(SelectOK));
		}
	}
	{
		int size;
		unsigned char* data = GetData("QBBlueBallClearData",&size);
		if (data) {
			memcpy(ClearData,data,sizeof(ClearData));
		}
	}
	return 0;
}

int QBBlueBall::ExitGame()
{
	SetData("QBBlueBallSelectOK",(unsigned char*)SelectOK,sizeof(SelectOK));
	SetData("QBBlueBallClearData",(unsigned char*)ClearData,sizeof(ClearData));
	return 0;
}

void QBBlueBall::GoStep(int _step)
{
	SetStep(_step);
	subStep = 0;
}

void QBBlueBall::GoNextStep()
{
	NextStep();
	subStep = 0;
}

void QBBlueBall::Put(int x,int y,int pat)
{
	QBGame::Put(x,y,pat,PG_BLUEBALL,QBCHAR_BLUEBALL);
}

void QBBlueBall::PutFont(int x,int y,int pat,unsigned char r,unsigned char g,unsigned char b,unsigned char a)
{
	QBGame::Put(x,y,pat,PG_FONT2,QBCHAR_FONT,r,g,b,a);
}

int QBBlueBall::Idle()
{

	//フリック処理
	{
		int dir = 0;
		{
			if (TouchCount() > 0) {
				switch (truckStep) {
				case 0:
					truckTouchY = TouchYPosition(0);
					truckTouchX = TouchXPosition(0);
					truckStep ++;
					break;
				case 1:
					if (truckTouchY - TouchYPosition(0) > 64) {
						dir = PAD_UP;
						truckStep = 2;
					} else 
					if (truckTouchY - TouchYPosition(0) < -64) {
						dir = PAD_DOWN;
						truckStep = 2;
					}
					if (truckTouchX - TouchXPosition(0) > 64) {
						dir = PAD_LEFT;
						truckStep = 2;
					}
					if (truckTouchX - TouchXPosition(0) < -64) {
						dir = PAD_RIGHT;
						truckStep = 2;
					}
					break;
				}
			} else {
				truckStep = 0;
			}
		}
		SetKey(GetKey() | dir);
	}

	{
		unsigned long key;
		key = GetKey();

		Clear();
		Fill(0,0,480,320,1);

		if (key == oldKey && key != 0) {
			frameTimer ++;
			if (frameTimer > 20) {
				frameTimer = 15;
				oldKey = 0;
			}
		}
		if (key == 0) {
			frameTimer = 0;
		}

		switch (Step()) {
		case STEP_INIT:
			buttonVisible = true;
			gStage = 0;
			GoNextStep();
		case STEP_TITLE:
			SelectStage();
			break;
		case STEP_TITLE_INIT:
			buttonVisible = true;
			menu[curMenu].cur = 0;
			SelectStage();
			GoStep(STEP_TITLE);
			break;
		case STEP_SETTING:
			Setting();
			break;
		case STEP_RESTART:
			RestartGame();
			break;
		case STEP_MAINLOOP:
			MainLoop();
			break;
		case STEP_ENDING:
			buttonVisible = false;
			Ending();
			break;
		}
		oldKey = GetKey();
	}
	
	return 0;
}

enum {
	STEP_MAINLOOP_INIT,
	STEP_MAINLOOP_START,
	STEP_MAINLOOP_LOOP,
	STEP_MAINLOOP_RESTART_INIT,
	STEP_MAINLOOP_RESTART,
	STEP_MAINLOOP_GIVEUP_INIT,
	STEP_MAINLOOP_GIVEUP,
	STEP_MAINLOOP_CLEAR_INIT,
	STEP_MAINLOOP_CLEAR
};

void QBBlueBall::MainLoop()
{
	switch (subStep) {
	case STEP_MAINLOOP_INIT:
		curMenu = 0;
		menu[curMenu].Init();
		menu[curMenu].offsetx = 8;
		menu[curMenu].x = -1;
		menu[curMenu].y = 3;
		menu[curMenu].w = 9;
		menu[curMenu].h = 10;
		menu[curMenu].fillColor = -1;
		menu[curMenu].frameColor = -1;
		menu[curMenu].string[0] = "RESTART";
		menu[curMenu].string[1] = "SELECT";
		menu[curMenu].cur = 0;
		subStep ++;
	case STEP_MAINLOOP_START:
		StageGet();
		subStep ++;
		buttonVisible = true;
		menu[curMenu].enable = true;
		menu[curMenu].draw(this);
		DrawStage();
		PlayBGM(1);
		break;
	case STEP_MAINLOOP_RESTART_INIT:
		curMenu = 1;
		menu[curMenu].Init();
		menu[curMenu].x = (30-8)/2;
		menu[curMenu].y = 10;
		menu[curMenu].w = 8;
		menu[curMenu].h = 6;
		if (Locale() == QBGAME_LOCALE_JAPANESE) {
			menu[curMenu].string[0] = "いいえ";
			menu[curMenu].string[1] = "はい";
		} else {
			menu[curMenu].string[0] = "NO";
			menu[curMenu].string[1] = "YES";
		}
		menu[curMenu].string[2] = NULL;
		subStep ++;
		break;
	case STEP_MAINLOOP_RESTART:
		buttonVisible = false;
		menu[0].enable = false;
		menu[0].draw(this);
		DrawStage();
		Fill(40,80,400,48,0);
		Box(40,80,400,48,15);
		
		Offset(40+16,80+16);
		Color(15);
		if (Locale() == QBGAME_LOCALE_JAPANESE) {
			Print("リスタートしますか？");
		} else {
			Print("Do you want to restart ?");
		}
		menu[curMenu].draw(this);

		if (KeyWait(PAD_A,true) || menu[curMenu].select) {
			switch (menu[curMenu].cur) {
			case 0:			//いいえ
				menu[curMenu].play();
				curMenu = 0;
				subStep = STEP_MAINLOOP_LOOP;
				break;
			case 1:			//はい
				menu[curMenu].play();
				curMenu = 0;
				subStep = STEP_MAINLOOP_INIT;
				break;
			}
		}
		break;
	case STEP_MAINLOOP_GIVEUP_INIT:
		curMenu = 1;
		menu[curMenu].Init();
		menu[curMenu].x = (30-8)/2;
		menu[curMenu].y = 10;
		menu[curMenu].w = 8;
		menu[curMenu].h = 6;
		if (Locale() == QBGAME_LOCALE_JAPANESE) {
			menu[curMenu].string[0] = "いいえ";
			menu[curMenu].string[1] = "はい";
		} else {
			menu[curMenu].string[0] = "NO";
			menu[curMenu].string[1] = "YES";
		}
		menu[curMenu].string[2] = NULL;
		subStep ++;
		break;
	case STEP_MAINLOOP_GIVEUP:
		buttonVisible = false;
		menu[0].enable = false;
		menu[0].draw(this);
		DrawStage();
		Fill(40,80,400,48,0);
		Box(40,80,400,48,15);
		
		Offset(40+16,80+16);
		Color(15);
		if (Locale() == QBGAME_LOCALE_JAPANESE) {
			Print("ステージせんたくしますか？");
		} else {
			Print("Do you select stage ?");
		}
		menu[curMenu].draw(this);

		if (KeyWait(PAD_A,true) || menu[curMenu].select) {
			switch (menu[curMenu].cur) {
			case 0:			//いいえ
				menu[curMenu].play();
				curMenu = 0;
				subStep = STEP_MAINLOOP_LOOP;
				break;
			case 1:			//はい
				menu[curMenu].play();
				curMenu = 0;
				//subStep = STEP_MAINLOOP_INIT;
				GoStep(STEP_TITLE_INIT);
				break;
			}
		}
		break;
	case STEP_MAINLOOP_LOOP:
		buttonVisible = true;
		menu[curMenu].enable = true;
		menu[curMenu].draw(this);
		DrawStage();
		if (menu[curMenu].select) {
			switch (menu[curMenu].cur) {
			case 0:					//RESTART
				menu[curMenu].play();
				subStep = STEP_MAINLOOP_INIT;
				//subStep = STEP_MAINLOOP_RESTART_INIT;
				break;
			case 1:					//GIVE UP
				menu[curMenu].play();
				GoStep(STEP_TITLE_INIT);
				StopBGM();
				//subStep = STEP_MAINLOOP_GIVEUP_INIT;
				break;
			}
		}
		if (KeyWait(PAD_A)) {
			gStage --;
			if (gStage < 0) gStage = 63;
				subStep = STEP_MAINLOOP_INIT;
		}
		if (KeyWait(PAD_B)) {
			gStage ++;
			if (gStage > 63) gStage = 0;
				subStep = STEP_MAINLOOP_INIT;
		}
		{
			int x,y;
			if (KeyWait(PAD_UP)) {
				menu[curMenu].play();
				temp_parts = parts;
				temp_parts_flag = 0;
				for (y=1;y<S_H_SIZE-1;y++) {
					for (x=0;x<S_W_SIZE;x++) {
						if (bg[x][y] & 0x80) {
							MoveBlock(x,y,0,-1);
						}
					}
				}
				parts = temp_parts;
				if (temp_parts_flag) parts --;
			}
			if (KeyWait(PAD_DOWN)) {
				menu[curMenu].play();
				temp_parts = parts;
				temp_parts_flag = 0;
				for (y=S_H_SIZE-3;y>=0;y--) {
					for (x=0;x<S_W_SIZE;x++) {
						if (bg[x][y] & 0x80) {
							MoveBlock(x,y,0,1);
						}
					}
				}
				parts = temp_parts;
				if (temp_parts_flag) parts --;
			}
			if (KeyWait(PAD_LEFT)) {
				menu[curMenu].play();
				temp_parts = parts;
				temp_parts_flag = 0;
				for (x=1;x<S_W_SIZE;x++) {
					for (y=0;y<S_H_SIZE-1;y++) {
						if (bg[x][y] & 0x80) {
							MoveBlock(x,y,-1,0);
						}
					}
				}
				parts = temp_parts;
				if (temp_parts_flag) parts --;
			}
			if (KeyWait(PAD_RIGHT)) {
				menu[curMenu].play();
				temp_parts = parts;
				temp_parts_flag = 0;
				for (x=S_W_SIZE-2;x>=0;x--) {
					for (y=0;y<S_H_SIZE-1;y++) {
						if (bg[x][y] & 0x80) {
							MoveBlock(x,y,1,0);
						}
					}
				}
				parts = temp_parts;
				if (temp_parts_flag) parts --;
			}
			{
				int x,y;
				int a,b,c;
				clearflag = 0;
				a = 0;
				b = 0;
				c = 0;
				for (y=0;y<S_H_SIZE-1;y++) {
					for (x=0;x<S_W_SIZE;x++) {
						if (bg[x][y] & 0x80) {
							c++;
							if ((bg[x][y] & 0x1f)==C_HOLE) {
								a++;
							}
						}
						if ((bg[x][y] & 0x1f)==C_HOLE) {
							b++;
						}
					}
				}
				if (a!=0 && a==b && a==c) {
					clearflag = 1;
				}
			}
			if (clearflag == 1) {
				//gStage ++;
				//if (gStage >= 64) gStage = 0;
				StopBGM();
				subStep = STEP_MAINLOOP_CLEAR_INIT;
			}
		}
		break;
	case STEP_MAINLOOP_CLEAR_INIT:
		Play("t255M0o4l64v32CRDREFGAB>C<");
		buttonVisible = true;
		menu[0].enable = false;
		subStep = STEP_MAINLOOP_CLEAR;
	case STEP_MAINLOOP_CLEAR:
		menu[0].draw(this);
		DrawStage();

		{
			int px,py;
			px = (480-6*16)/2;
			py = (320+16)/2-32;

			Fill((480-(13*16+8))/2,py-4,13*16+8,24,0);
			Box((480-(13*16+8))/2,py-4,13*16+8,24,15);
			
			Offset(px,py);
			Color(12);
			Print("CLEAR!");
		}
		
		if (TouchCount() > 0 && touchChange != TouchCount()) {
			ClearData[gStage] = 1;
			{
				if (gStage-1 >= 0 && gStage-1 < 64) SelectOK[gStage-1] = 1;
				if (gStage+1 >= 0 && gStage+1 < 64) SelectOK[gStage+1] = 1;
				if (gStage-8 >= 0 && gStage-8 < 64) SelectOK[gStage-8] = 1;
				if (gStage+8 >= 0 && gStage+8 < 64) SelectOK[gStage+8] = 1;
			}
			//gStage ++;
			//if (gStage >= 64) gStage = 0;
			int c = 0;
			for (int i=0;i<64;i++) {
				if (ClearData[i] != 0) {
					c ++;
				}
			}
			if (c >= 64) {
				PlayBGM(2);
				GoStep(STEP_ENDING);
			} else {
				GoStep(STEP_TITLE_INIT);
			}
		}
		
		break;
	}

	touchChange = TouchCount();
}

//int QBBlueBall::Locale()
//{
//	return QBGAME_LOCALE_JAPANESE;
//}

void QBBlueBall::DrawStageSelect()
{
	DrawLogo();

	int size=32;
	int px=480-256-32;
	int py=(320-256)/2;
	int no;
	//背景パネルの描画
	{
		no=0;
		for (int y=7;y>=0;y--) {
			for (int x=0;x<8;x++) {
				int tx = px+x*size;
				int ty = py+y*size;
				if (ClearData[no]) {
					Fill(tx,ty,size,size,14);
				} else
				if (SelectOK[no]) {
					Fill(tx,ty,size,size,1);
				} else {
					Fill(tx,ty,size,size,12);
				}
				Box(tx,ty,size,size,15);
				no ++;
			}
		}
	}
	//数字の描画
	{
		no=0;
		for (int y=7;y>=0;y--) {
			for (int x=0;x<8;x++) {
				int tx = px+x*size;
				int ty = py+y*size;
				if (SelectOK[no]) {
					if (gStage == no) {
						Color(12);
					} else {
						Color(15);
					}
					Offset(tx+(size-16*2)/2+2,ty+(size-16)/2);
					Print("%2d",no+1);
				}
				no ++;
			}
		}
	}
	//選択ステージ
	{
		no=0;
		for (int y=7;y>=0;y--) {
			for (int x=0;x<8;x++) {
				int tx = px+x*size;
				int ty = py+y*size;
				if (gStage == no) {
					Box(tx,ty,size,size,12);
				}
				no ++;
			}
		}
	}

	Color(15);
	if (Locale() == QBGAME_LOCALE_JAPANESE) {
		Offset(480-320+(320-10*16)/2,320-16-8);
		Print("せんたくしてください");
	} else {
		Offset(480-320+(320-11*16)/2,320-16-8);
		Print("SELECT ROOM");
	}

//	Box(160,0,320,320,15);
}

void QBBlueBall::Setting()
{
	abButtonVisible = true;
	
	curMenu = 0;
	switch (subStep) {
	case 0:
		menu[curMenu].Init();
		break;
	}
	menu[curMenu].offsetx = 8;
	menu[curMenu].x = -1;
	menu[curMenu].y = 0;
	menu[curMenu].w = 12;
	menu[curMenu].h = 10;
	menu[curMenu].fillColor = -1;
	menu[curMenu].frameColor = -1;
	menu[curMenu].string[0] = "START";
	menu[curMenu].string[1] = "OPTION";
	menu[curMenu].string[2] = "RESET";
	menu[curMenu].string[3] = "EXIT";
	menu[curMenu].string[4] = NULL;
	menu[curMenu].enable = false;
	menu[curMenu].cur = 1;
	menu[curMenu].draw(this);
	
	DrawStageSelect();

	curMenu = 1;
	switch (subStep) {
	case 0:
		menu[curMenu].Init();
		subStep ++;
		break;
	}
	menu[curMenu].x = 8;
	menu[curMenu].y = 4;
	menu[curMenu].w = 14;
	menu[curMenu].h = 12;
	menu[curMenu].string[0] = "EXIT";
	menu[curMenu].string[1] = "BGM ROOM";
	menu[curMenu].string[2] = "BGM ENDING";
	menu[curMenu].string[3] = "BGM RESET";
	menu[curMenu].draw(this);

	if (KeyWait(PAD_UP)) {
		menu[curMenu].up();
	} else
	if (KeyWait(PAD_DOWN)) {
		menu[curMenu].down();
	}
	if (KeyWait(PAD_RIGHT) || KeyWait(PAD_LEFT)) {
		if (menu[curMenu].cur > 0 && menu[curMenu].cur < 3) {
			PlayBGM(menu[curMenu].cur);
			menu[curMenu].play();
		}
	}
	if (KeyWait(PAD_A,true) || menu[curMenu].select) {
		switch (menu[curMenu].cur) {
		case 0:
			//menu[curMenu].cur = 2;
			menu[curMenu].play();
			GoStep(STEP_TITLE);
			StopBGM();
			abButtonVisible = false;
			break;
		case 1:
			menu[curMenu].cur = 2;
			SelectBGM(1);
			menu[curMenu].play();
			break;
		case 2:
			menu[curMenu].cur = 0;
			SelectBGM(2);
			menu[curMenu].play();
			break;
		case 3:
			ResetBGM();
			StopBGM();
			//menu[curMenu].cur = 2;
			menu[curMenu].playDispose();
			GoStep(STEP_TITLE);
			abButtonVisible = false;
			break;
		}
	}

//	Box(0,0,480,320,15);
}

void QBBlueBall::RestartGame()
{
	curMenu = 0;
	switch (subStep) {
	case 0:
		menu[curMenu].Init();
		break;
	}
	menu[curMenu].offsetx = 8;
	menu[curMenu].x = -1;
	menu[curMenu].y = 0;
	menu[curMenu].w = 12;
	menu[curMenu].h = 10;
	menu[curMenu].fillColor = -1;
	menu[curMenu].frameColor = -1;
	menu[curMenu].string[0] = "START";
	menu[curMenu].string[1] = "OPTION";
	menu[curMenu].string[2] = "RESET";
	menu[curMenu].string[3] = "EXIT";
	menu[curMenu].string[4] = NULL;
	menu[curMenu].enable = false;
	menu[curMenu].draw(this);

	DrawStageSelect();

	Fill(40,80,400,48,0);
	Box(40,80,400,48,15);
	
	Offset(40+16,80+16);
	Color(15);
	if (Locale() == QBGAME_LOCALE_JAPANESE) {
		Print("ゲームをリセットしますか？");
	} else {
		Print("Do you want to reset ?");
	}

	curMenu = 1;
	switch (subStep) {
	case 0:
		menu[curMenu].Init();
		subStep ++;
		break;
	}
	menu[curMenu].x = (30-8)/2;
	menu[curMenu].y = 10;
	menu[curMenu].w = 8;
	menu[curMenu].h = 6;
	if (Locale() == QBGAME_LOCALE_JAPANESE) {
		menu[curMenu].string[0] = "いいえ";
		menu[curMenu].string[1] = "はい";
	} else {
		menu[curMenu].string[0] = "NO";
		menu[curMenu].string[1] = "YES";
	}
	menu[curMenu].string[2] = NULL;
	menu[curMenu].draw(this);

	if (KeyWait(PAD_UP)) {
		menu[curMenu].up();
	} else
	if (KeyWait(PAD_DOWN)) {
		menu[curMenu].down();
	}
	if (KeyWait(PAD_A,true) || menu[curMenu].select) {
		if (menu[curMenu].cur == 0) {
			menu[curMenu].play();
			GoStep(STEP_TITLE);
		} else
		if (menu[curMenu].cur == 1) {
			for (int i=0;i<64;i++) {
				SelectOK[i] = 0;
				ClearData[i] = 0;
			}
			for (int i=0;i<8;i++) {
				SelectOK[i] = 1;
			}
			gStage = 0;
			menu[curMenu].play();
			GoStep(STEP_TITLE);
		}
	}

//	Box(0,0,480,320,15);
}

void QBBlueBall::SelectStage()
{
	curMenu = 0;
	switch (subStep) {
	case 0:
		//menu[curMenu].Init();
		menu[curMenu].enable = true;
		subStep ++;
		break;
	}
	menu[curMenu].offsetx = 8;
	menu[curMenu].x = -1;
	menu[curMenu].y = 0;
	menu[curMenu].w = 12;
	menu[curMenu].h = 10;
	menu[curMenu].fillColor = -1;
	menu[curMenu].frameColor = -1;
	menu[curMenu].string[0] = "START";
	menu[curMenu].string[1] = "OPTION";
	menu[curMenu].string[2] = "RESET";
	menu[curMenu].string[3] = "EXIT";
	menu[curMenu].string[4] = NULL;
	menu[curMenu].draw(this);
	
	DrawStageSelect();
	//Box(0,0,480,320,15);

	{
		int s = gStage;
		if (KeyWait(PAD_UP)) {
			menu[curMenu].play();
			s += 8;
			if (s >= 64) s -= 8;
		} else
		if (KeyWait(PAD_DOWN)) {
			menu[curMenu].play();
			s -= 8;
			if (s < 0) s += 8;
		}
		if (KeyWait(PAD_LEFT)) {
			menu[curMenu].play();
			s -= 1;
			if (s < 0) s = 0;
		} else
		if (KeyWait(PAD_RIGHT)) {
			menu[curMenu].play();
			s += 1;
			if (s >= 64) s = 63;
		}
		if (s >= 0) {
			if (SelectOK[s]) {
				gStage = s;
			}
		}
	}

	if (KeyWait(PAD_A,true) || menu[curMenu].select) {
		if (menu[curMenu].cur == 0) {
			menu[curMenu].play();
			GoStep(STEP_MAINLOOP);
		} else
		if (menu[curMenu].cur == 2) {
			menu[curMenu].play();
			GoStep(STEP_RESTART);
		} else
		if (menu[curMenu].cur == 1) {
			menu[curMenu].play();
			GoStep(STEP_SETTING);
		} else {
			//exitGame = true;
			Exit();
		}
	}

	//ステージの選択
	if (TouchCount() > 0 && touchChange != TouchCount()) {
		int size=32;
		int px=480-256-32;
		int py=(320-256)/2;
		for (int i=0;i<TouchCount();i++) {
			int no=0;
			int hx,hy;
			hx = TouchXPosition(i);
			hy = TouchYPosition(i);
			for (int y=7;y>=0;y--) {
				for (int x=0;x<8;x++) {
					if (SelectOK[no]) {
						int tx = px+x*size;
						int ty = py+y*size;
						if (hx >= tx && hx < tx+size) {
							if (hy >= ty && hy < ty+size) {
								menu[curMenu].play();
								if (gStage == no) {
									GoStep(STEP_MAINLOOP);
								} else {
									gStage = no;
								}
							}
						}
					}
					no ++;
				}
			}
		}
	}
	
	touchChange = TouchCount();
}

/*
void QBBlueBall::Title()
{
	Color(13);
	Locate(2,4);
	Print("BLUE BALL PUZZLE");
	Color(15);
	curMenu = 0;
	menu[curMenu].x = 9;
	menu[curMenu].y = 7;
	menu[curMenu].w = 12;
	menu[curMenu].h = 12;
	menu[curMenu].string[0] = "START";
	menu[curMenu].string[1] = "CONTINUE";
	menu[curMenu].string[2] = "OPTION";
	menu[curMenu].string[3] = "EXIT";
	menu[curMenu].draw(this);
	if (KeyWait(PAD_UP)) {
		menu[curMenu].up();
	} else
	if (KeyWait(PAD_DOWN)) {
		menu[curMenu].down();
	}
	if (KeyWait(PAD_A) || menu[curMenu].select) {
		if (menu[curMenu].cur == 0) {
			Play("t255M0o4l64v32CDEFGAB>C<");
			step = STEP_MAINLOOP;
		} else
		if (menu[curMenu].cur == 1) {
			Play("t255M0o4l64v32CDEFGAB>C<");
			step = STEP_MAINLOOP;
		} else
		if (menu[curMenu].cur == 2) {
			menu[curMenu].cur = 0;
			menu[curMenu].play();
			step = STEP_SETTING;
		} else {
			exitGame = true;
		}
	}
}
*/

void QBBlueBall::ResetStage(void)
{
	int x,y;
	for (y=0;y<S_H_SIZE;y++) {
		for (x=0;x<S_W_SIZE;x++) {
			bg[x][y] = stage[x+y*S_W_SIZE];
		}
	}
	parts = 0;
}

void QBBlueBall::StageGet(void)
{
	int x,y;
	for (y=0;y<S_H_SIZE-1;y++) {
		for (x=0;x<S_W_SIZE;x++) {
			bg[x][y] = StageData[gStage*S_W_SIZE*(S_H_SIZE-1)+x+y*S_W_SIZE];
		}
	}
	parts = 0;
	clearflag = 0;

	int max = sizeof(StageData)/(S_W_SIZE*(S_H_SIZE-1));
printf("max %d(%d)\n",max,(S_W_SIZE*(S_H_SIZE-1))/16);
}

void QBBlueBall::PutChar(short x,short y,int pat)
{
	Put(x,y,CharData[pat]);
}

void QBBlueBall::DrawLogo()
{
	int px = 480-320+(320-16*16)/2;
	Offset(px+2,8+2);
	Color(9);
	Print("PICO PICO PUZZLE");
	Offset(px,8);
	Color(11);
	Print("PICO PICO PUZZLE");
}

void QBBlueBall::DrawStage()
{
	int ox,oy;
	int x,y;
	int size = 20;
//	PixMapHandle CPixH;
//	Rect rect1,rect2;

	ox = 160-2;
	oy = 40;

	for (y=0;y<S_H_SIZE-1;y++) {
		for (x=0;x<S_W_SIZE;x++) {
			unsigned char pat;
			pat = bg[x][y]&0x1f;
			Put(ox+x*size,oy+y*size,CharData[pat&0x7f]);
		}
	}
	for (y=0;y<S_H_SIZE-1;y++) {
		for (x=0;x<S_W_SIZE;x++) {
			unsigned char pat;
			pat = bg[x][y];
			if ((pat&0x7f)==C_BLOCK || (pat&0x40) != 0 || (pat&0x7f)==C_BLUE_BLOCK || (pat&0x20) != 0) {
				Put(ox+x*size,oy+y*size,CharData[7]);
				if ((pat&0x40) != 0) {
					pat = C_RED_BLOCK;
				} else if ((pat&0x20) != 0) {
					pat = C_YELLOW_BLOCK;
				}
				Put(ox+x*size,oy+y*size,CharData[pat&0x7f]);
			}
		}
	}
	{
		int i;
		for (i=0;i<SPMAX;i++) {
			SP_Data[i].char_id = -1;
		}
		i = 0;
		for (y=0;y<S_H_SIZE-1;y++) {
			for (x=0;x<S_W_SIZE;x++) {
				unsigned char pat;
				pat = bg[x][y];
				if (pat & 0x80) {
					SP_Data[i].char_id = 4;
					SP_Data[i].x = ox+x*size;
					SP_Data[i].y = oy+y*size;
					i++;
				}
			}
		}
	}
	{
		int i;
		for (i=0;i<SPMAX;i++) {
			unsigned char pat;
			pat = bg[x][y];
			if (SP_Data[i].char_id >= 0) {
				Put(SP_Data[i].x,SP_Data[i].y,CharData[SP_Data[i].char_id]);
				if (SP_Data[i].char_id == C_BALL) {
					if (parts > 0) {
						Put(SP_Data[i].x-8,SP_Data[i].y,CharData[C_SWORD]);
					}
				}
			}
		}
	}
	
	Box(ox-2,oy-2,324,244,15);
	
	{
		int px = 480-320+(320-7*16)/2;
		Color(15);
		Offset(px,320-16-8-4);
		Print("ROOM %d",gStage+1);
	}
	
	DrawLogo();
}

void QBBlueBall::MoveBlock(short x,short y,short dx,short dy)
{
	//移動できるか調べる
	if (((bg[x][y] & 0x7f) == C_PLATE) ||
		((bg[x][y] & 0x7f) == C_HIBI_1) ||
		((bg[x][y] & 0x7f) == C_HIBI_2) ||
		((bg[x][y] & 0x7f) == C_HOLE) ||
		((bg[x][y] & 0x7f) == C_YELLOW_BLOCK) ||
		((dx == 1 && dy == 0) && (bg[x][y] & 0x7f) == C_RIGHT) ||
		((dx == -1 && dy == 0) && (bg[x][y] & 0x7f) == C_LEFT) ||
		((dx == 0 && dy == -1) && (bg[x][y] & 0x7f) == C_UP) ||
		((dx == 0 && dy == 1) && (bg[x][y] & 0x7f) == C_DOWN)) {
		//レッドブロック又はイエローブロックなら
		if ((bg[x+dx][y+dy]&0x40) != 0 || (bg[x+dx][y+dy]&0x20) != 0) {
			int xx;
			int yy;
			int flag;
			int space;
			flag = 1;
			space = 0;
			xx = x+dx;
			yy = y+dy;
			while (1) {
				if (bg[xx][yy] == C_PLATE ||
				    bg[xx][yy] == C_HOLE ||
				    bg[xx][yy] == C_HIBI_1 ||
				    bg[xx][yy] == C_HIBI_2 ||
				    bg[xx][yy] == C_UP ||
				    bg[xx][yy] == C_DOWN ||
				    bg[xx][yy] == C_RIGHT ||
				    bg[xx][yy] == C_LEFT ||
				    bg[xx][yy] == C_BALL_HOLE ||
				    bg[xx][yy] == C_YELLOW_BLOCK ||
				    bg[xx][yy] == C_PARTS) {
					break;
				} else if ((bg[xx-dx][yy-dy]&0x20) != 0 && bg[xx][yy] == C_SPACE) {
					space = 1;
					break;
				} else if ((bg[xx][yy]&0x40) == 0 && (bg[xx][yy]&0x20) == 0) {
					flag = 0;
					break;
				}
				xx += dx;
				yy += dy;
				if (xx < 0 || xx >= S_W_SIZE || yy < 0 || yy >= S_H_SIZE-1)
					break;
			}
			if (flag == 1 && xx >= 0 && xx < S_W_SIZE && yy >= 0 && yy < S_H_SIZE-1) {
				short xxx;
				short yyy;
				if (space == 1) {
//					eff(SD_YELWBLOCK);
					bg[xx][yy] = C_PLATE;
				}
				xxx = xx - dx;
				yyy = yy - dy;
				if (space == 1) {
					bg[xxx][yyy] &= (unsigned char)0x9f;
				}
				while (1) {
					bg[xxx+dx][yyy+dy] &= (unsigned char)0x9f;
					bg[xxx+dx][yyy+dy] |= (bg[xxx][yyy]&(unsigned char)0x60);
					xxx -= dx;
					yyy -= dy;
					if (xxx == x && yyy == y)
						break;
				}
				bg[x+dx][y+dy] &= 0x9f;
			}
		}
		if ((bg[x+dx][y+dy] & 0x80) == 0 &&
			(bg[x+dx][y+dy] & 0x40) == 0 &&	//not red block
			bg[x+dx][y+dy] != C_BLUE_BLOCK &&
			(bg[x+dx][y+dy] & 0x20) == 0 &&	//not yellow block
//			bg[x+dx][y+dy] != C_YELLOW_BLOCK &&
			bg[x+dx][y+dy] != C_BLOCK &&
			(((bg[x][y] & 0x7f) != C_RIGHT &&
			  (bg[x][y] & 0x7f) != C_LEFT &&
			  (bg[x][y] & 0x7f) != C_UP &&
			  (bg[x][y] & 0x7f) != C_DOWN
			 ) ||
			 (dx == 1 && dy == 0) && (bg[x][y] & 0x7f) == C_RIGHT ||
			 (dx == -1 && dy == 0) && (bg[x][y] & 0x7f) == C_LEFT ||
			 (dx == 0 && dy == -1) && (bg[x][y] & 0x7f) == C_UP ||
			 (dx == 0 && dy == 1) && (bg[x][y] & 0x7f) == C_DOWN)
			) {
			if ((bg[x][y] & 0x7f) == C_HIBI_1) {
				bg[x][y] = C_HIBI_2;
			} else if ((bg[x][y] & 0x7f) == C_HIBI_2) {
				bg[x][y] = C_SPACE;
			} else {
				bg[x][y] &= 0x7f;
			}
			if ((bg[x+dx][y+dy] & 0x7f) == C_BALL_HOLE) {
				menu[curMenu].playDispose();
//				eff(SD_FALL);
//				bg[x+dx][y+dy] = C_PLATE;
			} else if ((bg[x+dx][y+dy] & 0x7f) == C_PARTS) {
				Play("t255l64M0o6v32dcr");
//				menu[curMenu].playDispose();
//				eff(SD_PARTS);
				temp_parts ++;
				bg[x+dx][y+dy] = 0x80;
			} else if (bg[x+dx][y+dy] != C_SPACE) {
				bg[x+dx][y+dy] |= 0x80;
			} else {
				menu[curMenu].playDispose();
//				eff(SD_FALL);
			}
		} else if ((bg[x+dx][y+dy] & 0x7f) == C_BLUE_BLOCK && parts > 0) {
//			menu[curMenu].playDispose();
			Play("T255O6M5v32L32O7bO5bO7gO5gO7eO5eO7cO5cM0");
//			eff(SD_BLUEBLOCK);
			temp_parts_flag = 1;
			bg[x][y] &= 0x7f;
			bg[x+dx][y+dy] = 0x80;
		}
	}
}

void QBBlueBall::Ending()
{
	int py = (320+16)/2-32-32-32;
	Offset((480-15*16)/2,py);
	Color(12);
	Print("CONGRATULATION!");

	py += 32+32;
	Offset((480-24*16)/2+16,py);
	Color(14);
	Print("YOU CLEARED ALL ROOMS !!");

	py += 32;
	Offset((480-6*16)/2,py);
	Color(14);
	Print("GREAT!");

	py += 32+32;
	Offset((480-12*16)/2,py);
	Color(15);
	Print("Presented by");

	py += 32;
	Color(12);
	Offset((480-8*16)/2,py);
	Print("YAMAGAME");
	
	if (TouchCount() > 0 && touchChange != TouchCount()) {
		StopBGM();
		GoStep(STEP_TITLE_INIT);
	}

	touchChange = TouchCount();
}

int QBBlueBall::Step()
{
	return step;
}

int QBBlueBall::SetStep(int istep)
{
	step = istep;
	return 0;
}

int QBBlueBall::NextStep()
{
	SetStep(Step()+1);
	return 0;
}

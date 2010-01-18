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

#include "QBTShadow.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "font.h"
#include "key.h"
#define _PG_GLOBAL_
#include "tshadowChar.h"

#include "QBRect.h"

static PPGameTextureInfo texList[] = {
	{"tshadowChar.png",tshadowChar_group,false},
	{"font.png",font_group,false},
	{"key.png",key_group,false},
//	{"text.png",NULL,true},
	{NULL,NULL,false}
};

#define QBSOUND_TAG (-255)
#define QBIGNORE_TAG (-254)

#if !defined(ABS)
    #define ABS(A)	({ __typeof__(A) __a = (A); __a < 0 ? -__a : __a; })
#endif

static const char* _messageDataJ[] = {
	"_A4_E1_B1<_K2>_I1_D0<_K2>",					//0:おにぎりだ
	"_B1_E4_H0_D0<_K2>",							//1:きのやだ
	"_H0_B2_C4_A2_D0<_K2>",							//2:やくそうだ
	"_D4<_K2>_B2_B3_C1_C4_A2_D0<_K2>",				//3:どくけしそうだ
	"_G3_B2<_K2>_C2_I1_D0<_K2>",					//4:めぐすりだ
	"_B1_J3_A2_B0_E4_D2_I2_B1<_K2>_D0<_K2>",		//5:きょうかのつるぎだ
	"_B1_J3_A2_B0_E4_D0_D3_D0<_K2>",				//6:きょうかのたてだ
	"_D3<_K2>_B2<_K2>_D1_E4_B0_B1<_K2>_D0<_K2>",	//7:でぐちのかぎだ
	"_D4<_K2>_B2_E4_B4_A2_B3<_K2>_B1_D0<_K2>",		//8:どくのこうげきだ
	"_F0<_K2>_B2_F0_D2_C1_D0",						//9:ばくはつした
	
	"_C1_D0_E4_B0_A1_F3_A4_I1_D0",					//10:したのかいへおりた
	"_F0<_K2>_B2_F0_D2_C1_D0",						//11:バクハツ
	"_J0_E0_D0<_K2>_K4_D0<_K2>_G3_K3_C1<_K2>",		//12:わなだ、だめーじ
	"_D4<_K2>_B2_E4_J0_E0_D0<_K2>",					//13:どくのわなだ
	"_H1_B0_B0<_K2>_A2_B4<_K2>_A1_D0",				//14:ゆかがうごいた
//	"_A4_D4_C1_A0_E0_D0<_K2>",						//14:おとしあなだ
	"_A4_E0_B0_B0<_K2>_C2_A1_D0",					//15:おなかがすいた
	"_A4_A4_B0<_K2>_D0_C1<_K2>_I0_A1_D0<_K2>",		//16:おおがたじらいだ
	"_A4_E1_B1<_K2>_I1_B0<_K2>_B1_A3_D0",			//17:おにぎりがきえた
	"_J0_K3_F2<_K1>",								//18:わーぷ
};

static const char* _messageDataE[] = {
	"RICE BALL",									//0:おにぎりだ
	"WOODEN ARROW",									//1:きのやだ
	"LIFE LEAF",									//2:やくそうだ
	"POISON CURE",									//3:どくけしそうだ
	"EYE LOTION",									//4:めぐすりだ
	"STRENGTH UP",									//5:きょうかのつるぎだ
	"DEFENSE UP",									//6:きょうかのたてだ
	"EXIT KEY",										//7:でぐちのかぎだ
	"POISON ATTACK",								//8:どくのこうげきだ
	"BANG!",										//9:ばくはつした
	
	"GO DOWN STAIR",								//10:したのかいへおりた
	"BANG!",										//11:バクハツ
	"TRAP DAMAGE!",									//12:わなだ、だめーじ
	"TRAP POISON!",									//13:どくのわなだ
	"TRAP FLYING!",									//14:おとしあなだ
	"TRAP HUNGRY!",									//15:おなかがすいた
	"TRAP BIG BOMB!",								//16:おおがたじらいだ
	"TRAP LOST FOOD",								//17:おにぎりがきえた
	"WARP!",										//18:わーぷ
};

static unsigned char colorTable[16][4] = {
	{0x00,0x00,0x00,0xff},
	{0x00,0x00,0xa0,0xff},
	{0x00,0xa0,0x00,0xff},
	{0x00,0xa0,0xa0,0xff},
	{0xa0,0x00,0x00,0xff},
	{0xa0,0x00,0xa0,0xff},
	{0xa0,0xa0,0x00,0xff},
	{0xa0,0xa0,0xa0,0xff},
	{0x40,0x40,0x40,0xff},
	{0x00,0x00,0xff,0xff},
	{0x00,0xff,0x00,0xff},
	{0x00,0xff,0xff,0xff},
	{0xff,0x00,0x00,0xff},
	{0xff,0x00,0xff,0xff},
	{0xff,0xff,0x00,0xff},
	{0xff,0xff,0xff,0xff}
};

enum {
STEP_INITPOWERON,
STEP_GAMESTART,
STEP_MAPCREATE,
STEP_TITLE,
STEP_ENDING,
STEP_GAMEOVER,
STEP_MAINLOOP,
STEP_DIRECCHANGE,
STEP_SETTING
};

static const char* sdgetS = "T255L32v32O3BGAD";
static const char* sddamS = "T255L32v32O4CB";
static const char* sdhitS = "T255L32v32O4CD#";
static const char* sdnextS = "T255L32v32O2CC#DD#DC#CB";
static const char* sdprevS = "T255L32v32O2D#DC#CCD#DC#C";
static const char* sddeadS = "T255L32v32O2CF#<C>F#C>C";
static const char* sduseS = "T255L32v32O2D#DC#CCD#DC#C";
static const char* sdwanaS = "T255L32v32O2BA#AG#GF#FED#DC#C";
static const char* sdatS = "T255L32v32O5CG#E<C>G#<ECG#>CG#CG#CG#CG#";
static const char* sdopenS = "T255L32v32O3CDEFG<CEGP24O6B<D#>B<D#>B<D#>B<D#>";
//static const char* musicS = "T255v32O3L64C2CCG2GGF#2F#F#F2FFE2EEO4C2CCO3B2BBA#2A#A#";

QBTShadow::QBTShadow()
{
	texId = -1;
	stS = NULL;
	poly_que = 0;
	for (int i=0;i<100;i++) {
		poly_count[i] = 0;
		key_wait[i] = 0;
	}
	frameskip = 0;
	preSelectItem = -1;
	selectItem = -1;
	_printOffsetX = 0;
	_printOffsetY = 0;
	_fx = 0;
	_fy = 0;
	menu[curMenu].x = 0;
	menu[curMenu].y = 0;
	step = 0;
	//oldKey = 0;
	InitBGM(1,"tshadowNormal");
	InitBGM(2,"tshadowDragon");
	InitBGM(3,"tshadowShadow");
	InitBGM(4,"tshadowEnding");
}

QBTShadow::~QBTShadow()
{
}

PPGameTextureInfo* QBTShadow::TextureInfo()
{
	return texList;
}

void QBTShadow::Put(int x,int y,int pat,int type,int scale)
{
	int i=poly_que-1;
	if (i<0) i=0;
	if (poly_count[i] < MAX_POLY) {
		poly_buff[i][poly_count[i]].group = type;
		poly_buff[i][poly_count[i]].x = x*2;
		poly_buff[i][poly_count[i]].y = y*2;
		poly_buff[i][poly_count[i]].pat = pat;
		poly_buff[i][poly_count[i]].color = 15;
		poly_buff[i][poly_count[i]].w = scale;//_fscalex*scale;
		poly_buff[i][poly_count[i]].h = 1;//_fscaley*scale;
		poly_count[i] ++;
	}
}

void QBTShadow::LINE(int x1,int y1,int x2,int y2,int color)
{
	int i=poly_que-1;
	if (i<0) i=0;
	if (poly_count[i] < MAX_POLY) {
		poly_buff[i][poly_count[i]].group = QBLINE_TAG;
		poly_buff[i][poly_count[i]].x = x1;
		poly_buff[i][poly_count[i]].y = y1;
		poly_buff[i][poly_count[i]].w = x2;
		poly_buff[i][poly_count[i]].h = y2;
		poly_buff[i][poly_count[i]].color = color;
		poly_count[i] ++;
	}
}

void QBTShadow::FILL(int x1,int y1,int x2,int y2,int color)
{
	int i=poly_que-1;
	if (i<0) i=0;
	if (poly_count[i] < MAX_POLY) {
		poly_buff[i][poly_count[i]].group = QBFILL_TAG;
		poly_buff[i][poly_count[i]].x = x1;
		poly_buff[i][poly_count[i]].y = y1;
		poly_buff[i][poly_count[i]].w = x2;
		poly_buff[i][poly_count[i]].h = y2;
		poly_buff[i][poly_count[i]].color = color;
		poly_count[i] ++;
	}
}

void QBTShadow::BOX(int x1,int y1,int x2,int y2,int color)
{
	LINE(x1,y1,x2,y1,color);
	LINE(x2,y1,x2,y2,color);
	LINE(x2,y2,x1,y2,color);
	LINE(x1,y1,x1,y2,color);
}

void QBTShadow::Fill(int x1,int y1,int x2,int y2,int color)
{
	FILL(x1,y1,x1+x2,y1+y2,color);
}

void QBTShadow::Box(int x1,int y1,int x2,int y2,int color)
{
	FILL(x1,y1,x1+x2,y1+2,color);
	FILL(x1+x2-2,y1,x1+x2,y1+y2,color);
	FILL(x1,y1+y2-2,x1+x2,y1+y2,color);
	FILL(x1,y1,x1+2,y1+y2,color);
}

void QBTShadow::CLEAR()
{
	poly_count[poly_que] = 0;
	key_wait[poly_que] = 0;
}

bool QBTShadow::CKeyWait()
{
	if (ckey ==' ') return true;
	return false;
}

bool QBTShadow::KeyWaitWithArrowKey()
{
	if (ckey == ' ') return true;
	if (ckey == 75) return true;
	if (ckey == 80) return true;
	if (ckey == 77) return true;
	if (ckey == 72) return true;
	return false;
}

int QBTShadow::PLAY(const char* mml)
{
	if (poly_st+1 < poly_que) {
		int i=poly_que-1;
		if (i<0) i=0;
		if (poly_count[i] < MAX_POLY) {
			poly_buff[i][poly_count[i]].group = QBSOUND_TAG;
			poly_buff[i][poly_count[i]].pat = (unsigned long)mml;
			poly_count[i] ++;
		}
	} else {
		QBGame::Play(mml);
	}
	return 0;
}

int QBTShadow::STOP()
{
	if (poly_st+1 < poly_que) {
		int i=poly_que-1;
		if (i<0) i=0;
		if (poly_count[i] < MAX_POLY) {
			poly_buff[i][poly_count[i]].group = QBSOUND_TAG;
			poly_buff[i][poly_count[i]].pat = NULL;//(unsigned long)mml;
			poly_count[i] ++;
		}
	} else {
		QBGame::Stop();
	}
	return 0;
}

/*
unsigned long QBTShadow::TouchIdle(bool touch,int x,int y)
{
	int r;
	char key=0;
	switch (step) {
	case STEP_INITPOWERON:
	case STEP_GAMEOVER:
	case STEP_ENDING:
	case STEP_TITLE:
		key = (touch && retouch == false)?' ':0;
		break;
	case STEP_MAINLOOP:
		if (touch && retouch == false) {
			int cx = 320/2;
			int cy = 480/2-180;
			double r = atan2(y-cy,x-cx);
			if (r < 0) r+=2*M_PI;
			int qr = r*360/(2*M_PI)/45;
			switch (qr) {
			case 0:
			case 7:
				key = 75;	//Right
				break;
			case 1:
			case 2:
				key = 80;	//Up
				break;
			case 3:
			case 4:
				key = 77;	//Left
				break;
			case 5:
			case 6:
				key = 72;	//Down
				break;
			}
		}
		break;
	}
	r = Idle(key);
	retouch = touch;
	return r;
}
*/

bool QBTShadow::touchCheckWithPoly(float* keyPos,float dx,float dy)
{
	bool touch = false;
	for (int i=0;i<TouchCount();i++) {
		QBRect r;
		int x,y;
		x = TouchXPosition(i);
		y = TouchYPosition(i);
		if (r.tri_hitcheck(keyPos,x-dx,y-dy)) {
			touch = true;
		}
	}
	return touch;
}

int QBTShadow::Idle()
{
	selectItem = -1;
	directonChange = 0;
	{
		for (int i=0;i<=4;i++) {
			float pos[8][2] = {
				{0,0},
				{32,0},
				{32,32},
				{0,0},

				{0,0},
				{32,32},
				{0,32},
				{0,0},
			};
			if (touchCheckWithPoly(&pos[0][0],32+(i+3)*32+16,48)) {
				selectItem = i;
			}
			if (touchCheckWithPoly(&pos[4][0],32+(i+3)*32+16,48)) {
				selectItem = i;
			}
		}
	}
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
					directonChange = PAD_UP;
					truckStep = 2;
				} else 
				if (truckTouchY - TouchYPosition(0) < -64) {
					directonChange = PAD_DOWN;
					truckStep = 2;
				}
				if (truckTouchX - TouchXPosition(0) > 64) {
					directonChange = PAD_LEFT;
					truckStep = 2;
				}
				if (truckTouchX - TouchXPosition(0) < -64) {
					directonChange = PAD_RIGHT;
					truckStep = 2;
				}
				break;
			}
		} else {
			truckStep = 0;
		}
	}
	int r = IdleCore();
	preSelectItem = selectItem;
	return r;
}

int QBTShadow::IdleCore()
{
	int r=0;
	ckey = 0;
	RepeatKey();
/*
	if (GetKey() == oldKey && GetKey() != 0) {
		frameTimer ++;
		if (frameTimer > 20) {
			frameTimer = 15;
			oldKey = 0;
		}
	}
	if (GetKey() == 0) {
		frameTimer = 0;
	}
*/
//	if (GetKey() != oldKey) 
	{
		if (KeyWait(PAD_UP)) {
			ckey = 72;
		}
		if (KeyWait(PAD_DOWN)) {
			ckey = 80;
		}
		if (KeyWait(PAD_LEFT)) {
			ckey = 75;
		}
		if (KeyWait(PAD_RIGHT)) {
			ckey = 77;
		}
		if (KeyWait(PAD_A)) {
			ckey = ' ';
		}
		if (KeyWait(PAD_B)) {
			ckey = ' ';
		}
	}
	if (poly_st+1 < poly_que) {
		int i=poly_st;
		int count=0;
		for (int j=0;j<poly_count[i];j++) {
			if (poly_buff[i][j].group == QBSOUND_TAG) {
				char* s = (char*)poly_buff[i][j].pat;
//printf("%s\n",s);
				if (s) {
					QBGame::Play(s);
				} else {
					QBGame::Stop();
				}
				poly_buff[i][j].group = QBIGNORE_TAG;
			} else
			if (poly_buff[i][j].group == QBIGNORE_TAG) {
			} else {
#if 1
				unsigned char r,g,b;
				GetRGBColor(poly_buff[i][j].color,&r,&g,&b);
				poly[count].color(r,g,b);
				poly[count].alpha = 255;
				switch (poly_buff[i][j].group) {
				case QBLINE_TAG:
					poly[count].line(
						poly_buff[i][j].x, poly_buff[i][j].y,
						poly_buff[i][j].w, poly_buff[i][j].h);
					break;
				case QBFILL_TAG:
					poly[count].fill(
						poly_buff[i][j].x, poly_buff[i][j].y,
						poly_buff[i][j].w, poly_buff[i][j].h);
					break;
				default:
					poly[count].splite(
						poly_buff[i][j].x, poly_buff[i][j].y,
						poly_buff[i][j].pat,0,poly_buff[i][j].group,poly_buff[i][j].w);
					break;
				}
#else
				poly[count].type = PG_TSHADOW;//poly_buff[i][j].group;
				poly[count].group =  poly_buff[i][j].group;//QBCHAR_TSHADOW_TSHADOWCHAR;
				poly[count].color(255, 255, 255);
				poly[count].alpha = 255;
				//poly[count].color = poly_buff[i][j].color;
				poly[count].x = poly_buff[i][j].x;
				poly[count].y = poly_buff[i][j].y;
				poly[count].w = poly_buff[i][j].w;
				poly[count].h = poly_buff[i][j].h;
				poly[count].pat = poly_buff[i][j].pat;
#endif
				count ++;
			}
		}
		polyCount = count;//poly_count[i];
		if (key_wait[i]==0 || KeyWaitWithArrowKey()) {
			key_wait[i] = 0;
			frameskip ++;
			if (frameskip > 8) {
				frameskip = 0;
				poly_st ++;
				if (poly_st >= poly_que) {
					poly_st = 0;
					poly_que = 0;
					goto IDLE;
				}
			}
		}
	} else {
IDLE:
		poly_st = 0;
		poly_que = 0;
		if (key_wait[poly_st]==0 || KeyWaitWithArrowKey()) {
			Clear();
			key_wait[poly_st]=0;
			switch (Step()) {
			case STEP_INITPOWERON:
				InitPowerOn();
				break;
			case STEP_TITLE:
				Title();
				//r = GameSelect(key);
				break;
			case STEP_GAMESTART:
				GameStart();
				break;
			case STEP_MAPCREATE:
				MapCreate();
				break;
			case STEP_DIRECCHANGE:
				DirecChange();
				break;
			case STEP_GAMEOVER:
				GameOver();
				break;
			case STEP_ENDING:
				Ending();
				break;
			case STEP_MAINLOOP:
	//			if (frameTimer == 0) {
					MainLoop();
	//			}
	//			if (frameTimer > 0) frameTimer --;
				break;
			case STEP_SETTING:
				Setting();
				break;
			}
		}
		{
			int i=poly_st;
			int count=0;
			for (int j=0;j<poly_count[i];j++) {
				if (poly_buff[i][j].group == QBSOUND_TAG) {
					char* s = (char*)poly_buff[i][j].pat;
					if (s) {
						QBGame::Play(s);
					} else {
						QBGame::Stop();
					}
					poly_buff[i][j].group = QBIGNORE_TAG;
				} else
				if (poly_buff[i][j].group == QBIGNORE_TAG) {
				} else {
#if 1
					unsigned char r,g,b;
					GetRGBColor(poly_buff[i][j].color,&r,&g,&b);
					poly[count].color(r,g,b);
					poly[count].alpha = 255;
					switch (poly_buff[i][j].group) {
					case QBLINE_TAG:
						poly[count].line(
							poly_buff[i][j].x, poly_buff[i][j].y,
							poly_buff[i][j].w, poly_buff[i][j].h);
						break;
					case QBFILL_TAG:
						poly[count].fill(
							poly_buff[i][j].x, poly_buff[i][j].y,
							poly_buff[i][j].w, poly_buff[i][j].h);
						break;
					default:
						poly[count].splite(
							poly_buff[i][j].x, poly_buff[i][j].y,
							poly_buff[i][j].pat,0,poly_buff[i][j].group,poly_buff[i][j].w);
						break;
					}
#else
					poly[count].type = PG_TSHADOW;
					poly[count].group = QBCHAR_TSHADOW_TSHADOWCHAR;
					poly[count].color(255, 255, 255);
					poly[count].alpha = 255;
					//poly[count].color = poly_buff[i][j].color;
					poly[count].x = poly_buff[i][j].x;
					poly[count].y = poly_buff[i][j].y;
					poly[count].w = poly_buff[i][j].w;
					poly[count].h = poly_buff[i][j].h;
					poly[count].pat = poly_buff[i][j].pat;
#endif
					count ++;
				}
			}
			polyCount = count;
			if (poly_st+1 >= poly_que) {
				poly_st = 0;
				poly_que = 0;
			}
		}
	}
//	prekey = _key;
//printf("%d,%d\n",poly_st,poly_que);
	//oldKey = GetKey();
	return r;
}

int QBTShadow::PSET(int x,int y,int color)
{
	x -= 176;
	y -= 16;
	if (x >= 0 && y >= 0 && x < mapw && y < maph ) {
		_pixel[x*4+0+y*256*4] = colorTable[color][0];
		_pixel[x*4+1+y*256*4] = colorTable[color][1];
		_pixel[x*4+2+y*256*4] = colorTable[color][2];
		_pixel[x*4+3+y*256*4] = colorTable[color][3];
	}
	return 0;
}

/*
static short map[1][9] = {
    {    0,    0,    0,   mapw,   maph,    0,    0,    0,    0},
};
*/

/*
static PPGameImage map_group[]={
    {map[0],mapw,maph}
};
*/

/*
void QBTShadow::DrawSpecial(GLGameSplite* g)
{
	if (step == STEP_MAINLOOP || step == STEP_DIRECCHANGE || step == STEP_GAMEOVER) {
		g->DeleteTexture(texId);
		texId = g->RegistTexture(_pixel,(int)256,(int)256,(int)256*4,true);
		g->BindTexture(texId);
		g->patternMaster = map_group;
		g->MoveTo(176*2+16,16*2+16);
		g->Scale(2,2);
		g->Draw(texId,0,0);
		g->Scale(1,1);
	}
}
*/

void QBTShadow::CLS()
{
	memset(_pixel,0,256*256*4);
/*
	for (int y=0;y<128;y++) {
		for (int x=0;x<128;x++) {
			_pixel[x*4+0+y*256*4] = 0xff;
			_pixel[x*4+1+y*256*4] = 0xff;
			_pixel[x*4+2+y*256*4] = 0xff;
			_pixel[x*4+3+y*256*4] = 0xff;
		}
	}
*/
}

//###############################################################  Item List Data
static int _bgdata[] = {5,7,8,9,10,27,28};
//#################################################################  Monster Data
static int _monsterdata[][4] = {
	{1,0,3,1}, 
	{3,1,15,1}, 
	{2,2,20,2}, 
	{15,3,32,4}, 
	{18,5,50,16},
	{20,5,140,20}, 
	{20,10,200,40}, 
	{45,20,500,0}
};
static int _monsterFloorData[][3] = {
	{0,0,0}, {0,0,1}, {0,1,1}, {1,1,2}, {1,2,2}, {2,2,3}, {2,3,3}, {3,3,3},
	{2,3,4}, {3,3,4}, {5,5,5}, {4,4,5}, {4,5,6}, {5,6,6}, {6,6,6}, {6,6,6}
};

#if 0
//##############################################################  Ending BGM Data
//endingbgm:
DATA "T164MBv32O2L16C2CC","G2GG","F#2F#F#","F2FF"
DATA "E2EE", "O3C2CC", "O2B2BB", "A#2A#A#"
DATA "LOOPPOINT"
#endif

void QBTShadow::InitPowerOn()
{
	for (int i=0;i<=6;i++) {
		bgdata[i] = _bgdata[i];
	}
	for (int i=0;i<=7;i++) {
		eds[i] = _monsterdata[i][0];
		edd[i] = _monsterdata[i][1];
		edl[i] = _monsterdata[i][2];
		ede[i] = _monsterdata[i][3];
	}
	for (int i=0;i<=15;i++) {
		for (int j=0;j<=2;j++) {
			edata[i][j] = _monsterFloorData[i][j];
		}
	}
	int l = Locale();
	for (int i=0;i<=18;i++) {
		if (l == QBGAME_LOCALE_JAPANESE) {
			its[i] = _messageDataJ[i];
		} else {
			its[i] = _messageDataE[i];
		}
	}
	SetStep(STEP_TITLE);
}

void QBTShadow::GameStart()
{
#if 0
	floorNum = 15; 
	plife = 900; 
	pmax = 900; 
	pstr = 50; 
	pdef = 20; 
#else
	floorNum = 0; 
	plife = 10; 
	pmax = 10; 
	pstr = 1; 
	pdef = 0; 
#endif
	pfood = 100; 
	pfmax = 100;
	pkey = 0; psel = 0; pct = 0; pdoku = 0; pexp = 0; pnexp = 5;
	for (int i=0;i<=4;i++) {
		itm[i] = 0;
	}
	SetStep(STEP_MAPCREATE);
	endingStep = 0;
	endingCounter = 0;
//	step = STEP_ENDING;
	preSelectItem = -1;
	selectItem = -1;
	//	endingStep = 0;
	//	endingCounter = 0;
	//	step = STEP_ENDING;
	//hiraDispFlag = false;
}

void QBTShadow::MapCreate()
{
	//int i;
	prek = 0;
	floorNum = floorNum + 1;
	if (floorNum == 17) {
		endingStep = 0;
		endingCounter = 0;
		SetStep(STEP_ENDING);
		PlayBGM(4);
		return;
	}
retry_map_create:
	for (int sy=0;sy<=maph;sy++) {
		for (int sx=0;sx<=mapw;sx++) {
			bg0[sx][sy] = 0;
			bg1[sx][sy] = 1;
		}
	}
	for (int sy=0;sy<=kmaph+1;sy++) {
		for (int sx=0;sx<=kmapw+1;sx++) {
			bg2[sx][sy] = 0;
		}
	}
	startx = QBINT(QBRnd(kmapw));
	starty = QBINT(QBRnd(kmaph));
	bg2[startx][starty] = 1;
	int r = QBINT(QBRnd(QBINT((kmapw * kmaph) / 4))) + QBINT((kmapw * kmaph) / 4) - 2;
	if (floorNum == 16) {
		if (r < 6) r = 6;
	}
	for (int i=0;i<=r/*-1*/;i++) {
		AreaInc();
	}
	//部屋のサイズを決める
	{
		int n=0;
		int spaceNum = 0;
		for (int sy=0;sy<=kmaph;sy++) {
			for (int sx=0;sx<=kmapw;sx++) {
				if (bg2[sx][sy] == 1) {
					int w = QBINT(QBRnd(15)) + 1;
					rw[sx][sy] = w;
					int h = QBINT(QBRnd(15)) + 1;
					rh[sx][sy] = h;
					n ++;
					spaceNum += w*h;
				}
			}
		}
		//小さな部屋ばかりなら一つを大きくする
		if (spaceNum < n*9) {
			int k = QBRnd(n);
			int n=0;
			for (int sy=0;sy<=kmaph;sy++) {
				for (int sx=0;sx<=kmapw;sx++) {
					if (bg2[sx][sy] == 1) {
						if (n == k) {
							rw[sx][sy] = 8;
							rh[sx][sy] = 8;
							break;
						}
						n++;
					}
				}
				if (n == k) {
					break;
				}
			}
		}
	}
	//部屋のレイアウト決める
	for (int sy=0;sy<=kmaph;sy++) {
		for (int sx=0;sx<=kmapw;sx++) {
			if (bg2[sx][sy] == 1) {
				int t = QBINT(QBRnd(2));
				int w = rw[sx][sy];
				int h = rh[sx][sy];
//				int w = INT(Rnd(15)) + 1;
//				rw[sx][sy] = w;
//				int h = INT(Rnd(15)) + 1;
//				rh[sx][sy] = h;
				int x = QBINT(QBRnd((15 - w)));
				rx[sx][sy] = x + sx * 16 + 4;
				int y = QBINT(QBRnd((15 - h)));
				ry[sx][sy] = y + sy * 16 + 4;
				//小さい部屋に一つおきブロックはなし
				if (w < 3 || h < 3 || w*h < 25) t = 0;
				//一つおきのブロック配置
				for (int ssy=y;ssy<=y + h;ssy++) {
					for (int ssx=x;ssx<=x + w;ssx++) {
						int xx = sx * 16 + ssx + 4;
						int yy = sy * 16 + ssy + 4;
						if (t == 1 && ((xx % 2) == 1 && (yy % 2) == 1)) {
							bg1[xx][yy] = 1;
						} else {
							bg1[xx][yy] = 0;
						}
					}
				}
			}
		}
	}
	//部屋をつなぐ
	for (int sy=0;sy<=kmaph;sy++) {
		for (int sx=0;sx<=kmapw;sx++) {
			if (bg2[sx][sy] == 1) {
				int x1 = rx[sx][sy] + QBINT(QBRnd(rw[sx][sy]));
				int y1 = ry[sx][sy] + QBINT(QBRnd(rh[sx][sy]));
				if (bg2[sx + 1][sy] == 1) {
					int x2 = rx[sx + 1][sy] + QBINT(QBRnd(rw[sx + 1][sy]));
					int y2 = ry[sx + 1][sy] + QBINT(QBRnd(rh[sx + 1][sy]));
					MakeRoad(x1,y1,x2,y2);
				}
				if (bg2[sx][sy + 1] == 1) {
					int x2 = rx[sx][sy + 1] + QBINT(QBRnd(rw[sx][sy + 1]));
					int y2 = ry[sx][sy + 1] + QBINT(QBRnd(rh[sx][sy + 1]));
					MakeRoad(x1,y1,x2,y2);
				}
			}
		}
	}
	CLS();
	//LINE (0, 0)-(319, 239), 1, BF
	//FILL(0,0,319,239,1);
	//LINE (176, 16)-(177 + mapw, 17 + maph), 2, B
	//BOX(176,16,177+mapw,17+maph,2);
	//RESTORE datalist1
/*
	for (int i=0;i<=7;i++) {
		int pp[] = {7,8,4,5,9,10,27,28};
		Put((15 + i * 3) * 8, 20 * 8, pp[i]);
	}
*/
	rmx = startx;
	rmy = starty;
	//Player
	{
		if (GetPosXY()) goto retry_map_create;
		px = mx = x;
		py = my = y;
		dir = 0;
		bg1[mx][my] = 23;
	}
	//Door
	{
		if (GetPosXY()) goto retry_map_create;
		doorx = x;
		doory = y;
		doorRoom = num2;
		bg0[doorx][doory] = 2;
		exitf = 0;
	}
	//Key
	if (floorNum < 16) {
		if (GetPosXY()) goto retry_map_create;
		bg0[x][y] = 4;
		keyx = x;
		keyy = y;
	}
	//Dummy Door
	if (QBRnd(10) < 2 && floorNum > 1) {
		GetPosXY();
		bg0[x][y] = 2;
		exitx = x;
		exity = y;
		exitf = 1;
		bg1[x - 1][y] = 0;
		bg1[x + 1][y] = 0;
	}
	//アイテム
	r = QBINT(QBRnd(8)) + 3;
	for (int i=0;i<=r;i++) {
		GetPosXY();
		bg0[x][y] = bgdata[QBINT(QBRnd(7))];
	}
	if (floorNum == 16) {
		GetPosXY();
		bg0[x][y] = 28;	//めぐすり
		r ++;
	}
	//トラップ
	tnum = QBINT(QBRnd(8));
	for (int i=0;i<=tnum;i++) {
		int tn = QBINT(QBRnd(7));
		if (floorNum == 16 && tn == 2) tn = 0;
		GetPosXY(1);
		tx[i] = x;
		ty[i] = y;
		if (tn == 4) {
			if (x+1 == doorx && y == doory) {
			}
			if (x-1 == doorx && y == doory) {
			}
			if (x == doorx && y+1 == doory) {
			}
			if (x == doorx && y-1 == doory) {
			}
			tn = 0;
		}
		tf[i] = tn;
	}
	//敵
	for (int i=0;i<=31;i++) {
		el[i] = 0;
	}
	kfloorNum = floorNum;
	if (floorNum >= 16) floorNum = 16;
	
	if (floorNum == 16) {
		PlayBGM(3);
	} else
	if (floorNum < 13) {
		PlayBGM(1);
	} else {
		PlayBGM(2);
	}
	
	eNum = QBINT(QBRnd(4)) + 6;
	{
		int i;
		for (i=0;i<=eNum;i++) {
			GetPosXYWithoutEnemy(i);
			ex[i] = x;
			ey[i] = y;
			ek[i] = edata[floorNum - 1][QBINT(QBRnd(3))];
			el[i] = edl[ek[i]];
		}
		floorNum = kfloorNum;
		if (floorNum == 16) {
			if (GetPosXYWithoutEnemy(i)) goto retry_map_create;
			ex[i] = x;
			ey[i] = y;
			ek[i] = 7;
			el[i] = edl[ek[i]];
			eNum = eNum + 1;
			pkey = 0;
		}
	}
	//針トラップ
	nnum = QBINT(QBRnd(8));
	for (int i=0;i<=nnum;i++) {
		GetPosXY();
		nx[i] = x;
		ny[i] = y;
		bg0[x][y] = 11 + (QBINT(QBRnd(10)) < 5);
	}
	el[5] = -23;
	megusuri = 0;
	EnemyDisp();
	WaitVSync();
	DispScr();
	SetStep(STEP_MAINLOOP);
	
	{
		for (int sy=0;sy<=kmaph;sy++) {
			for (int sx=0;sx<=kmapw;sx++) {
			}
		}
	}
}

void QBTShadow::MainLoop()
{
mainloop:
if (plife == 0) {
	WaitVSync();
	DispScr();
	PLAY("T255O3L4cfcf#cgcf#cD#ec");
	SetStep(STEP_GAMEOVER);
	StopBGM();
	return;
}
prek = k;
k = ckey;//INP(&H60)
//if (ckey != 0) printf("%d\n",ckey);
//a$ = INKEY$
pskip = 0;
//if (a$ == CHR$(27)) goto mapcreate
//ÉAÉCÉeÉÄégóp
//ÉAÉCÉeÉÄëIë
#if 1
if (preSelectItem < 0 && selectItem >= 0) {
	if (psel == selectItem) {
		UseItem(); pskip = 1; goto mainloopskip;
	}
}
if (selectItem >= 0) {
	psel = selectItem;
	WaitVSync();
	DispScr(); PLAY("T128L32O4C");
}

if (directonChange > 0) {
	dir = ((directonChange & PAD_LEFT)!=0) * 2 + ((directonChange & PAD_RIGHT)!=0) * 3 + ((directonChange & PAD_UP)!=0) * 1;
	bg1[mx][my] = 23 + dir;
}
#else
if (prek != 28 && k == 28) {UseItem(); pskip = 1; goto mainloopskip;}
if (prek != 15 && k == 15) {
	psel = (psel + 1) % 5; WaitVSync(); DispScr(); PLAY("T128L32O4C");
//printf("psel %d\n",psel);
}
//方向転換
if (prek != 42 && k == 42) {
	step = STEP_DIRECCHANGE;
	WaitVSync();
	DispScr();
	return;
}
if (prek != 54 && k == 54) {
//printf("push 54\n");
	step = STEP_DIRECCHANGE;
	WaitVSync();
	DispScr();
	return;
}
#endif
//プレイヤーの攻撃
if (prek != ' ' && k == ' ') {
//bg1[mx][my] = 23 + dir; WaitVSync(); DispScr(); floorNum = floorNum - 1;
//PLAY(sdprevS); stS = "C1D0E4B0A1F3A4I1D0!"; HiraDisp(); 
//MapCreate();
//return;	
	STOP();
	PLAY(sdatS);
	dx = 0;
	dy = 0;
	pskip = 1;
	PlayerFight();
	goto mainloopskip;
}
dx = 0;
dy = 0;
if (k == 77 || k == 75 || k == 80 || k == 72) {
	dx = (k == 77) - (k == 75);
	dy = (k == 80) - (k == 72);
	PLAY("l32o5M5v32crM0");
}
if (dx == 0 && dy == 0) {
	WaitVSync();
	DispScr();
	return;
}
//printf("k %d\n",k);
dir = (dx == -1) * 2 + (dx == 1) * 3 + (dy == -1) * 1;
mainloopskip:
bg1[mx][my] = 0;
px = mx;
py = my;
mx = mx + dx;
my = my + dy;
if (mx < 0) mx = px;
if (my < 0) my = py;
if (mx >= mapw - 9) mx = px;
if (my >= maph - 9) my = py;
if (bg1[mx][my] != 0) {mx = px; my = py;}
if (bg0[mx][my] == 3 && (mx == doorx && my == doory)) {
//	PrevFloor();
	NextFloor();
	return;
}
if (bg0[mx][my] == 3 && (mx == exitx && my == exity)) {
	PrevFloor();
	return;
}
if (bg0[mx][my] == 2 && pkey > 0) {bg0[mx][my] = 3; mx = px; my = py; pkey = pkey - 1; PLAY(sdopenS);}
if (bg0[mx][my] == 7) {PLAY(sdgetS); pstr = pstr + 1; bg0[mx][my] = 0; stS = its[5]; HiraDisp(); mx = px; my = py;}
if (bg0[mx][my] == 8) {PLAY(sdgetS); pdef = pdef + 1; bg0[mx][my] = 0; stS = its[6]; HiraDisp(); mx = px; my = py;}
if (bg0[mx][my] == 4) {PLAY(sdgetS); pkey = pkey + 1; bg0[mx][my] = 0; stS = its[7]; HiraDisp(); mx = px; my = py;}
if (bg0[mx][my] == 5) {PLAY(sdgetS); itm[0] = itm[0] + 1; bg0[mx][my] = 0; stS = its[0]; HiraDisp(); mx = px; my = py;}
if (bg0[mx][my] == 9) {PLAY(sdgetS); itm[1] = itm[1] + 1; bg0[mx][my] = 0; stS = its[1]; HiraDisp(); mx = px; my = py;}
if (bg0[mx][my] == 10) {PLAY(sdgetS); itm[2] = itm[2] + 1; bg0[mx][my] = 0; stS = its[2]; HiraDisp(); mx = px; my = py;}
if (bg0[mx][my] == 27) {PLAY(sdgetS); itm[3] = itm[3] + 1; bg0[mx][my] = 0; stS = its[3]; HiraDisp(); mx = px; my = py;}
if (bg0[mx][my] == 28) {PLAY(sdgetS); itm[4] = itm[4] + 1; bg0[mx][my] = 0; stS = its[4]; HiraDisp(); mx = px; my = py;}
Needle();
if ((mx != px || my != py) || pskip == 1) if (Trap()) return;
dam = QBINT(plife / 10);
if (dam <= 0) dam = 1;
if (bg0[mx][my] == 11 && plife > 0) {plife = plife - dam; pDamAnim();}
bg1[mx][my] = 23 + dir;
if ((mx != px || my != py) || pskip == 1) pct = pct + 1;
if (pct > 5) {
	pct = 0;
	if (pfood <= 0) {
		if (plife > 0) plife = plife - 1;
	} else {
		if (pdoku == 1) {
			if (plife > 0) plife = plife - 1;
		} else {
			plife = plife + 1;
			if (plife >= pmax) plife = pmax;
		}
		pfood = pfood - 1;
	}
}
px = mx;
py = my;
EnemyMove();
WaitVSync();
DispScr();
//goto mainloop
}

void QBTShadow::Scale(int x,int y)
{
//	_fscalex = x;
//	_fscaley = y;
}

void QBTShadow::PrintOffset(int dx,int dy)
{
	_printOffsetX = dx;
	_printOffsetY = dy;
}

void QBTShadow::TPrint(const char* format,...)
{
	int x,y;
	x = locatex+_fx;
	y = locatey+_fy;
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
						FontPut(x+_printOffsetX,y+_printOffsetY,str[i],2);
						//PutFont(x*8,y*8,str[i],colorTable[color][0],colorTable[color][1],colorTable[color][2]);
						//Put(x*16,y*16,str[i]);
						//Put((_fx-1)*16/resolutionValue()+_printOffsetX,(_fy-1)*16/resolutionValue()+_printOffsetY,str[i],fontTextureID(),1);
						//Splite((_fx-1)*16/resolutionValue()+_printOffsetX,(_fy-1)*16/resolutionValue()+_printOffsetY,0,str[i],fontTextureID(),1);
						//_fx +=_fscalex;
						x += 32;
					}
					free(str);
				}
			}
		}
	}
	//Locate(x,y);
	locatex = x;
	locatey = y;
}

void QBTShadow::Print(const char* format,...)
{
	int x,y;
	x = locatex+_fx;
	y = locatey+_fy;
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
						FontPut(x+_printOffsetX,y+_printOffsetY,str[i]);
						//PutFont(x*8,y*8,str[i],colorTable[color][0],colorTable[color][1],colorTable[color][2]);
						//Put(x*16,y*16,str[i]);
						//Put((_fx-1)*16/resolutionValue()+_printOffsetX,(_fy-1)*16/resolutionValue()+_printOffsetY,str[i],fontTextureID(),1);
						//Splite((_fx-1)*16/resolutionValue()+_printOffsetX,(_fy-1)*16/resolutionValue()+_printOffsetY,0,str[i],fontTextureID(),1);
						//_fx +=_fscalex;
						x += 16;
					}
					free(str);
				}
			}
		}
	}
	//Locate(x,y);
	locatex = x;
	locatey = y;
}

void QBTShadow::Title()
{
WaitVSync();
CLS();
for (int y=0;y<=15;y++) {
	for (int x=0;x<=10;x++) {
		Put (x * 16, y * 16,1);
	}
}
Scale(4,4);
Color(8);
Locate(4,10);
TPrint("SHADOW");
Color(7);
PrintOffset(-2,-2);
Locate(4,10);
TPrint("SHADOW");
PrintOffset(0,0);
Scale(1);
Color(15);

#if 0
int xx = 64;
int yy = 64;
for (int y=0;y<=7;y++) {
	for (int x=0;x<=47;x++) {
		int x1 = xx + x * 4;
		int y1 = yy + y * 4;
		if (POINT(x, y + 24 * 8) != 0) {
			LINE (x1, y1)-(x1 + 3, y1 + 3), 8, BF
			LINE (x1 - 2, y1 - 2)-(x1 + 1, y1 + 1), 7, BF
		}
	}
}
#endif

//LINE (0, 192)-(48, 200), 0, BF
Locate(1,4); Print("RANDOM DUNGEON RPG");
Locate(4,7); Print("THE TOWER OF");
//Locate(1,9); Print("SHADOW");
//Locate(7,20); Print("Copyright H.Yamaguchi 1998");
//Locate(8,22); Print("PUSH SPACE KEY TO START!");

	menu[curMenu].x = 4;
	menu[curMenu].y = 14;
	menu[curMenu].w = 12;
	menu[curMenu].h = 9;
	menu[curMenu].string[0] = "START";
	menu[curMenu].string[1] = "OPTION";
	menu[curMenu].string[2] = "EXIT";
	menu[curMenu].string[3] = NULL;
	menu[curMenu].draw(this);
	if (KeyWait(PAD_UP)) {
		menu[curMenu].up();
	}
	if (KeyWait(PAD_DOWN)) {
		menu[curMenu].down();
	}

//stS = its[14]; HiraDisp(false);

if (KeyWait(PAD_A,true) || KeyWait(PAD_B,true) || menu[curMenu].select) {
	if (menu[curMenu].cur == 0) {
		PLAY("T255L8O3CCRCRCRCGGRGRGRGF#F#RF#RF#RF#");
		SetStep(STEP_GAMESTART);
	} else
	if (menu[curMenu].cur == 1) {
		SetStep(STEP_SETTING);
	} else {
		//exitGame = true;
		Exit();
	}
}
QBRnd(1);
/*
titleloop:
dummy = Rnd(1);
prek = k;
k = INP(&H60);
a$ = INKEY$;
if (prek != 57 && k == 57) {
	PLAY("MBT128L16O3CCP16CP16CP16C GGP16GP16GP16G F#F#P16F#P16F#P16F#");
	goto gamestart;
}
goto titleloop;
*/
}

void QBTShadow::GameOver()
{
WaitVSync();
DispScr();
PrintOffset(4,0);
Locate(5,9+3); Color(4); Print("GAME OVER!");
PrintOffset();
if (KeyWait(PAD_A,true) || KeyWait(PAD_B,true)) {
	SetStep(STEP_TITLE);
}
/*
gameoverloop:
prek = k;
k = INP(&H60);
a$ = INKEY$;
if (prek != 57 && k == 57) goto title;
goto gameoverloop;
*/
}

void QBTShadow::Ending()
{
WaitVSync();
	int t=3;
//	if (!IsPlaying()) PLAY(musicS);
	endingCounter ++;
	if (endingCounter > 24) {
		endingCounter = 0;
		endingStep ++;
		if (endingStep >= t*7+1) endingStep = t*7+1;
	}
//CLS : 
Color(15);Fill(0, 0,319, 239,0);
//ON PLAY(3) GOSUB Background: PLAY ON
//RESTORE endingbgm: READ Music$: PLAY Music$
Locate(0,25-endingStep); Color(4);
_fx = 3;
if (endingStep >= 0) Print("CONGRATULATION!");
Color(15);
//FOR i = 0 TO 5: PRINT : GOSUB endingwait: NEXT
_fy += t;
_fx = 1;
Locate(0,25-endingStep);
if (endingStep >= t) Print("YOU DESTROY SHADOW.");
//FOR i = 0 TO 5: PRINT : GOSUB endingwait: NEXT
_fy += t;
_fx = 1;
Locate(0,25-endingStep);
if (endingStep >= t*2) Print("THIS STORY IS OVER.");
//FOR i = 0 TO 5: PRINT : GOSUB endingwait: NEXT
Color(2);
_fy += t;
_fx = 5;
Locate(0,25-endingStep);
if (endingStep >= t*3) Print("THANK YOU");
_fy += t;
_fx = 4;
Locate(0,25-endingStep);
if (endingStep >= t*4) Print("FOR PLAYING!");
//FOR i = 0 TO 10: PRINT : GOSUB endingwait: NEXT
Color(14);
_fy += t;
_fx = 4;
Locate(0,25-endingStep);
if (endingStep >= t*5) Print("PRESENTED BY");
//FOR i = 0 TO 2: PRINT : GOSUB endingwait: NEXT
_fy += t;
_fx = 1;
Locate(0,25-endingStep);
if (endingStep >= t*6) Print("YAMAGAME 1998,2009");
//FOR i = 0 TO 3: PRINT : GOSUB endingwait: NEXT
//_fy += t;
//_fx = 0;
//Locate(1,25-endingStep);
//if (endingStep >= t*6) Print("SEE YOU NEXT GAME !!");
//FOR i = 0 TO 5: PRINT : GOSUB endingwait: NEXT
//FOR i = 0 TO 4: GOSUB endingwait: NEXT
_fy += t;
_fx = 0;
_fy = 0;
_fx = 0;
Color(15);Locate(0,0);
//Color(15);Locate(13,_fy);
//if (endingStep >= t*7+1) PRINT("PUSH SPACE KEY");
if (endingStep >= t*7+1) {
if (KeyWait(PAD_A,true) || KeyWait(PAD_B,true)) {
	StopBGM();
	SetStep(STEP_TITLE);
}
}
//endingloop:
//prek = k: k = INP(&H60): a$ = INKEY$
//IF prek <> 57 AND k = 57 THEN PLAY OFF: PLAY "MFP16": GOTO title
//GOTO endingloop
//endingwait:
//FOR j = 0 TO 7: GOSUB waitvsync: NEXT: RETURN
//Background:
//READ Music$: IF Music$ = "LOOPPOINT" THEN RESTORE endingbgm: READ Music$
//PLAY Music$: RETURN

	return;
}

void QBTShadow::DirecChange()
{
//direcchange:
prek = k; k = ckey;// a$ = INKEY$;
dx = (k == 77)  - (k == 75);
dy = (k == 80)  - (k == 72);
if (dx == 0 && dy == 0) {
//	goto direcchange;
	if (prek != k && (k == ' '  || k == 42 || k == 54)) {
		SetStep(STEP_MAINLOOP);
	}
} else {
	dir = (dx == -1) * 2 + (dx == 1) * 3 + (dy == -1) * 1;
	bg1[mx][my] = 23 + dir;
	if (prek != k && (k == 42 || k == 54 || k == 77 || k == 75 || k == 72 || k == 80)) {
		SetStep(STEP_MAINLOOP);
	}
}
WaitVSync();
DispScr();
int px,py;
px = 16+3*16;
py = 8+3*16;
Put(px+16,py,33);
Put(px,py+16,48);
Put(px+32,py+16,50);
Put(px+16,py+32,65);
/*
direcchgloop:
prek = k; k = INP(&H60); a$ = INKEY$;
if (prek == k) goto direcchgloop;
goto mainloop;
*/
}

void QBTShadow::PrevFloor()
{
bg1[mx][my] = 23 + dir; WaitVSync(); DispScr(); floorNum = floorNum - 2;
PLAY(sdprevS); stS = its[10]; HiraDisp(false); /*WaitVSync(); DispScr();*/ MapCreate();
}

void QBTShadow::NextFloor()
{
bg1[mx][my] = 23 + dir; WaitVSync(); DispScr(); PLAY(sdnextS); MapCreate();
}

void QBTShadow::PlayerFight()
{
oldmx = mx;
oldmy = my;
mx = mx - (dir == 2) + (dir == 3);
my = my - (dir == 1) + (dir == 0);
for (j=0;j<=tnum;j++) {
	if (tx[j] == mx && ty[j] == my) {
		bg0[tx[j]][ty[j]] = 6;
	}
}
for (j=0;j<=eNum;j++) {
	if (el[j] > 0 && (ex[j] == mx && ey[j] == my)) {
		mx = oldmx;
		my = oldmy;
		dam = pstr - edd[ek[j]];
		if (dam <= 0) dam = 1;
		bg1[mx][my] = 23 + dir;
		bg1[ex[j]][ey[j]] = 100;
		WaitVSync();
		DispScr();
		bg1[ex[j]][ey[j]] = 15 + ek[j];
		WaitVSync();
		DispScr();
		el[j] = el[j] - pstr;
		if (el[j] <= 0) kEne(j); else PLAY(sdhitS);
		if ((el[j] <= dam && ek[j] == 4) && QBRnd(10) < 5) {
			PLAY(sddeadS);
			stS = its[11];	//バクハツ
			HiraDisp();
			el[j] = 0;
			bg1[ex[j]][ey[j]] = 0;
			pDamAnim();
			pct = 0;
			if (plife == 1) plife = 0;
			if (plife > 1) plife = 1;
			eex = ex[j] - 1; eey = ey[j];
			ItemBreak();
			EneDam();
			eex = ex[j] + 1; eey = ey[j];
			ItemBreak();
			EneDam();
			eex = ex[j]; eey = ey[j] - 1;
			ItemBreak();
			EneDam();
			eex = ex[j]; eey = ey[j] + 1;
			ItemBreak();
			EneDam();
		}
		goto pfightskip;
	}
}
pfightskip:
if (pexp >= pnexp) lUp();
mx = oldmx;
my = oldmy;
}

void QBTShadow::kEne(int j)
{
kene:
bg1[ex[j]][ey[j]] = 0; if (ek[j] == 7) bg0[ex[j]][ey[j]] = 4;
pexp = pexp + ede[ek[j]]; PLAY(sddeadS); return;
}

void QBTShadow::lUp()
{
pmax = pmax + 10; pnexp = pnexp * 2; pstr = pstr + 1; pdef = pdef + 1;
return;
}

void QBTShadow::EneDam()
{
	for (q=0;q<=eNum;q++) {
		if ((ex[q] == eex && ey[q] == eey) && el[q] > 0) {
			return;
		}
	}
	return;
}

void QBTShadow::ItemBreak()
{
int pat = bg0[eex][eey];
if (pat == 5 || pat == 9 || pat == 7 || pat == 8 || pat == 10 || pat == 27 || pat == 28) {
	bg0[eex][eey] = 0;
}
return;
}

void QBTShadow::pDamAnim()
{
	bg1[mx][my] = 100; WaitVSync(); DispScr();
	PLAY(sddamS);
	bg1[mx][my] = 23 + dir; WaitVSync(); DispScr();
return;
}

void QBTShadow::EnemyFight()
{
	if (estr <= 0) estr = 1;
	plife = plife - estr;
	if (plife < 0) plife = 0;
	pDamAnim();
	return;
}

void QBTShadow::EnemyDisp()
{
	for (int j=0;j<=eNum;j++) {
		if (el[j] > 0) bg1[ex[j]][ey[j]] = 15 + ek[j];
	}
	return;
}

void QBTShadow::EnemyMove()
{
for (int j=0;j<=eNum;j++) {
	if (el[j] > 0) {
		ox = ex[j]; oy = ey[j];
		ax = ABS(mx - ex[j]); ay = ABS(my - ey[j]);
		if (ek[j] == 7 && megusuri == 0) {
			dx = QBINT(QBRnd(3)) - 1; dy = QBINT(QBRnd(3)) - 1;
		} else if (ek[j] != 0 && (ax <= 4 && ay <= 4)) {
			dx = QBSGN(mx - ex[j]); dy = QBSGN(my - ey[j]);
			if (ek[j] == 3 || ek[j] == 6) {
				if ((dx == 0 || dy == 0) && (ax >= 2 || ay >= 2)) {
					EnemyShot(j);
					goto nextenemy;
				}
			}
		} else {
			dx = QBINT(QBRnd(3)) - 1; dy = QBINT(QBRnd(3)) - 1;
		}
		eex = ex[j] + dx; eey = ey[j] + dy; bg1[ex[j]][ey[j]] = 0;
		if ((dx == 0 || dy == 0) && (bg1[eex][eey] >= 23 && bg1[eex][eey] <= 26)) {
			if ((ek[j] == 4 && el[j] == 1) && QBINT(QBRnd(10)) < 5) {
				px = mx;
				py = my;
				stS = its[9];	//ばくはつした
				HiraDisp();
			}
			bg1[ex[j]][ey[j]] = 15 + ek[j]; estr = eds[ek[j]] - pdef;
			EnemyFight();
			if (pdoku == 0 && (ek[j] == 2 && QBINT(QBRnd(10)) < 3)) {
				PLAY(sdwanaS);
				pdoku = 1;
				px = mx;
				py = my;
				//DispScr();
				stS = its[8];	//どくのこうげきだ
				HiraDisp();
			}
		} else if (bg1[eex][eey] != 0 && ek[j] != 5) {
			if (QBRnd(10) < 5) {
				if (bg1[eex][oy] == 0) {
					ex[j] = eex; ey[j] = oy;
				} else if (bg1[ox][eey] == 0) {
					ex[j] = ox; ey[j] = eey;
				}
			} else {
				if (bg1[ox][eey] == 0) {
					ex[j] = ox; ey[j] = eey;
				} else if (bg1[eex][oy] == 0) {
					ex[j] = eex; ey[j] = oy;
				}
			}
		} else if (ek[j] != 5) {
			ex[j] = eex; ey[j] = eey;
		}
		bg1[ex[j]][ey[j]] = 15 + ek[j];
	}
nextenemy:;
}
return;
}

void QBTShadow::EnemyShot(int j)
{
	PLAY("T128L32O2CG#CG#CG#CG#");
	esx = ex[j]; esy = ey[j];
	for (int g=0;g<=4;g++) {
	  bg1[esx][esy] = 0; bg1[ex[j]][ey[j]] = 15 + ek[j];
	  esx = esx + dx; esy = esy + dy;
	  if (bg1[esx][esy] >= 23 && bg1[esx][esy] <= 26) {
		PLAY(sddamS);
		bg1[mx][my] = 100;
		WaitVSync();
		DispScr();
		bg1[mx][my] = 23 + dir; plife = plife - (ek[j] == 3) * 5 - (ek[j] == 6) * 10;
		if (plife < 0) plife = 0;
		WaitVSync();
		DispScr();
	  }
	  if (bg1[esx][esy] != 0) return;
	  if (ek[j] == 3) bg1[esx][esy] = 14; else bg1[esx][esy] = 13;
	  WaitVSync();
	  DispScr();
	}
	return;
}

void QBTShadow::UseItem()
{
	if (itm[psel] > 0) {
		itm[psel] = itm[psel] - 1;
		if (psel == 0) EatFood();
		if (psel == 1) ThrowYa();
		if (psel == 2) Yakusou();
		if (psel == 3) Dokukesi();
		if (psel == 4) Megusuri();
		WaitVSync();
		DispScr();
	}
	return;
}

void QBTShadow::EatFood()
{
PLAY(sduseS);
pfood = pfood + 100; if (pfood > pfmax) pfood = pfmax;
return;
}

void QBTShadow::ThrowYa()
{
PLAY(sdatS);
dx = -(dir == 2) + (dir == 3); dy = -(dir == 1) + (dir == 0);
int pat = bg1[mx][my]; yax = mx; yay = my;
for (int ii=0;ii<=3;ii++) {
	bg1[yax][yay] = 0; bg1[mx][my] = pat; yax = yax + dx; yay = yay + dy;
	if (bg1[yax][yay] >= 15 && bg1[yax][yay] <= 22) {
		for (int j=0;j<=eNum;j++) {
			if (el[j] > 0 && (ex[j] == yax && ey[j] == yay)) {
				bg1[ex[j]][ey[j]] = 100; WaitVSync(); DispScr();
				bg1[ex[j]][ey[j]] = 15 + ek[j]; WaitVSync(); DispScr();
				el[j] = el[j] - 30;
				if (el[j] <= 0) {
					PLAY(sddeadS);
					bg1[ex[j]][ey[j]] = 0;
					if (ek[j] == 7) bg0[ex[j]][ey[j]] = 4;
					pexp = pexp + ede[ek[j]];
					if (pexp >= pnexp) lUp();
				} else {
					PLAY(sdhitS);
				}
				goto throwyaskip;
			}
		}
	}
	if (bg1[yax][yay] != 0) goto throwyaskip;
	bg1[yax][yay] = 14;
	WaitVSync();
	DispScr();
}
throwyaskip:
bg1[yax][yay] = 0;
EnemyDisp();
WaitVSync();
DispScr();
dx = 0;
dy = 0;
return;
}

void QBTShadow::Yakusou()
{
PLAY(sduseS);
plife = plife + 100; if (plife > pmax) plife = pmax;
pfood = pfood + 5; if (pfood > pfmax) pfood = pfmax;
return;
}

void QBTShadow::Dokukesi()
{
PLAY(sduseS);
pdoku = 0; pfood = pfood + 5; if (pfood > pfmax) pfood = pfmax;
//pfood = pfood + 5; if (pfood > pfmax) pfood = pfmax;
return;
}

void QBTShadow::Megusuri()
{
PLAY(sduseS);
if (megusuri == 0) {
	megusuri = 1;
	for (int ii=0;ii<=tnum;ii++) {
		int ttx,tty;
		ttx = tx[ii]; tty = ty[ii]; bg0[ttx][tty] = 6;
	}
	if (exitf == 1) {
		bg0[exitx - 1][exity] = 6; bg0[exitx + 1][exity] = 6;
	}
} else {
	PLAY(sdwanaS);
	stS = its[18]; HiraDisp();	//わーぷ
	bg1[mx][my] = 0;
	PSET (176 + mx, 16 + my, 0);
	GetPosXYWithoutEnemy(eNum);
	mx = x;
	my = y;
}
return;
}

void QBTShadow::Needle()
{
	for (int ii=0;ii<=nnum;ii++) {
		sx = nx[ii];
		sy = ny[ii];
		bg0[sx][sy] = 11 + (bg0[sx][sy] == 11);
	}
	return;
}

int QBTShadow::Trap()
{
for (int ii=0;ii<=tnum;ii++) {
	int txx,tyy;
	txx = tx[ii]; tyy = ty[ii];
	if (mx == txx && my == tyy) {
		px = mx;
		py = my;
		bg0[txx][tyy] = 6; /*bg1[mx][my] = 23 + dir;*/ WaitVSync(); DispScr();
		if (tf[ii] == 0) {
			dam = QBINT(pmax / 5); if (dam <= 0) dam = 1;
			if (plife > 0) plife = plife - dam;
			if (plife < 0) plife = 0;
			pDamAnim();
			stS = its[12]; HiraDisp();		//わなだ、だめーじ
		}
		if (tf[ii] == 1 && pdoku == 0) {
			PLAY(sdwanaS);
			stS = its[13]; HiraDisp(); pdoku = 1;	//どくのわなだ
		}
		if (tf[ii] == 2) {
			PLAY(sdwanaS);
			stS = its[14]; HiraDisp();	//おとしあなだ
			WaitVSync();
			MapCreate();
			return 1;
		}
		if (tf[ii] == 3 && pfood > 0) {
			PLAY(sdwanaS);
			pfood = pfood - 50; if (pfood < 0) pfood = 0;
			stS = its[15]; HiraDisp();	//おなかがすいた
		}
		if (tf[ii] == 4) {
			PLAY(sdwanaS);
			stS = its[18]; HiraDisp();	//わーぷ
			bg1[mx][my] = 0;
			PSET (176 + mx, 16 + my, 0);
			GetPosXY();
			mx = x;
			my = y;
		}
		if (tf[ii] == 5) {
			PLAY(sdwanaS);
			stS = its[16]; HiraDisp(); plife = 1;	//おおがたじらいだ
		}
		if (tf[ii] == 6) {
			if (itm[0] > 0) {
				PLAY(sdwanaS);
				stS = its[17]; HiraDisp(); itm[0] = 0;	//おにぎりがきえた
			}
		}
		return 0;
	}
}
return 0;
}

int QBTShadow::GetPosXY(int trap)
{
	int i;
	for (i=0;i<10;i++) {
		GetRoom();
		if (trap == 1 && doorRoom == num1) {
			if (GetPos(1) == 0) return 0;
		} else {
			if (GetPos() == 0) return 0;
		}
	}
	return -1;
}

int QBTShadow::GetPosXYWithoutEnemy(int enemynum,int trap)
{
	int i;
	for (i=0;i<10;i++) {
		GetRoom();
		if (trap == 1 && doorRoom == num1) {
			if (GetPos(1) == 0) {
				bool f = true;
				for (int i=0;i<enemynum;i++) {
					if (ex[i] == x && ey[i] == y) {
						f = false;
					}
				}
				if (f) return 0;
			}
		} else {
			if (GetPos() == 0) {
				bool f = true;
				for (int i=0;i<enemynum;i++) {
					if (ex[i] == x && ey[i] == y) {
						f = false;
					}
				}
				if (f) return 0;
			}
		}
	}
	return -1;
}

void QBTShadow::GetRoom()
{
	num0 = 0;
	for (int sy=0;sy<=kmaph;sy++) {
		for (int sx=0;sx<=kmapw;sx++) {
			if (bg2[sx][sy] == 1) {
				num0 = num0 + 1;
			}
		}
	}
	num1 = QBINT(QBRnd(num0));
	num2 = num1;
	num0 = 0;
	for (int sy=0;sy<=kmaph;sy++) {
		for (int sx=0;sx<=kmapw;sx++) {
			if (bg2[sx][sy] == 1) {
				if (num0 == num1) {
					rmx = sx;
					rmy = sy;
					x = sx * 16 + 4;
					y = sy * 16 + 4;
					return;
				}
				num0 = num0 + 1;
			}
		}
	}
	return;
}

int QBTShadow::GetPos(int trap)
{
	num0 = 0;
	x = rx[rmx][rmy];
	y = ry[rmx][rmy];
	if (trap) {
		//トラップを部屋の周囲に配置しない
		for (int sy=1;sy<=rh[rmx][rmy]-1;sy++) {
			for (int sx=1;sx<=rw[rmx][rmy]-1;sx++) {
				if (bg1[sx + x][sy + y] == 0 && bg0[sx + x][sy + y] == 0) {
					num0 = num0 + 1;
				}
			}
		}
		num1 = QBINT(QBRnd(num0));
		num0 = 0;
		for (int sy=1;sy<=rh[rmx][rmy]-1;sy++) {
			for (int sx=1;sx<=rw[rmx][rmy]-1;sx++) {
				if (bg1[sx + x][sy + y] == 0 && bg0[sx + x][sy + y] == 0) {
					if (num0 == num1) {
						x = sx + x;
						y = sy + y;
						return 0;
					}
					num0 = num0 + 1;
				}
			}
		}
	} else {
		for (int sy=0;sy<=rh[rmx][rmy];sy++) {
			for (int sx=0;sx<=rw[rmx][rmy];sx++) {
				if (bg1[sx + x][sy + y] == 0 && bg0[sx + x][sy + y] == 0) {
					num0 = num0 + 1;
				}
			}
		}
		num1 = QBINT(QBRnd(num0));
		num0 = 0;
		for (int sy=0;sy<=rh[rmx][rmy];sy++) {
			for (int sx=0;sx<=rw[rmx][rmy];sx++) {
				if (bg1[sx + x][sy + y] == 0 && bg0[sx + x][sy + y] == 0) {
					if (num0 == num1) {
						x = sx + x;
						y = sy + y;
						return 0;
					}
					num0 = num0 + 1;
				}
			}
		}
	}
	return -1;
}

void QBTShadow::MapAllDisp()
{
	for (int y=0;y<=maph;y++) {
		for (int x=0;x<=mapw;x++) {
			int xx = 176 + x;
			int yy = 16 + y;
			if (bg1[x][y] == 1) PSET (xx, yy, 2); else PSET (xx, yy, 0);
		} 
	}
	return;
}

void QBTShadow::DispScr()
{
	FILL(0,0,319,239,0);
	//BOX(176,16,177+mapw,17+maph,2);
	for (int sy=0;sy<=8;sy++) {
		//a$ = INKEY$;
		for (int sx=-1;sx<=9;sx++) {
			int col;
			x = mx + sx - 4;
			y = my + sy - 4;
			int xx = sx * 16 + 8;
			int yy = sy * 16 + 40+8;
			if (x < 0 || y < 0 || x >= mapw || y >= maph) {
				Put (xx,yy,30);
			} else
			if (bg1[x][y] == 100) {
				//LINE (xx, yy)-(xx + 15, yy + 15), 15, BF
				Put (xx,yy,30);
			} else if (bg1[x][y] != 0 && bg1[x][y] < 4) {
				if (floorNum >= 16 && bg1[x][y] == 1) {
					Put (xx,yy,35);
				} else
				if (floorNum >= 13 && bg1[x][y] == 1) {
					Put (xx,yy,37);
				} else {
					Put (xx,yy,bg1[x][y]);
				}
			} else {
				Put (xx,yy,bg0[x][y]);
				if (bg1[x][y] != 0 && (megusuri == 1 || bg1[x][y] != 22)) {
					Put(xx,yy,bg1[x][y]);
				}
			}
			if (x < 0 || y < 0 || x >= mapw || y >= maph) {
			} else {
				if (bg1[x][y] == 1) col = 2; else col = 0;
				if (bg0[x][y] == 3 || bg0[x][y] == 2) col = 14;
				PSET(176 + x, 16 + y, col);
			}
		} 
	}

	Box(0,48*2,160*2,9*16*2,15);
/*
	{
		for (int y=0;y<=maph;y++) {
			for (int x=0;x<=mapw;x++) {
				int col;
				if (bg1[x][y] == 1) col = 2; else col = 0;
				if (bg0[x][y] == 3 || bg0[x][y] == 2) col = 14;
				PSET(176 + x, 16 + y, col);
			}
		}
	}
*/
	if (pdoku == 1) Color(5); else Color(15);
	if (pfood == 0) Color(2);
	if (plife <= 1) Color(4);
	PSET (176 + mx, 16 + my, 15);
#if 1
//	Locate(0, 0); Print("FLOOR     %4d", floorNum);
	Locate(6,0); Print("LIFE %4d/%4d", plife, pmax);
	Locate(6,1); Print("FOOD %4d/%4d", pfood, pfmax);
	Locate(6,2); Print("EXP. %4d/%4d", pexp, pnexp);

	Locate(0, 0); Print("FLOOR");
	Locate(0, 1); Print(" %4d", floorNum);
#else
	Locate(3,1+1); Print("FLOOR    %3d", floorNum);
	Locate(3,2+1); Print("LIFE %3d/%3d", plife, pmax);
	Locate(3,3+1); Print("FOOD %3d/%3d", pfood, pfmax);
	Locate(3,4+1); Print("EXP.%4d/%4d", pexp, pnexp);
#endif
	{
		int x,y;
		x = 2;
		y = 3;
		for (int i=0;i<=7;i++) {
			int pp[] = {7,8,4,5,9,10,27,28};
			if (i >= 3) {
				Put((i * 2+x +1) * 8, y * 8, pp[i]);
			} else {
				Put((i * 2+x -1) * 8, y * 8, pp[i]);
			}
		}
		Locate(0+x-1,2+y); Print("%2d", pstr);
		Locate(2+x-1,2+y); Print("%2d", pdef);
		Locate(4+x-1,2+y); Print("%2d", pkey);
		for (int i=0;i<=4;i++) {
			int c = 0;
			int x1 = i * 2+x;
			//int x1 = 25 + i * 3;
			int y1 = 0+y;
			Locate(x1+7,y1+2); Print("%2d", itm[i]);
			if (i == psel) c = 2; else c = 1;
			//int xx = (x1 - 1) * 8;
			//int yy = (y1 - 3) * 8;
			if (c == 2) {
				Put(16*3+8+x1*8,y1*8,29);
			}
			//LINE (xx - 2, yy - 2)-(xx + 17, yy + 25), c, B; 
		}
	}
	return;
}

void QBTShadow::MakeRoad(int x1,int y1,int x2,int y2)
{
x = x1;
y = y1;
dx = QBSGN(x2 - x1);
dy = QBSGN(y2 - y1);
if (QBRnd(10) < 5) {
makeroadloop1: 
		bg1[x][y] = 0; x = x + dx; if (x != x2) goto makeroadloop1;
makeroadloop2: 
		bg1[x][y] = 0; y = y + dy; if (y != y2) goto makeroadloop2;
} else {
makeroadloop3: 
		bg1[x][y] = 0; y = y + dy; if (y != y2) goto makeroadloop3;
makeroadloop4: 
		bg1[x][y] = 0; x = x + dx; if (x != x2) goto makeroadloop4;
}
bg1[x1][y1] = 0;
bg1[x2][y2] = 0;
return;
}

void QBTShadow::AreaInc()
{
num0 = 0;
for (int sy=0;sy<=kmaph;sy++) {
	for (int sx=0;sx<=kmapw;sx++) {
		if (bg2[sx][sy] == 1) {
			x = sx - 1; y = sy; AreaIncSub1(); x = sx; y = sy - 1; AreaIncSub1();
			x = sx + 1; y = sy; AreaIncSub1(); x = sx; y = sy + 1; AreaIncSub1();
		}
	}
}
num1 = QBINT(QBRnd(num0)); num0 = 0;
for (int y=0;y<=kmaph;y++) {
	for (int x=0;x<=kmapw;x++) {
		if (bg2[x][y] == 2) {
			if (num0 == num1) bg2[x][y] = 1; goto areaincskip1;
			num0 = num0 + 1;
		}
	}
}
areaincskip1:
for (int y=0;y<=kmaph;y++) {
	for (int x=0;x<=kmapw;x++) {
		if (bg2[x][y] == 2) bg2[x][y] = 0;
	}
}
return;
}

void QBTShadow::AreaIncSub1()
{
	if ((x < 0 || y < 0) || (x > kmapw || y > kmaph)) return;
	if (bg2[x][y] == 0) {bg2[x][y] = 2; num0 = num0 + 1;}
	return;
}

void QBTShadow::WaitVSync()
{
	poly_count[poly_que] = 0;
	poly_que ++;
}

void QBTShadow::HiraDisp(int disp)
{
int s1;
int s2;
int sx,sy,i;
int omx = mx;
int omy = my;
int pc;
sx = 32-8;
sy = 44+32;
i = 0;
Color(15);
//strloop:
if (disp) {
	WaitVSync();
	mx = px;
	my = py;
	pc = bg1[mx][my];
	bg1[mx][my] = 23 + dir;
	DispScr();
	bg1[mx][my] = pc;
	mx = omx;
	my = omy;
}
Color(15);
Fill(16*2, (32+32)*2,128*2,32*2,0);
Box(16*2, (32+32)*2,128*2,32*2,15);
for (int i=0;/*stS[i]!='!'&&*/stS[i]!=0;i++) {
	unsigned char s = (unsigned int)stS[i];
	//if (s == '!') goto strskip;
	if (s == '<') {sx = sx - 8; sy = sy - 8; continue;}
	if (s == '>') {sy = sy + 8; continue;}
	if (s == '_') {
		i++;
		s = (unsigned int)stS[i];
		if (s >= 'A' && s <= 'K') {
			s1 = s - 'A'; //continue;
			i++;
			s = (unsigned int)stS[i];
		}
		if (s >= '0' && s <= '4') {
			s2 = s - '0'; 
			if (s1 > 6) {
				const int tableS1[4][5] = {
					{ 15, 15, 15,  9, 15},
					{  8,  8,  8,  8,  8},
					{ 15, 10, 10, 10,  9},
					{  9, 10, 12,  2, 10}
				};
				const int tableS2[4][5] = {
					{  0,  1,  1,  6,  2},
					{  5,  6,  7,  8,  9},
					{  3,  5,  6,  7,  7},
					{  5,  9, 11, 13,  8}
				};
				int ss1 = tableS1[s1-7][s2]-8;
				int ss2 = tableS2[s1-7][s2];
				s1 = ss1;
				s2 = ss2;
			}
			{
				int fs = 128+s2+s1*16;
	//printf("%d,",fs);
				FontPut(sx*2,sy*2,fs);
				//PUT (sx, sy), hira%(0, s2, s1), PSET;
			}
			sx = sx + 8;
		}
	} else {
		FontPut(sx*2,sy*2,s);
		sx = sx + 8;
	}
	//i = i + 1;
}
//goto strloop;
strskip:
//printf("\n");
{
	int i = poly_que-1;
	if (i < 0) i = 0;
	key_wait[i]=1;
	//DispScr();
}
//WaitVSync();
//if (CKeyWait()) {
//	hiraDispFlag = false;
//}
/*
prek = k; k = INP(&H60); a$ = INKEY$; if (prek == k) goto strskip;
strskip2;
prek = k; k = INP(&H60); a$ = INKEY$; if (prek == k) goto strskip2;
return;
*/
}

void QBTShadow::FontPut(int x,int y,int pat,int scale)
{
	int i=poly_que-1;
	if (i<0) i=0;
	if (poly_count[i] < MAX_POLY) {
		poly_buff[i][poly_count[i]].group = QBCHAR_TSHADOW_FONT;
		poly_buff[i][poly_count[i]].x = x;
		poly_buff[i][poly_count[i]].y = y;
		poly_buff[i][poly_count[i]].pat = pat;
		poly_buff[i][poly_count[i]].color = color;
		poly_buff[i][poly_count[i]].w = scale;
		poly_buff[i][poly_count[i]].h = scale;
		poly_count[i] ++;
	}
}

void QBTShadow::PutFont(int x,int y,int pat,unsigned char r,unsigned char g,unsigned char b,unsigned char a)
{
	int i = polyCount;

	poly[i].r = r;
	poly[i].g = g;
	poly[i].b = b;
	poly[i].alpha = a;
//	poly[i].blend = GLGameBlend_Color;
	poly[i++].splite(x,y,pat,PG_FONT,QBCHAR_TSHADOW_FONT);
	
	polyCount++;
}

void QBTShadow::Setting()
{
	WaitVSync();
	CLS();
	menu[curMenu].x = 0;
	menu[curMenu].y = 0;
	menu[curMenu].w = 20;
	menu[curMenu].h = 19;
	menu[curMenu].string[0] = "EXIT";
	menu[curMenu].string[1] = "BGM FLOOR";
	menu[curMenu].string[2] = "BGM DRAGON";
	menu[curMenu].string[3] = "BGM SHADOW";
	menu[curMenu].string[4] = "BGM ENDING";
	menu[curMenu].string[5] = "BGM RESET";
	menu[curMenu].string[6] = NULL;
	menu[curMenu].draw(this);
	if (KeyWait(PAD_UP)) {
		menu[curMenu].up();
	} else
	if (KeyWait(PAD_DOWN)) {
		menu[curMenu].down();
	}
	if (KeyWait(PAD_RIGHT) || KeyWait(PAD_LEFT)) {
		if (menu[curMenu].cur > 0 && menu[curMenu].cur < 5) {
			PlayBGM(menu[curMenu].cur);
			menu[curMenu].play();
		}
	}
	if (KeyWait(PAD_A,true) || KeyWait(PAD_B,true) || menu[curMenu].select) {
		switch (menu[curMenu].cur) {
		case 0:
			menu[curMenu].cur = 1;
			menu[curMenu].play();
			SetStep(STEP_TITLE);
			StopBGM();
			break;
		case 5:
			ResetBGM();
			menu[curMenu].cur = 1;
			menu[curMenu].playDispose();
			SetStep(STEP_TITLE);
			StopBGM();
			break;
		default:
			SelectBGM(menu[curMenu].cur);
			menu[curMenu].cur ++;
			if (menu[curMenu].cur >= 5) menu[curMenu].cur = 0;
			//menu[curMenu].cur = 0;
			menu[curMenu].play();
			break;
		}
	}
}

int QBTShadow::Step()
{
	return step;
}

int QBTShadow::SetStep(int istep)
{
	step = istep;
	return 0;
}

int QBTShadow::NextStep()
{
	SetStep(Step()+1);
	return 0;
}

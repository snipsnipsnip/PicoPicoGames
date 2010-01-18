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

#include "QBPicoHero2.h"
#include "font.h"
#include "PPGameRunlength.h"
#include <stdarg.h>
#include "key.h"
#define _PG_GLOBAL_
#include "shadowChar.h"

static PPGameTextureInfo texList[] = {
	{"shadowChar.png",shadowChar_group,false},
	{"font.png",font_group,false},
	{"key.png",key_group,false},
//	{"text.png",NULL,true},
	{NULL,NULL,false}
};

enum {
	P_SLIME = (11*4+1),
	P_SHADOW_EGG = (11*4+9),
	P_FIER = (11*5+4),
	P_SHADOW = (11*5+9),
	P_DRAGON = (11*5+10),
	P_BIGENEMY = (11*5+10),
	P_ENDENEMY = (11*6+10),
	P_ROCK = (11*0+4),
	P_GRAVE = (11*0+3),
	P_CAVE = (11*7+6),
	P_STAIR = (11*7+8),
	P_PRINCESS = (11*2+9),
};

enum {
	P_PLAYER = (11*4+0)
};

enum {
	P_MOVEROCK = (11*3+5),
	P_MOVEGRAVE = (11*3+6),
	P_LACKWALL = (11*3+3),
	P_DOOR = (11*3+2),
	P_POTION = (11*2+10),
	P_SWORD_UP = (11*3+1),
	P_SHIELD_UP = (11*3+0),
	P_SHIELD = (11*2+0),
	P_SWORD = (11*2+1),
	P_CLYSTAL3 = (11*2+8),
	P_BURNTREE = (11*3+4),
};

enum {
	I_SHIELD = 0,
	I_SWORD = 1,
	I_KEY = 2,
	I_MATOCK = 3,
	I_CANDLE = 4,
	I_GROVE = 5,
	I_CRYSTAL1 = 6,
	I_CRYSTAL2 = 7,
	I_CRYSTAL3 = 8,
};

enum {
	STEP_INIT,
	STEP_STARTPROGRAM,
	STEP_GAMEOVER,
	STEP_ENDING,
	STEP_MAINLOOP,
	STEP_FIGHTPLAYER,
	STEP_FIGHTPLAYERSUB,
	STEP_FIGHTENEMY,
	STEP_SCROLL,
	STEP_DOWNSTAIR,
	STEP_SETTING
};



#if 0
static char a[][52] = {
" 11111111111111111111111111111111111111111111111111",
" 11111111111111111111111111111111111111111111111111",
" 11K     D 1 E           J11111111J111611       611",
" 11 2 2 2  1111111 D           1  H1 F4     111 111",
" 11  DJD   5         D    1      F      F   1J  111",
" 11  2 2   1111111        1111111111111111111111111",
" 11 D    D 1  E     E     17 J13C  G      333333331",
" 111111111 1     E        1   13  G  G    55  I  31",
" 116  D  D 1   D      D  J1   13333333333333 3 3 31",
" 11111111111151111111111111   1    G    G  3     31",
" 11       E   11111911        1GG3 3333333 33353331",
" 11 F  4    F 1111 4  D       16 3 3G G  3       31",
" 11   4A4     11   2      22  1333 3 3335333     31",
" 11 2  4  F2  1  F   2 D  22  1 G  3 3 3 3 3     31",
" 11  E        1  2      2 222 1 3333G3     3  H  31",
" 1111111   1111   F  2222  62 1  G   333 333 H6H 31",
" 1 4             22222222   2 1111111111 1111111111",
" 118111J    11112222B H   222                  JJ11",
" 11111111111112222222222222222222222222222222111111",
" 11111111111111111111111111111111111111111111111111"
};

static const char* mapChar = " 123456789ABCDEFGHIJK";

static int sa = 'A';
static int s0 = '0';
static int s7 = '7';
#endif

typedef struct _enemy {
	int eds,edd,edl,ede;
} enemy;

static enemy local_enemy_d[] = {
	//	攻撃		防御		ライフ		経験値
	{	1,		0,		5,			1},		//グリーンスライム
	{	3,		0,		30,			3},		//スネーク
	{	5,		0,		50,			5},		//レッドスライム
	{	7,		0,		70,			7},		//ラット
	{	9,		0,		90,			9},		//バット
	{	11,		0,		110,		11},	//オクトパス
	{	13,		2,		130,		13},	//マッシュルーム
	{	15,		4,		150,		15},	//オーク
	{	17,		6,		300,		17},	//シャドーエッグ
	{	19,		0,		190,		19},	//ゾンビ
	{	21,		0,		210,		21},	//オークL2
	{	23,		2,		230,		23},	//ウィザード
	{	25,		4,		250,		25},	//スケルトン
	{	27,		6,		270,		27},	//ドラキュラ
	{	29,		8,		290,		29},	//ファイヤー
	{	31,		0,		410,		31},	//キングオーク
	{	33,		2,		330,		33},	//スカルナイト
	{	35,		4,		350,		35},	//デビル
	{	37,		6,		370,		37},	//デス
	{	39,		8,		500,		39},	//シャドー
	{	250,	0,		15000,		0},		//ドラゴン

	{	 7,		0,		1300,		30},	//Gスネーク
	{	 16,	0,		2500,		45},	//Gオクトパス
	{	 15,	0,		4100,		60},	//Gスケルトン
};

#if 0
static int d_eds[] = {1,1,2,5,10,10};		//攻撃力
static int d_edd[] = {0,1,2,3,5,5};			//防御力
static int d_edl[] = {3,5,10,24,50,140};	//ライフ
static int d_ede[] = {1,1,2,4,16,0};		//獲得経験値
#endif

void QBPicoHero2::FontPut(int x,int y,int pat,int scale)
{
	int i = polyCount;

	poly[i].r = 255;
	poly[i].g = 255;
	poly[i].b = 0;
	poly[i].alpha = 255;
	poly[i++].splite(x,y,pat,PG_FONT,QBCHAR_FONT,2);
	
	polyCount++;
}

void QBPicoHero2::TPrint(const char* format,...)
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

QBPicoHero2::QBPicoHero2()
{
	menu[curMenu].x = 0;
	menu[curMenu].y = 0;
	fskip = 0;
	debug = 0;
	step = 0;
	//oldKey = 0;
}

QBPicoHero2::~QBPicoHero2()
{
}

PPGameTextureInfo* QBPicoHero2::TextureInfo()
{
	return texList;
}

int QBPicoHero2::KeySize()
{
	return 96*96;
}

/*
bool QBPicoHero2::KeyWait()
{
	if (key ==' ') return true;
	return false;
}
*/

/*
unsigned long QBPicoHero2::TouchIdle(bool touch,int x,int y)
{
	int r;
	char key=0;
	switch (step) {
	case STEP_STARTPROGRAM:
	case STEP_GAMEOVER:
	case STEP_ENDING:
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

void QBPicoHero2::InitBGMSetting()
{
	InitBGM(1,"shadowBusterOverWorld");
	InitBGM(2,"shadowBusterUnderWorld");
	InitBGM(3,"shadowBusterEnding");
}

int QBPicoHero2::Idle()
{
	int r=0;
	Clear();
	switch (Step()) {
	case STEP_INIT:
		InitBGMSetting();
		NextStep();
	case STEP_STARTPROGRAM:
		StartProgram();
		//r = GameSelect(key);
		break;
	case STEP_GAMEOVER:
		GameOver();
		break;
	case STEP_ENDING:
		Ending();
		break;
	case STEP_MAINLOOP:
		MainLoop();
		break;
	case STEP_FIGHTPLAYER:
		FightPlayer();
		break;
	case STEP_FIGHTPLAYERSUB:
		FightPlayerSub();
		break;
	case STEP_FIGHTENEMY:
		FightEnemySub();
		break;
	case STEP_SCROLL:
		ScrollLoop();
		break;
	case STEP_DOWNSTAIR:
		WaitDisp();
		break;
	case STEP_SETTING:
		Setting();
		break;
	}
	//oldKey = GetKey();
	return r;
}

void QBPicoHero2::Define()
{
}

void QBPicoHero2::StartProgram()
{
//startprogram: CLS : SCREEN 7: 
Color(15);

if (ShadowEnding()) {
	Fill(0, 0,320, 480,0);
	Locate(1,2);Print("The legend of");
	Locate(2,3);TPrint("PicoHERO");
	Put(2*16, 88-32,P_SHADOW);
	Put(7*16, 88-32,P_PLAYER);
} else {
	Fill(0, 0,320, 480,1);
	Locate(1,2);Print("The legend of");
	Locate(1,3);TPrint("PicoHERO2");
	Locate(3,7);Print("Help!");
	Put(2*16, 88-24,P_PRINCESS);
	Put(3*16, 88-24,P_DRAGON);
	Put(7*16, 88-24,P_PLAYER);
}

	menu[curMenu].x = 4;
	menu[curMenu].y = 11;
	menu[curMenu].w = 12;
	int n=0;
	int indexContinue=-1;
	int indexSetting=-1;
	int indexExit=-1;
	menu[curMenu].string[n++] = "START";
	if (Continue()) {
		indexContinue = n;
		menu[curMenu].string[n++] = "CONTINUE";
	}
	indexSetting = n;
	menu[curMenu].string[n++] = "OPTION";
	indexExit = n;
	menu[curMenu].string[n++] = "EXIT";
	menu[curMenu].string[n++] = NULL;
	menu[curMenu].h = 3+(n-2)*3;
	menu[curMenu].draw(this);

/*
	if (key == prekey && key != 0) {
		frameTimer ++;
		if (frameTimer > 20) {
			frameTimer = 15;
			prekey = 0;
		}
	}
	if (key == 0) {
		frameTimer = 0;
	}
*/

	if (KeyWait(PAD_UP)) {
		menu[curMenu].up();
	} else
	if (KeyWait(PAD_DOWN)) {
		menu[curMenu].down();
	}
	
	//mapWidth = 50;
	//mapHeight = 20;

if (KeyWait(PAD_A,true) || KeyWait(PAD_B,true) || menu[curMenu].select) {
	if (menu[curMenu].cur == 0) {
		SystemInit();
		InitEnemy();
		DrawItem();
		DrawMap();
	} else
	if (menu[curMenu].cur == indexContinue) {
		LoadGame();
	} else
	if (menu[curMenu].cur == indexSetting) {
		menu[curMenu].cur = 0;
		SetStep(STEP_SETTING);
	} else {
		//exitGame = true;
		Exit();
	}
}
}

void QBPicoHero2::SystemInit()
{
//RESTORE darea2
int i=0;
LoadMap(MapDataName());
//mapHeight = map[1]->height;
//mapWidth = map[1]->width;
mapHeight = 512;
mapWidth = 256;
mx = 2; my = 2; //fgt = 0;
for (int y=0;y<512;y++) {
	for (int x=0;x<256;x++) {
		bg[x][y] = 0;
	}
}
for (int y=0;y<GetMapData(0)->height&&y<256;y++) {
	for (int x=0;x<GetMapData(0)->width&&x<256;x++) {
		bg[x][y] = GetMapData(0)->map[x+y*GetMapData(0)->width];
	}
}
for (int y=0;y<GetMapData(1)->height&&y<256;y++) {
	for (int x=0;x<GetMapData(1)->width;x++) {
//		int aa = ' ';
		int k = GetMapData(1)->map[x+y*GetMapData(1)->width];
#if 1
		m[x][y] = k;
		if (k == P_PLAYER) {
			mx = x;
			my = y;
//printf("mx %d,my %d\n",mx,my);
		}
		if (k == P_CAVE || k == P_STAIR) {
			m[x][y] = 0;
			bg[x][y] = k;
		}
#else
		int c = mapChar[k];
		//int c = a[i][x + 1];
		if (c == aa) {
			m[x][y] = 0;
		} else
		if (c == 'K') {
			mx = x;
			my = y;
		} else {
			m[x][y] = c - (c < sa) * s0 - (c >= sa) * s7;
		}
		if (c == 0) {
			printf("_(%d)",k);
		} else {
			printf("%c",c);
		}
#endif
	}
//	printf("\n");
	i ++;
}
for (int y=0;y<GetMapData(2)->height&&y<256;y++) {
	for (int x=0;x<GetMapData(2)->width;x++) {
		int k = GetMapData(2)->map[x+y*GetMapData(2)->width];
		m[x][y+256] = k;
		if (k == P_CAVE || k == P_STAIR) {
			m[x][y+256] = 0;
			bg[x][y+256] = k;
		}
	}
}
for (int i=0;i<QBSHADOW_ITEM_MAX;i++) {
	it[i] = 0;
}
//pmax = 1000;
mpx = (mx / 10)*10;
mpy = (my / 9)*9;
InitPlayer();
level = 1;
continueCount = 0;
dark = false;
//LINE (0, 0)-(319, 199), 1, BF
//LINE (87, 131)-(233, 179), 2, B: LINE (88, 132)-(232, 178), 0, BF
statusx = 0;
statusy = 18*8;
itemx = (160-16*9)/2;
itemy = 22*8;
//frameTimer = 0;
SetStep(STEP_MAINLOOP);
prestep = STEP_MAINLOOP;
subcount = 8;
Play("t255l16v32o4ggagggagg");
PlayBGM(1);
}

void QBPicoHero2::LoadGame()
{
	SystemInit();
	if (SaveDataName()) {
		Load(PPGameResourcePath(SaveDataName()));
	}
	InitEnemy();
	DrawItem();
	DrawMap();
}

void QBPicoHero2::MainLoop()
{
	bool pushkeyH=false;
	bool pushkeyV=false;
	int dx,dy;
	int p,p2;
	//int omx = mx;
	//int omy = my;
	//int k = key;
	if (prestep == STEP_FIGHTPLAYER) goto SKIP_FIGHTPLAYER;
	//k = INP(&H60)
	//if (fgt == k) k = 0; else fgt = 0;

	dx = 0;
	dy = 0;

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
	if (bg[mx][my] == P_CAVE || bg[mx][my] == P_STAIR) {
		
	}
/*
	if (KeyWait(PAD_B)) {
		debug ++;
		if (debug > 2) debug = 0;
	}
*/	
	if (KeyWait(PAD_LEFT)) {
		Play("l32o5M5v32crM0");
		pushkeyH = true;
		dx = -1;
	}
	if (KeyWait(PAD_RIGHT)) {
		Play("l32o5M5v32crM0");
		pushkeyH = true;
		dx = 1;
	}
	if (KeyWait(PAD_UP)) {
		Play("l32o5M5v32crM0");
		pushkeyV = true;
		dy = -1;
	}
	if (KeyWait(PAD_DOWN)) {
		Play("l32o5M5v32crM0");
		pushkeyV = true;
		dy = 1;
	}

//	dx = (k == 77)  - (k == 75);
//	dy = (k == 80)  - (k == 72);

	m[mx][my] = 0;
	mx = mx + dx;
	my = my + dy;
	px = mx;
	py = my;
	p = m[px][py];
	p2 = bg[px][py];
	if (p != 0) {
		pushkeyH = false;
		pushkeyV = false;
		mx = mx - dx;
		my = my - dy;
	}
	if (p == P_MOVEROCK) MoveRock(dx,dy);
	if (p == P_LACKWALL) MoveWall(dx,dy);
	if (p == P_PLAYER) MoveWall(dx,dy);
	if (p == P_MOVEGRAVE) MoveGrave(dx,dy);
	if (p == P_BURNTREE) MoveTree(dx,dy);
	if (p == P_DOOR) OpenDoor();
	if (p == P_PRINCESS) {
		Play("t255l16v32o4ggggg");
		PlayBGM(3);
		SetStep(STEP_ENDING);
	}
	if (pushkeyH || pushkeyV) {
		if (p2 == P_CAVE || p2 == P_STAIR) {
			timer = 0;
			SetStep(STEP_DOWNSTAIR);
#if 0
			if (my < 256) {
				my += 256;
				mpy += 256;
			} else {
				my -= 256;
				mpy -= 256;
			}
#endif
			//mx = mx + dx;
			//my = my + dy;
		}
	}
	if (p >= P_SHIELD && p <= P_CLYSTAL3) GetItem();
	if (p == P_POTION) {
		Play("t255l16v32o5ffg");
		m[px][py] = 0;
		plife = pmax;
		//DrawItem();
	}
	if (p == P_SWORD_UP) {
		Play("t255l16v32o5ffg");
		m[px][py] = 0;
		pstr ++;
	}
	if (p == P_SHIELD_UP) {
		Play("t255l16v32o5ffg");
		m[px][py] = 0;
		pdef ++;
	}
#if 1
	if (ScrollMap()) {
		mpx = mx-5;
		mpy = my-4;
	} else {
		if (pushkeyH) {
			bool sc = false;
			tmpx = mpx;
			tmpy = mpy;
			if (mx-mpx <= -1) {
				tmpx -= 10;
				mx -= dx;
				sc = true;
			} else
			if (mx-mpx <= 0) {
				tmpx -= 10;
				sc = true;
			} else
			if (mx-mpx >= 11) {
				tmpx += 10;
				mx -= dx;
				sc = true;
			} else
			if (mx-mpx >= 10) {
				tmpx += 10;
				sc = true;
			}
			if (sc) {
				SetStep(STEP_SCROLL);
				timer = 0;
			}
		}
		if (pushkeyV) {
			bool sc = false;
			tmpx = mpx;
			tmpy = mpy;
			if (my-mpy <= -1) {
				tmpy -= 9;
				my -= dy;
				sc = true;
			} else
			if (my-mpy <= 0) {
				tmpy -= 9;
				sc = true;
			} else
			if (my-mpy >= 10) {
				tmpy += 9;
				my -= dy;
				sc = true;
			} else
			if (my-mpy >= 9) {
				tmpy += 9;
				sc = true;
			}
			if (sc) {
				SetStep(STEP_SCROLL);
				timer = 0;
			}
		}
	}
#endif
	m[mx][my] = P_PLAYER;
	if (p >= P_SLIME && p <= P_ENDENEMY) {
		//fgt = k;
		//FightPlayer();
		DrawMap();
		DrawItem();
		prestep = STEP_FIGHTPLAYER;
		SetStep(STEP_FIGHTPLAYER);
		return;
	}
SKIP_FIGHTPLAYER:
	if (plife == 0) {
		Play("t255l16v32o4ggeecc");
		StopBGM();
		SetStep(STEP_GAMEOVER);
	}
	if (ShadowEnding()) {
		if (el[boss] <= 0) {
			Play("t255l16v32o4ggggg");
			PlayBGM(2);
			SetStep(STEP_ENDING);
		}
	}
	if (fskip > 0) fskip --;
	if (EnemyMove(0) == 0) {
		if (fskip <= 0) {
			fskip = 48;
		}
	}
	DrawMap();
	DrawItem();
//	Locate(0,0);
//	Print("%d,%d",mx-mpx,my-mpy);
	prestep = STEP_MAINLOOP;
}

void QBPicoHero2::ScrollLoop()
{
	timer ++;
	if (timer > 2) {
		timer = 0;
		int dx = QBSGN(tmpx-mpx);
		int dy = QBSGN(tmpy-mpy);
		if (dx == 0 && dy == 0) SetStep(STEP_MAINLOOP);
		mpx += dx;
		mpy += dy;
	}
	DrawMap();
	DrawItem();
}

void QBPicoHero2::WaitDisp()
{
	timer ++;
	if (timer > 8) {
		m[mx][my] = 0;
		timer = 0;
		SetStep(STEP_MAINLOOP);
		if (my < 256) {
			if (it[I_CANDLE] == 0) dark = true;
			my += 256;
			mpy += 256;
			PlayBGM(2);
		} else {
			dark = false;
			my -= 256;
			mpy -= 256;
			PlayBGM(1);
		}
		Play("T255O6M5v32L8ardrarM0");
	}
	DrawMap();
	DrawItem();
}

void QBPicoHero2::Ending()
{
	//LINE (0, 0)-(319, 199), 1, BF
	if (ShadowEnding()) {
		Put((160-16)/2, 11*8, P_PLAYER);
	} else {
		Put((160-16)/2-16, 11*8, P_PLAYER);
		Put((160-16)/2+16, 11*8, P_PRINCESS);
	}
	Color(12);
	Locate(3,5);Print("CONGRATULATION!");
	if (ShadowEnding()) {
		Locate(0,17);Print("YOU DESTROY SHADOW !");
	} else {
		Locate(5,17);Print("YOU RESCUED");
		Locate(4,19);Print("THE PRINCESS!");
	}

	Color(15);
	if (ShadowEnding()) {
	} else {
		char str[1024];
		sprintf(str,"CONTINUE %d TIMES",continueCount);
		int l = strlen(str);
		Locate((20-l)/2,22);Print(str);
	}
	//Color(15);
	//Locate(6,18);Print("PRESENTED BY H.Yamaguchi 1998");
	//Locate(7,20);Print("Push Space Button to Restart");
	if (KeyWait(PAD_A,true) || KeyWait(PAD_B,true)) {
		StopBGM();
		SetStep(STEP_STARTPROGRAM);
	}
}

void QBPicoHero2::GameOver()
{
	m[mx][my] = 0;
	DrawMap();
	DrawItem();
	Color(12);
	Locate(5,10);Print("GAME OVER!");
	Color(15);
	//Locate(7,15);Print("Push Space Button to Restart");
	//gol: a$ = INKEY$: if a$ != CHR$(32) THEN GOTO gol ELSE GOTO startprogram
	if (KeyWait(PAD_A,true) || KeyWait(PAD_B,true)) {
		if (SaveDataName()) {
			Save(PPGameResourcePath(SaveDataName()));
		}
		SetStep(STEP_STARTPROGRAM);
	}
}

void QBPicoHero2::FightPlayer()
{
	for (ei=0;ei<=e_num-1;ei++) {
		if (el[ei] > 0 && (ex[ei] == px && ey[ei] == py)) {
			goto fpskip;
		}
/*
		int etype = m[ex[ei]][ey[ei]];
		if (el[ei] > 0 && etype >= P_BIGENEMY) {
			if (ex[ei]-1 == px && ey[ei]-2 == py) goto fpskip;
			if (ex[ei]   == px && ey[ei]-2 == py) goto fpskip;
			if (ex[ei]+1 == px && ey[ei]-2 == py) goto fpskip;
			if (ex[ei]-1 == px && ey[ei]-1 == py) goto fpskip;
			if (ex[ei]+1 == px && ey[ei]-1 == py) goto fpskip;
			if (ex[ei]-1 == px && ey[ei]   == py) goto fpskip;
			if (ex[ei]+1 == px && ey[ei]   == py) goto fpskip;
		}
*/
	}
	DrawMap();
	DrawItem();
	SetStep(STEP_MAINLOOP);
	return;
fpskip:
	ec = m[ex[ei]][ey[ei]];
	//シャドーを倒すにはクリスタルが必要
	if (ec == P_SHADOW && it[I_CRYSTAL1]+ it[I_CRYSTAL2] + it[I_CRYSTAL3] != 3) {
		Play("t255l16v32o5d16");
		DrawMap();
		DrawItem();
		SetStep(STEP_MAINLOOP);
		return;
	}
	//シャドーエッグを倒すにはクリスタルが必要
	if (ec == P_SHADOW_EGG && it[I_CRYSTAL1]+ it[I_CRYSTAL2] + it[I_CRYSTAL3] != 3) {
		Play("t255l16v32o5d16");
		DrawMap();
		DrawItem();
		SetStep(STEP_MAINLOOP);
		return;
	}
	//ドラゴンを倒すにはドラゴンスレイヤーが必要
	if (ec == P_DRAGON && it[I_SWORD] != 1) {
		Play("t255l16v32o5d16");
		DrawMap();
		DrawItem();
		SetStep(STEP_MAINLOOP);
		return;
	}
	dam = (pstr + pstr * (it[I_SWORD] == 1)) - edd[ec - P_SLIME];
	if (dam <= 0) {
		Play("t255l16v32o5c");
		DrawMap();
		DrawItem();
		SetStep(STEP_MAINLOOP);
		return;
	}
	if (m[ex[ei]][ey[ei]] >= P_BIGENEMY) {
		m[ex[ei]][ey[ei]] = 200;
	} else {
		m[ex[ei]][ey[ei]] = 100;
	}
	SetStep(STEP_FIGHTPLAYERSUB);
	DrawMap();
	DrawItem();
	subcount = 8;
}

void QBPicoHero2::FightPlayerSub()
{
	if (subcount > 0) subcount --;
	if (subcount <= 0) {
		Play("t255l16v32O5cDM0");
		m[ex[ei]][ey[ei]] = ec;
		el[ei] = el[ei] - dam;
		if (el[ei] <= 0) {
			if (ec == P_SHADOW_EGG) {
				ec = P_SHADOW;
				m[ex[ei]][ey[ei]] = ec;
				el[ei] = edl[ec-P_SLIME];
			} else {
				kEne(ec,ei);
				while (pexp >= pnexp) LevelUp();
			}
		}
		SetStep(STEP_MAINLOOP);
	}
	DrawMap();
	DrawItem();
}

void QBPicoHero2::kEne(int ec,int ei)
{
	Play("T255O6M5v32L32O7bO5bO7gO5gO7eO5eO7cO5cM0");
	m[ex[ei]][ey[ei]] = 0;
	pexp = pexp + ede[ec - P_SLIME];
//printf("kill enemy el[%d] = %d\n",ei,el[ei]);
}

void QBPicoHero2::FightEnemy()
{
int dam = eds[et - P_SLIME]-pdef;
if (dam < 0) dam = 0;
if (it[I_SHIELD] == 1) dam = dam / 2;
if (dam <= 0) dam = 1;
plife = plife - dam;
if (plife < 0) plife = 0;
DrawItem();
m[mx][my] = 120;
DrawMap();
subcount = 8;
if (plife > 0) {
	SetStep(STEP_FIGHTENEMY);
}
}

void QBPicoHero2::FightEnemySub()
{
	if (subcount > 0) subcount --;
	if (subcount <= 0) {
		Play("t255l16v32O2f16D16M0");
		m[mx][my] = 20;
		if (EnemyMove(ei+1) == 0) {
			SetStep(STEP_MAINLOOP);
		}
	}
	DrawMap();
	DrawItem();
}

void QBPicoHero2::InitEnemy()
{
int n = 0;
enemy* enemy_d = (enemy*)EnemyData();
for (int i=0;i<QBSHADOW_MONSTER_KIND;i++) {
#if 1
	float t = enemy_d[i].ede;
	float d = enemy_d[i].ede-5;
	t /= 10;
	if (t <= 1) t = 1;
	if (d < 0) d = 0;
	eds[i] = enemy_d[i].eds*t;
	edd[i] = enemy_d[i].edd;//-d;
	edl[i] = enemy_d[i].edl*t;
	ede[i] = enemy_d[i].ede;
#else
	eds[i] = d_eds[i];
	edd[i] = d_edd[i];
	edl[i] = d_edl[i];
	ede[i] = d_ede[i];
#endif
}
for (int x=0;x<mapWidth;x++) {
	for (int y=0;y<mapHeight;y++) {
		int o = m[x][y];
		int q = o - P_SLIME;
		if (o == P_SHADOW) boss = n;
		if (o >= P_SLIME && o <= P_ENDENEMY) {
			ex[n] = x;
			ey[n] = y;
			el[n] = edl[q];
			n = n + 1;
		}
		if (n >= QBSHADOW_MONSTER) goto Exit;
	}
}
Exit:
e_num = n;
printf("enum %d\n",e_num);
}

int QBPicoHero2::EnemyMove(int s)
{
	for (ei=s;ei<=e_num-1;ei++) {
		if (el[ei] <= 0) continue;
		int d = QBRnd(32);
		int x = ex[ei] + (d < 8) * ((d < 4) - (d >= 4));
		int y = ey[ei] + (d > 24) * ((d > 28) - (d <= 28));
		if (x-mpx <= 0 || y-mpy <= 0 
		 || x-mpx >= 10 || y-mpy >= 9) {
			continue;
		}
		et = m[ex[ei]][ey[ei]];
		m[ex[ei]][ey[ei]] = 0;
		if (m[x][y] == 0 && et != P_FIER && et != P_SHADOW_EGG && et != P_DRAGON && et < P_BIGENEMY) {
			if (fskip == 0) {
				ex[ei] = x;
				ey[ei] = y;
			}
		}
		m[ex[ei]][ey[ei]] = et;
		if ((fskip % 4) == 0) {
			if (m[x][y] == P_PLAYER) {
				FightEnemy();
				return 1;
			}
		}
	}
	return 0;
}

void QBPicoHero2::GetItem()
{
	if (it[m[px][py] - P_SHIELD] == 1) return;
	Play("t255l16v32o3dcfg");
	it[m[px][py] - P_SHIELD] = 1;
	m[px][py] = 0;
	DrawItem();
}

void QBPicoHero2::MoveRock(int dx,int dy)
{
	if (it[I_GROVE] == 1 && m[px + dx][py + dy] == 0) {
		goto mskip;
	} else {
		return;
	}
mskip:
	Play("t255l16v32o2c");
	m[px][py] = 0;
	m[px + dx][py + dy]= P_ROCK;
}

void QBPicoHero2::MoveGrave(int dx,int dy)
{
	if (it[I_GROVE] == 1 && m[px + dx][py + dy] == 0) {
		goto mskip;
	} else {
		return;
	}
mskip:
	Play("t255l16v32o2def");
	m[px][py] = 0;
	m[px + dx][py + dy]= P_GRAVE;
}

void QBPicoHero2::MoveTree(int dx,int dy)
{
	if (it[I_CANDLE] == 1) {
		goto mskip;
	} else {
		return;
	}
mskip:
	Play("t255l16v32o2def");
	m[px][py] = 0;
	//m[px + dx][py + dy]= P_GRAVE;
}

void QBPicoHero2::MoveWall(int dx,int dy)
{
	if (it[I_MATOCK] == 1) {
		goto mskip;
	} else {
		return;
	}
mskip:
	Play("t255l16v32o2def");
	m[px][py] = 0;
	//m[px + dx][py + dy]= P_GRAVE;
}

void QBPicoHero2::OpenDoor()
{
	if (it[I_KEY] == 1) {
		Play("t255l16v32o2def");
		m[px][py] = 0;
		it[I_KEY] = 0;
		DrawItem();
	}
}

void QBPicoHero2::DrawItem()
{
	int y=statusy/8;
	int x=statusx/8;

	DrawItemIcon();

	Fill(statusx*2,statusy*2,160*2,4*8*2,0);

	Color(15);

	if (plife <= (pmax / 8)+1) {
		Color(20);
	}

	Locate(x,y++);Print("LIFE      %d/%d", plife, pmax);
	Locate(x,y++);Print("STR.      %d", pstr + pstr * (it[I_SWORD] == 1));
	Locate(x,y++);Print("DEF.      %d", pdef + pdef * (it[I_SHIELD] == 1));
	if (pnexp >= 10000) {
		if (pexp >= 10000) {
			Locate(x,y++);
			Print("EXP.    %d/%d", pexp,pnexp);
		} else {
			Locate(x,y++);
			Print("EXP.     %d/%d", pexp,pnexp);
		}
	} else {
		Locate(x,y++);
		Print("EXP.      %d/%d", pexp,pnexp);
	}
	//Locate(x,y++);Print("NEXT EXP. %d", pnexp);

	Color(15);

	int c=0;
	for (int ei=0;ei<=e_num-1;ei++) {
		if (el[ei] > 0) {
			c ++;
		}
	}
	//Locate(14,y++);Print("EMENY     %d", c);
	//Box(itemx, itemy,16*7,16, 2);
}

void QBPicoHero2::DrawMap()
{
#if 0
	for (int x=0;x<=50;x++) {
		for (int y=0;y<=25;y++) {
			int xx = x * 16;
			int yy = y * 16;
			Put(xx, yy, m[x][y]);
		}
	}
#else
	Fill(0, 0,160*2, 240*2,0);
#if 1
	for (int x=0;x<=10;x++) {
		for (int y=0;y<=10;y++) {
			int tx = x + mpx;
			int ty = y + mpy;
			int xx = x * 16;
			int yy = y * 16;
			xx -= 8;
			if (!ScrollMap()) {
				yy -= 8;
			}
			if (dark) {
				if (tx >= 0 && ty >= 0 && tx < mapWidth && ty < mapHeight) {
					int p = m[tx][ty];
					if (p == P_PLAYER || p == P_FIER) {
						if (p >= 100 && p < 200) Put(xx, yy, 21);
						if (p < 100) Put(xx, yy, p);
					}
				}
			} else {
				if (tx >= 0 && ty >= 0 && tx < mapWidth && ty < mapHeight) {
					int p = m[tx][ty];
					Put(xx, yy, bg[tx][ty]);
					if (p == 0) {
					} else {
						if (p >= P_BIGENEMY && p<= P_ENDENEMY) {
							//Putx3(xx-16, yy-32, p);
						} else {
							if (p >= 100 && p < 200) Put(xx, yy, 21);
							if (p < 100) Put(xx, yy, p);
						}
					}
				} else {
					Put(xx, yy, 1);
				}
			}
		}
	}
	for (int x=0;x<=10;x++) {
		for (int y=0;y<=10;y++) {
			int tx = x + mpx;
			int ty = y + mpy;
			int xx = x * 16;
			int yy = y * 16;
			xx -= 8;
			if (!ScrollMap()) {
				yy -= 8;
			}
			if (dark) {
			} else {
				if (tx >= 0 && ty >= 0 && tx < mapWidth && ty < mapHeight) {
					int p = m[tx][ty];
					if (p == 0) {
					} else {
						if (p >= P_BIGENEMY && p<= P_ENDENEMY) {
							//if (p >= 200) Putx3(xx-16, yy-32, 21);
							//if (p < 100) Putx3(xx-16, yy-32, p);
							Putx3(xx-16, yy-32, p);
						}
						if (p >= 200) {
							Putx3(xx-16, yy-32, 21);
						}
					}
				}
			}
		}
	}
#else
	for (int x=-1;x<=5;x++) {
		for (int y=-1;y<=5;y++) {
			int tx = x + mx - 2;
			int ty = y + my - 2;
			int xx = x * 16 + 120;
			int yy = y * 16 + 16;
			if (tx >= 0 && ty >= 0 && tx < 50 && ty < 20) {
				int p = m[tx][ty];
				if (p >= 100) Put(xx, yy, 21);
				if (p < 100) Put(xx, yy, p);
			} else {
				Put(xx, yy, 1);
			}
		}
	}
#endif
#endif
	Box(0,0,160*2,9*16*2,15);
	Fill(0, itemy*2, 20*8*2,16*2, 0);
}

void QBPicoHero2::WaitVSync()
{
}

void QBPicoHero2::PutFont(int x,int y,int pat,unsigned char r,unsigned char g,unsigned char b,unsigned char a)
{
	QBGame::Put(x,y,pat,PG_FONT,QBCHAR_FONT,r,g,b,a);
/*
	int i = polyCount;

	poly[i].r = r;
	poly[i].g = g;
	poly[i].b = b;
	poly[i].alpha = 255;
	poly[i++].splite(x*2,y*2,pat,PG_FONT,QBCHAR_FONT);
	
	polyCount++;
*/
}

void QBPicoHero2::Put(int x,int y,int pat)
{
	QBGame::Put(x*2,y*2,pat,PG_SHADOWBUSTER,QBCHAR_SHADOW);
/*
	int i = polyCount;
	poly[i].r = 255;
	poly[i].g = 255;
	poly[i].b = 255;
	poly[i].alpha = 255;
	poly[i++].splite(x*2,y*2,pat,PG_SHADOWBUSTER,QBCHAR_SHADOW);
	polyCount++;
*/
}

void QBPicoHero2::Putx3(int x,int y,int pat)
{
	int i = polyCount;
	poly[i].r = 255;
	poly[i].g = 255;
	poly[i].b = 255;
	poly[i].alpha = 255;
	poly[i++].splite(x*2,y*2,pat,PG_SHADOWBUSTER,QBCHAR_SHADOW,3);
	polyCount++;
}

void QBPicoHero2::Save(const char* filepath)
{
//	for (int i=0;i<10;i++) {
	long size = sizeof(unsigned short)*256*256*2;
	void* data = PPGameRunlength_Compress(&m[0][0],&size);
	if (data) 
	{
		SetData("mapData",(unsigned char*)data,size);
		//SetData("mapData",(unsigned char*)&m[0][0],size);
		SetParam("item0",it[0]);
		SetParam("item1",it[1]);
		SetParam("item2",it[2]);
		SetParam("item3",it[3]);
		SetParam("item4",it[4]);
		SetParam("item5",it[5]);
		SetParam("item6",it[6]);
		SetParam("item7",it[7]);
		SetParam("item8",it[8]);
		SetParam("item9",it[9]);
		SetParam("pmax",pmax);
		SetParam("pstr",pstr);
		SetParam("pdef",pdef);
		SetParam("pexp",pexp);
		SetParam("pnexp",pnexp);
		SetParam("level",level);
//		SetParam("mx",mx);
//		SetParam("my",my);
		SetParam("continue",continueCount);
		free(data);
	}
//	}
}

void QBPicoHero2::Load(const char* filepath)
{
//	for (int i=0;i<10;i++) {
	int size;
	unsigned char* data = GetData("mapData",&size);
	if (data) {
		long dsize = size;
		void* mdata = PPGameRunlength_Decompress((unsigned short*)data, &dsize);
		if (mdata) 
		{
//			memcpy(&m[0][0],data,size);
			memcpy(&m[0][0],mdata,dsize);
			it[0] = GetParam("item0");
			it[1] = GetParam("item1");
			it[2] = GetParam("item2");
			it[3] = GetParam("item3");
			it[4] = GetParam("item4");
			it[5] = GetParam("item5");
			it[6] = GetParam("item6");
			it[7] = GetParam("item7");
			it[8] = GetParam("item8");
			it[9] = GetParam("item9");
			pmax = GetParam("pmax");
			pstr = GetParam("pstr");
			pdef = GetParam("pdef");
			pexp = GetParam("pexp");
			pnexp = GetParam("pnexp");
			level = GetParam("level");
//			mx = GetParam("mx");
//			my = GetParam("my");
			continueCount = GetParam("continue");
			continueCount ++;
			plife = pmax;
			free(mdata);
		}
	}
//	}
}

void QBPicoHero2::Setting()
{
	menu[curMenu].x = 0;
	menu[curMenu].y = 0;
	menu[curMenu].w = 20;
	menu[curMenu].h = 15;
	int n=0;
	menu[curMenu].string[n++] = "EXIT";
	if (ShadowEnding()) {
		menu[curMenu].string[n++] = "BGM OVERWORLD";
		menu[curMenu].string[n++] = "BGM ENDING";
	} else {
		menu[curMenu].string[n++] = "BGM OVERWORLD";
		menu[curMenu].string[n++] = "BGM UNDERWORLD";
		menu[curMenu].string[n++] = "BGM ENDING";
	}
	menu[curMenu].string[n++] = "BGM RESET";
	menu[curMenu].string[n++] = NULL;
	menu[curMenu].h = 3+(n-2)*3;
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
		case 0:
			{
				//もどる
				if (Continue()) {
					menu[curMenu].cur = 2;
				} else {
					menu[curMenu].cur = 1;
				}
				menu[curMenu].play();
				SetStep(STEP_STARTPROGRAM);
				StopBGM();
			}
			break;
		default:
			if (menu[curMenu].cur == n-2) {
				//キャンセル
				ResetBGM();
				menu[curMenu].playDispose();
				if (Continue()) {
					menu[curMenu].cur = 2;
				} else {
					menu[curMenu].cur = 1;
				}
				SetStep(STEP_STARTPROGRAM);
				StopBGM();
			} else {
				//サウンド
				SelectBGM(menu[curMenu].cur);
				menu[curMenu].cur ++;
				if (menu[curMenu].cur >= n-1) menu[curMenu].cur = 0;
				menu[curMenu].play();
			}
			break;
		}
	}
}

bool QBPicoHero2::ScrollMap()
{
	return false;
}

bool QBPicoHero2::ShadowEnding()
{
	return false;
}

const char* QBPicoHero2::MapDataName()
{
	return "QBPicoHero2.plist";
}

const char* QBPicoHero2::SaveDataName()
{
	return "ShadowBusterSave.dat";
}

void QBPicoHero2::DrawItemIcon()
{
	for (int i=0;i<QBSHADOW_ITEM_MAX;i++) {
		Put(i * 16 + itemx, itemy, (it[i] == 1) * (i + P_SHIELD));
	}
}

void QBPicoHero2::InitPlayer()
{
	pmax = 10; plife = pmax; 
	//pstr = 100; pdef = 100; 
	pstr = 1; pdef = 0; 
	pexp = 0; pnexp = 10;
}

void QBPicoHero2::LevelUp()
{
	level ++;
	float t = level;
	t /= 10;
	if (t <= 1) t = 1;
	pmax = pmax + 10;
	pstr = pstr + 1*t;
	pdef = pdef + 1*t;
	pnexp += (level-1)*25;
	Play("t255l16v32o5cceegg");
}

void* QBPicoHero2::EnemyData()
{
	return local_enemy_d;
}

bool QBPicoHero2::Continue()
{
	return true;
}

int QBPicoHero2::Step()
{
	return step;
}

int QBPicoHero2::SetStep(int istep)
{
	step = istep;
	return 0;
}

int QBPicoHero2::NextStep()
{
	SetStep(Step()+1);
	return 0;
}

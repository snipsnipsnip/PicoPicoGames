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

#include "QBCarRace.h"
#include <stdio.h>
#include <stdlib.h>
#include "font.h"
#include "key.h"
#define _PG_GLOBAL_
#include "carrace.h"

static PPGameTextureInfo texList[] = {
	{"carrace.png",carrace_group,false},
	{"font.png",font_group,false},
	{"key.png",key_group,false},
//	{"text.png",NULL,true},
	{NULL,NULL,false}
};

#define MAX_SPEED 4
#define OFFSET_X (-72)
#define OFFSET_Y (-32)

#define DISP_Y 15

enum {
	STEP_INIT,
	STEP_RESTART,
	STEP_TITLE,
	STEP_MAINLOOP,
	STEP_SETTING,
	STEP_GAMEOVER,
	STEP_CLEAR_DEMO
};

static int NumLength(int n)
{
	int r=0;
	while (n != 0) {
		r ++;
		n /= 10;
	}
	return r;
}

QBCarRace::QBCarRace()
{
	menu[curMenu].x = 0;
	menu[curMenu].y = 0;
	dispHiScore = 0;
	oldKey = 0;
	step = 0;

	mx = 0;
	my = 0;
	tm = 0;
	st = 0;
	sc = 0;
	speed = 0;
	scrct = 0;
	ac = 0;
	acTimer = 0;
	sndf = 0;
	loop = 0;

	mapdy = 0;
	area = 0;
	arealoop = 0;
	areaMax = 0;
	
	stage = 0;
	stagedisp = 0;
	
	gameoverCount = 0;
	
	timer = 0;
	lapTime = 0;
	clearTimer = 0;
	
	hiscore = 0;
	frameTimer = 0;
	
	prebkey = 0;
	
	dispHiScore = 0;

	initCar();
	InitBGM(1,"carraceBGM");
}

QBCarRace::~QBCarRace()
{
	ExitGame();
}

PPGameTextureInfo* QBCarRace::TextureInfo()
{
	return texList;
}

int QBCarRace::KeySize()
{
	if (Step() == 3) {
		return 96*96;
	}
	return 64*64;
}

/*
bool QBCarRace::KeyWait()
{
	if ((!(prekey & PAD_ANY)) && (key & PAD_ANY)) return true;
	return false;
}
*/

int QBCarRace::Idle()
{
	int r=0;
	Clear();
	QBRnd(0);
	dispHiScore = 0;
	switch (Step()) {
	case STEP_INIT:
		//QBSound_LoopStreamPlayAAC(PPGameResourcePath("Ganon.aac"),44873);
		hiscore = GetParam("hiscore",0);
		Init();
	case STEP_RESTART:
		Restart();
	case STEP_TITLE:
		dispHiScore = 1;
		Title();
//		r = GameSelect(key);
		break;
	case STEP_MAINLOOP:
		MainLoop();
		break;
	case STEP_CLEAR_DEMO:
		MainLoop();
		break;
	case STEP_GAMEOVER:
		GameOver();
		break;
	case STEP_SETTING:
		dispHiScore = 1;
		Setting();
		break;
	}
	
	oldKey = GetKey();
	
	return r;
}

/*
unsigned long QBCarRace::TouchIdle(bool touch,int x,int y)
{
	//int r;
	unsigned long key=0;
	switch (step) {
	case STEP_INIT:
		break;
	case STEP_RESTART:
		if (touch) key = PAD_SPACE;
		break;
	case STEP_TITLE:
		if (touch) key = PAD_SPACE;
		break;
	case STEP_MAINLOOP:
#if 1
#else
		if (touch) {
			if (x > 160) {
				key = PAD_RIGHT;
			} else {
				key = PAD_LEFT;
			}
		}
#endif
		break;
	case STEP_GAMEOVER:
		if (touch) key = PAD_SPACE;
		break;
	}
	//r = Idle(key);
	//retouch = touch;
	return key;
}
*/ 

void QBCarRace::Init()
{
	//LoadStage(PPGameResourcePath("CARRACE.MAP"));
	LoadMap("CarRaceMap.plist");
	loop = 0;
	lapTime = GetMapData(0)->height*(MAX_SPEED-1);
	for (int i=0;i<QBCARRACE_MAX_MARK;i++) {
		mark[i] = -1;
		markCount[i] = 0;
	}
	{
		unsigned short* d = GetMapData(0)->map;
		int n=0;
		for (int i=GetMapData(0)->height-1,j=0;i>=0;i--,j++) {
			if (d[11+i*GetMapData(0)->width] != 0) {
				mark[n] = j;
				for (int x=11;x<GetMapData(0)->width;x++) {
					if (d[x+i*GetMapData(0)->width] != 0) {
						markCount[n] ++;
					}
				}
				n ++;
			}
		}
		areaMax = n;
	}
	SetStep(STEP_RESTART);
	frameTimer = 0;
	sdtimer = 0;
}

void QBCarRace::InitScr()
{
	for (int y=0;y<=DISP_Y;y++) {
		for (int x=0;x<=10;x++) {
			bg0[x][y] = 0;
		}
	}
/*
	for (int y=0;y<=10;y++) {
		for (int x=0;x<=8;x++) {
			bg1[x][y] = -1;
		}
	}
*/
}

void QBCarRace::Restart()
{
	InitScr();
	mx = 5;
	my = 10;
	scrct = 0;
	tm = 0;
	mapdy = 0;
	speed = 20-6;

	st = 0;
	area = 1;
	arealoop = 0;
	stage = 0;
	prebkey = 0;
	
	stagedisp = 0;

	initCar();
	for (int i=0;i<=DISP_Y;i++) {
		Scroll();
	}
	//PLAY("t1l1o2fedc<fedo2fedc<fedo2fedc<fedcfr");
	SetStep(STEP_TITLE);
}

void QBCarRace::Title()
{
	rand();
	DispScr(1);
	Color(13);
	Locate(3,4);
	Print("PICO PICO RACE");
//	Print("CAR RACE PANIC");
	Color(15);
//	Locate(14,13);
//	Print("Hit any key!");
	menu[curMenu].x = 4;
	menu[curMenu].y = 9;
	menu[curMenu].w = 12;
	menu[curMenu].h = 12;
	menu[curMenu].string[0] = "START";
	menu[curMenu].string[1] = "CONTINUE";
	menu[curMenu].string[2] = "OPTION";
	menu[curMenu].string[3] = "EXIT";
	menu[curMenu].draw(this);
	clearTimer = 0;
	timer = lapTime+lapTime/4;
	if (KeyWait(PAD_UP)) {
		menu[curMenu].up();
	} else
	if (KeyWait(PAD_DOWN)) {
		menu[curMenu].down();
	}
	if (KeyWait(PAD_A,true) || KeyWait(PAD_B,true) || menu[curMenu].select) {
		if (menu[curMenu].cur == 0) {
			sc =0;
			ac =0;
			Play("t255M0o4l64v32CDEFGAB>C<");
			//Play("l2o4c");
			SetStep(STEP_MAINLOOP);
			sndf = 0;
			initCar();
			PlayBGM(1);
			stagedisp = 120;
		} else
		if (menu[curMenu].cur == 1) {
			sc =0;
			ac =0;
			Play("t255M0o4l64v32CDEFGAB>C<");
			//Play("l2o4c");
			SetStep(STEP_MAINLOOP);
			sndf = 0;
			initCar();
			LoadGame();
			PlayBGM(1);
			stagedisp = 120;
		} else
		if (menu[curMenu].cur == 2) {
			menu[curMenu].cur = 0;
			menu[curMenu].play();
			SetStep(STEP_SETTING);
			//chooseBGM = true;
		} else {
			//exitGame = true;
			Exit();
		}
	}
	acTimer = 0;
}

void QBCarRace::MainLoop()
{
	timer --;
	if (timer <= 0) timer = 0;
	//bg0[mx][my] = 0;

	if ((GetKey() & (~PAD_B)) == (oldKey & (~PAD_B)) && (GetKey() & (~PAD_B)) != 0) {
		frameTimer ++;
		if (frameTimer > 20) {
			frameTimer = 15;
			oldKey = 0;
		}
	}
	if ((GetKey() & (~PAD_B)) == 0) {
		frameTimer = 0;
	}
	
	if ((prebkey & PAD_B) && !(GetKey() & PAD_B)) {
		Stop();
	}
	if ((prebkey & PAD_UP) && !(GetKey() & PAD_UP)) {
		Stop();
	}
	prebkey = GetKey();

	if (Step() == STEP_MAINLOOP) {
		if (KeyWait(PAD_RIGHT)) {
			Play("l64M0o5v32dcr");
			mx = mx + 1;
			if (mx >= 11) mx = 0;
		}
		if (KeyWait(PAD_LEFT)) {
			Play("l64M0o5v32dcr");
			mx = mx - 1;
			if (mx < 0) mx = 10;
		}
	}
	//if (KeyWait(PAD_UP)) ac = ac + 1;
	//if (KeyWait(PAD_DOWN)) ac = ac - 1;
	if (Step() == STEP_MAINLOOP) {
#if 1
#if 0
		if (timer <= 0) {
			acTimer ++;
			if (acTimer > 2) {
				acTimer = 0;
				ac = ac - 1;
				if (ac <= 0) {
					gameoverCount = 0;
					step = STEP_GAMEOVER;
				}
			}
		} else
#endif
		if (Key(PAD_UP) || Key(PAD_B)) {
			acTimer ++;
			if (acTimer > 4) {
				acTimer = 0;
				ac = ac + 1;
			}
		} else
		if (Key(PAD_DOWN)) {
			acTimer ++;
			if (acTimer > 4) {
				acTimer = 0;
				ac = ac - 1;
			}
		} else {
			acTimer ++;
			if (acTimer > 4) {
				acTimer = 0;
				if (ac <= 8) {
					ac = ac + 1;
					if (ac > 8) ac = 8;
				} else {
					ac = ac - 1;
				}
			}
		}
#else
		acTimer ++;
		if (acTimer > 4) {
			acTimer = 0;
			if (timer <= 0) {
				ac = ac - 1;
				if (ac <= 0) {
					gameoverCount = 0;
					step = STEP_GAMEOVER;
				}
			} else
			if (key & PAD_B) {
				ac = ac + 1;
			} else {
				ac = ac - 1;
			}
		}
#endif
	}
	if (Step() == STEP_CLEAR_DEMO) {
		clearTimer ++;
		if (clearTimer > 100) {
			gameoverCount = 0;
			SetStep(STEP_GAMEOVER);
		}
	}
	if (speed-ac < MAX_SPEED) ac = speed-MAX_SPEED;
	if (ac < 0) ac = 0;
//	if (mx <  0) mx = mx + 9;
//	if (mx >= 9) mx = mx - 9;
	int t = speed-ac;
	if (t < MAX_SPEED) t = MAX_SPEED;
	scrct = scrct + 1.0;
	if (scrct >= t/2.0) {
		scrct -= t/2.0;
		if (Step() == STEP_MAINLOOP) {
			enemySetTimer --;
			if (enemySetTimer <= 0) {
				enemySetTimer = enemySetInterval;
				setCar();
				setCar(DISP_Y);
			}
			Scroll();
			sc = sc + 1;
			if (hiscore < sc) hiscore = sc;
		}
		if (Step() == STEP_CLEAR_DEMO) {
			my --;
		}
		//Play("l64o1cr");
		if (IsPlaying() == 0 || sndf == 1) {
			if (Step() == STEP_MAINLOOP) {
//				sdtimer ++;
//				if (sdtimer > 1) {
//					sdtimer = 0;

					switch (t) {
					case MAX_SPEED:
						Play("l64o4M5v32dcrM0");
						break;
					case 3:
						Play("l64o3M5v32dcrM0");
						break;
					case 2:
						Play("l32o2M5v32dcrM0");
						break;
					case 1:
					default:
						Play("l32o1M5v32dcM0");
						break;
					}
//				}
			}
			sndf = 1;
		}
	}
	mapdy = ((float)scrct) / (t/2);
	idleCar();
	DispScr();
	if (Step() == STEP_MAINLOOP) {
		if (/*bg0[mx][my] != 0 && bg0[mx][my] != 8 && */bg0[mx][my] != 0 && bg0[mx][my] < 9) {
			//Play("t255o2l32fedc<fedcfedccccr");
			Stop();
			Play("t255M5l32o2v32fedc<fedo2fedc<fedo2fedc<fedcfrM0");
			gameoverCount = 0;
			SetStep(STEP_GAMEOVER);
			StopBGM();
		}
	}
}

void QBCarRace::GameOver()
{
	DispScr();
	Color(4);
	Locate(5,10);
	Print("GAME OVER!");
	gameoverCount ++;
	if (gameoverCount > 60) {
		gameoverCount = 60;
		if (KeyWait(PAD_A,true) || KeyWait(PAD_B,true)) {
			Save(PPGameResourcePath("CarRace.dat"));
			SetStep(STEP_RESTART);
		}
	}
}

void QBCarRace::DispScr(int title)
{

	//for (int i=0;i<3;i++) 
	{
	//	int tt;
#if 0
		if (my >= 0) {
			tt = bg0[mx][my];
		}
		if (step == STEP_MAINLOOP) {
			if (/*bg0[mx][my] != 0 && bg0[mx][my] != 8 && */bg0[mx][my] != 0 && bg0[mx][my] < 9) {
				bg0[mx][my] = 9;
			} else {
				bg0[mx][my] = 8;
			}
		} else
		if (step == STEP_GAMEOVER) {
			bg0[mx][my] = 9;
		} else {
			if (my >= 0) {
				bg0[mx][my] = 8;
			}
		}
#endif
		for (int y=0;y<=DISP_Y;y++) {
			for (int x=0;x<=10;x++) {
				//if (bg0[x][y] != bg1[x][y]) {
				//	bg1[x][y] = bg0[x][y];
				Put(x * 16-8, y * 16+mapdy*16-16,bg0[x][y]);
				//Splite(x * 16 + 5 * 16+OFFSET_X*8, y * 16+OFFSET_Y*8,0,bg0[x][y],0);
				//}
			}
		}
		
		if (Step() == STEP_GAMEOVER) {
			Put(mx * 16-8, my * 16-16,9);
		} else {
			Put(mx * 16-8, my * 16-16,8);
		}
/*
	for (int i=0;i<QBCARRACE_MAX_ENEMY;i++) {
		if (enemy[i].flag > 0) {
			int x = ((int)enemy[i].x);
			int y = ((int)enemy[i].y);
			if (x >= 0 && x <= 10
			 && y >= 0 && y <= 11) {
				//Color(15);
				//Put(enemy[i].x*16-8,enemy[i].y*16-16,enemy[i].flag+4);
				Put(0,i*16,5);
				Put(0,0,5);
			}
		}
	}
*/
		dispCar();
//		Fill(0,160+32,160,16,0);
/*
		if (my >= 0) {
			bg0[mx][my] = tt;
		}
*/
		Color(4);
		Locate(5,23);
		//Print("In memory of Home Page 500 hit!");
		Color(15);
		//Locate(7,24);
		//Print("Copyright H.Yamaguchi 1998");
		if (dispHiScore) {
			Locate((20-(9+NumLength(hiscore)))/2,0);
			Print("HI-SCORE %d",hiscore);
		} else {
			if (sc >= hiscore) {
				Locate((20-(9+NumLength(sc)))/2,0);
				Print("HI-SCORE %d",sc);
			} else {
				Locate((20-(6+NumLength(sc)))/2,0);
				Print("SCORE %d",sc);
			}
		}
		if (stagedisp > 0) {
			stagedisp --;
			Color(15);
			Locate((20-(6+NumLength(stage+1)))/2,10);
			Print("STAGE %d",stage+1);
		}
		Locate(4,22);
		int t = speed-ac;
		if (t < MAX_SPEED) t = MAX_SPEED;
		//Print("AC %d",ac);
		//Print("SPEED %dkm",(20-t+1)*20);
//		Locate(0,23);
//		Print("%d %d %d",timer,lapTime,loop);
	}
	//Print("%d",QBSound_Instance()->streampoint(2));
/*
	{
		static unsigned long t=0;
		unsigned long p;
		p = QBClock();
		Locate(15,2);
		Print("%8d",(int)p-t);
		Locate(15,3);
		Print("%8d",p);
		t = p;
	}
*/
}

void QBCarRace::Scroll()
{
	tm = tm - 1;
	if (tm < 0) {
		st = st + 1;
	
		if (st >= GetMapData(0)->height) {
			stage ++;
			stagedisp = 120;
			arealoop= 0;
			area = 0;
			st = 0;
		}

		if (mark[area] == st) {
			arealoop ++;
			int areat = area-1;
			if (areat < 0) areat = areaMax-1;
			if (arealoop == markCount[areat]) {
				arealoop = 0;
				area ++;
				if (area >= areaMax) area = 0;
			} else {
				st = mark[area-1];
			}
		}
		
		for (int i=0;i<QBCARRACE_MAX_ENEMY;i++) {
			if (enemy[i].flag > 0) {
				enemy[i].y += 1;
			}
		}

	}
#if 0
	{
		int l = (st-9) / map[0]->height;
		if (l != loop) {
			timer += lapTime;
			if (l >= 4) {
				//step = STEP_CLEAR_DEMO;
			}
		}
		loop = l;
	}
#endif
	for (int y=DISP_Y-1;y>=0;y--) {
		for (int x=0;x<=10;x++) {
			//if (bg0[x][y] == 8) {
			//	bg0[x][y+1] = 0;
			//} else {
				bg0[x][y+1] = bg0[x][y];
			//}
		}
	}
#if 0
	while (1) {
		while (stage[st][1] == '#') {
			tm = atoi(&stage[st][2]);
			st = st + 1;
		}
		if (stage[st][1] == 'L') {	//ループ
			tm = 0;
			st = 0;
			continue;
		}
		if (stage[st][1] == 'U') {	//スピードアップ
			speed = speed - 1;
			if (speed < 2) speed = 2;
			st = st + 1;
			continue;
		}
		if (stage[st][1] == 'D') {	//スピードダウン
			speed = speed + 1;
			if (speed > 20) speed = 20;
			st = st + 1;
			continue;
		}
		if (stage[st][1] == '\'') {	//コメント
			st = st + 1;
			continue;
		}
		break;
	}
	for (int x = 0;x<=10;x++) {
		bg0[x][0] = stage[st][x+1] - '0';
	}
#else
	{
		unsigned short* d = GetMapData(0)->map;
		int y = GetMapData(0)->height - (st % GetMapData(0)->height)-1;
		int f=0;
		for (int x = 0;x<=10;x++) {
			switch (stage % 5) {
			case 0:
				bg0[x][0] = d[x+y*GetMapData(0)->width];
				if (bg0[x][0] == 2) bg0[x][0] = 0;
				break;
			case 1:
				bg0[x][0] = d[x+y*GetMapData(0)->width];
				if (bg0[x][0] == 0) f = 1;
				if (bg0[x][0] == 1) {
					if (f == 0) {
						bg0[x][0] = 1;
					} else {
						bg0[x][0] = 3;
					}
				}
				break;
			case 2:
				bg0[x][0] = d[x+y*GetMapData(0)->width];
				if (bg0[x][0] == 1) bg0[x][0] = 3;
				break;
			case 3:
				bg0[x][0] = d[x+y*GetMapData(0)->width];
				if (bg0[x][0] == 0) f = 1;
				if (bg0[x][0] == 1) {
					if (f == 0) {
						bg0[x][0] = 4;
					} else {
						bg0[x][0] = 3;
					}
				}
				break;
			default:
				bg0[x][0] = d[x+y*GetMapData(0)->width];
				if (bg0[x][0] == 1) bg0[x][0] = 4;
				break;
			}
			if (stage < 10) {
				if (x == 5) {
					if (bg0[x][0] == 2) {
						bg0[x][0] = 0;
					}
				}
			}
		}
	}
#endif
}

void QBCarRace::LoadStage(const char* f)
{
#if 0
	FILE* fp = fopen(f,"rb");
	int s=0;
	if (fp) {
		while (!feof(fp)) {
			fgets(&stage[s][1],255,fp);
			s = s+1;
		}
		fclose(fp);
	}
#endif
}

int QBCarRace::ExitGame()
{
	SetParam("hiscore",hiscore);
	return 0;
}

void QBCarRace::PutFont(int x,int y,int pat,unsigned char r,unsigned char g,unsigned char b,unsigned char a)
{
	QBGame::Put(x,y,pat,PG_FONT,QBCHAR_FONT,r,g,b,a);
/*
	int i = polyCount;

	poly[i].r = r;
	poly[i].g = g;
	poly[i].b = b;
	poly[i].alpha = 255;
//	poly[i].blend = PPGameBlend_Color;
	poly[i++].splite(x*2,y*2,pat,PG_FONT,QBCHAR_FONT);
	
	polyCount++;
*/
}

void QBCarRace::Put(int x,int y,int pat)
{
	QBGame::Put(x*2,y*2,pat,PG_CARRACE,QBCHAR_CARRACE);
/*
	int i = polyCount;

	poly[i].r = 255;
	poly[i].g = 255;
	poly[i].b = 255;
	poly[i].alpha = 255;
	poly[i++].splite(x*2,y*2,pat,PG_CARRACE,QBCHAR_CARRACE);
	
	polyCount++;
*/
}

void QBCarRace::initCar()
{
	for (int i=0;i<QBCARRACE_MAX_ENEMY;i++) {
		enemy[i].flag = 0;
	}
	enemySetInterval = 80;
	enemySetTimer = 10;
}

void QBCarRace::setCar(int sety)
{
	for (int i=0;i<QBCARRACE_MAX_ENEMY;i++) {
		if (enemy[i].flag == 0) {
			enemy[i].flag = 1+(rand() % 3);
			enemy[i].x = 5;
			{
				int n=0;
				int m=0;
				for (int x=0;x<=10;x++) {
					if (bg0[x][sety] == 0) {
						n ++;
					}
				}
				if (n == 0) {
					enemy[i].flag = 0;
					break;
				}
				m = (rand() % n);
				n = 0;
				for (int x=0;x<=10;x++) {
					if (bg0[x][sety] == 0) {
						if (m == n) {
							enemy[i].x = x;
							break;
						}
						n ++;
					}
				}
			}
			if (sety > 0) {
				enemy[i].y = sety+1;
			} else {
				enemy[i].y = sety-1;
			}
			if ((rand() % 40) < 5 && stage > 5) {
				enemy[i].speed = -0.2-((rand() % 11)/10.0)*0.20;
			} else {
				enemy[i].speed = -0.2;
			}
			enemy[i].hiddenCount = 0;
			break;
		}
	}
}

void QBCarRace::idleCar()
{
	enemySetInterval = 100-stage*5;
	if (enemySetInterval < 12) {
		enemySetInterval = 12;
	}

	for (int i=0;i<QBCARRACE_MAX_ENEMY;i++) {
		if (enemy[i].flag > 0) {
			//enemy[i].x = 0;
			enemy[i].y += enemy[i].speed;
			if (enemy[i].y < -5) {
				enemy[i].flag = 0;
			} else
			if (enemy[i].y < -1) {
				enemy[i].hiddenCount ++;
				if (enemy[i].hiddenCount > 120) {
					enemy[i].flag = 0;
				}
			} else
			if (enemy[i].y > DISP_Y) {
				enemy[i].hiddenCount ++;
				if (enemy[i].hiddenCount > 120) {
					enemy[i].flag = 0;
				}
			} else
			if (enemy[i].y > 15) {
				enemy[i].flag = 0;
			} else {
				enemy[i].hiddenCount = 0;
			}
			
			if (enemy[i].flag > 0) {
				int x = ((int)enemy[i].x);
				int y = ((int)enemy[i].y);
				
				if (y > 0) {
					bool flag=false;
					if (bg0[x][y-1] != 0 && bg0[x][y-1] < 9) {
						flag = true;
						if (rand() % 2) {
							if (flag) {
								int d = x-1;
								if (d < 0) d += 11;
								if (bg0[d][y-1] == 0 || bg0[d][y-1] >= 9) {
									enemy[i].x = d;
									flag = false;
								}
							}
							if (flag) {
								int d = x+1;
								if (d >= 11) d -= 11;
								if (bg0[d][y-1] == 0 || bg0[d][y-1] >= 9) {
									enemy[i].x = d;
									flag = false;
								}
							}
						} else {
							if (flag) {
								int d = x+1;
								if (d >= 11) d -= 11;
								if (bg0[d][y-1] == 0 || bg0[d][y-1] >= 9) {
									enemy[i].x = d;
									flag = false;
								}
							}
							if (flag) {
								int d = x-1;
								if (d < 0) d += 11;
								if (bg0[d][y-1] == 0 || bg0[d][y-1] >= 9) {
									enemy[i].x = d;
									flag = false;
								}
							}
						}
					}
					if (flag) {
						int x = ((int)enemy[i].x);
						int y = ((int)enemy[i].y);
						if (x < 6) {
							enemy[i].x += 1.0;
						} else
						if (x >= 6) {
							enemy[i].x -= 1.0;
						} else {
							enemy[i].flag = 0;
							bg0[x][y] = 9;
						}
					}
				}
				
				x = ((int)enemy[i].x);
				
				if (x == mx && y+1 == my) {
					if (Step() == STEP_MAINLOOP) {
						Stop();
						Play("t255M5l32o2v32fedc<fedo2fedc<fedo2fedc<fedcfrM0");
						gameoverCount = 0;
						SetStep(STEP_GAMEOVER);
						enemy[i].flag = 0;
						StopBGM();
					}
				}
			}
		}
	}
}

void QBCarRace::dispCar()
{
	for (int i=0;i<QBCARRACE_MAX_ENEMY;i++) {
		if (enemy[i].flag > 0) {
			int x = ((int)enemy[i].x);
			int y = ((int)enemy[i].y);
			if (x >= 0 && x <= 10
			 && y >= 0 && y <= DISP_Y) {
				Put(enemy[i].x*16-8,enemy[i].y*16-16+mapdy*16,enemy[i].flag+4);
			}
		}
	}
}

void QBCarRace::Save(const char* filepath)
{
	SetParam("carRace_st",st);
	SetParam("carRace_area",area);
	SetParam("carRace_arealoop",arealoop);
	SetParam("carRace_stage",stage);
}

void QBCarRace::LoadGame()
{
	Load(PPGameResourcePath("CarRace.dat"));
}

void QBCarRace::Load(const char* filepath)
{
	st = GetParam("carRace_st");
	area = GetParam("carRace_area",1);
	arealoop = GetParam("carRace_arealoop");
	stage = GetParam("carRace_stage");
}

void QBCarRace::Setting()
{
	DispScr(1);
	Color(13);
	Locate(3,4);
	Print("CAR RACE PANIC");
	Color(15);
	menu[curMenu].x = 3;
	menu[curMenu].y = 9;
	menu[curMenu].w = 14;
	menu[curMenu].h = 9;
	menu[curMenu].string[0] = "EXIT";
	menu[curMenu].string[1] = "BGM SELECT";
	menu[curMenu].string[2] = "BGM RESET";
	menu[curMenu].string[3] = NULL;
	menu[curMenu].draw(this);
	if (KeyWait(PAD_UP)) {
		menu[curMenu].up();
	} else
	if (KeyWait(PAD_DOWN)) {
		menu[curMenu].down();
	}
	if (KeyWait(PAD_RIGHT) || KeyWait(PAD_LEFT)) {
		if (menu[curMenu].cur > 0 && menu[curMenu].cur < 2) {
			PlayBGM(menu[curMenu].cur);
			menu[curMenu].play();
		}
	}
	if (KeyWait(PAD_A,true) || KeyWait(PAD_B,true) || menu[curMenu].select) {
		switch (menu[curMenu].cur) {
		case 0:
			menu[curMenu].cur = 2;
			menu[curMenu].play();
			SetStep(STEP_TITLE);
			StopBGM();
			break;
		case 1:
			menu[curMenu].cur = 0;
			SelectBGM(1);
			menu[curMenu].play();
			break;
		case 2:
			ResetBGM();
			StopBGM();
			menu[curMenu].cur = 2;
			menu[curMenu].playDispose();
			SetStep(STEP_TITLE);
			break;
		}
	}
}

int QBCarRace::Step()
{
	return step;
}

int QBCarRace::SetStep(int istep)
{
	step = istep;
	return 0;
}

int QBCarRace::NextStep()
{
	SetStep(Step()+1);
	return 0;
}

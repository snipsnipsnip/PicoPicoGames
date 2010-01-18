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

#include "QBTankGame.h"
#include <stdio.h>
#include <stdlib.h>
#include "font.h"
#include "font2.h"
#include "key.h"

#define BG_W 11
#define BG_H 15

#define SD_SHOT "t255l8M5o6v32dcr"
#define SD_BANG "t255M5o6l32v32acav28cacacv24acacav20cacacav18cacacv14acacacacv12acacv8acacac"
#define SD_HIT "t255M5o7l32v32gfagfagfv28accv8ccv15ccv6ccv15cv4cv3cv2cv1ccccc"
#define SD_GET "t255M0o4l32v32efgabefgab"
#define SD_MOVE "t255l64M0o5v32dcr"

enum {
	STEP_GAME,
	STEP_GAMEOVER,
	STEP_ENDING
};

enum {
	QBCHAR_FONT,
	QBCHAR_FONT2,
	QBCHAR_KEY,
	QBCHAR_TANK,
};

static PPGameTextureInfo texList[] = {
	{"font.png",font_group,false},
	{"font2.png",font2_group,false},
	{"key.png",key_group,false},
	{"QBTankGame.png",NULL,false},
	{NULL,NULL,false}
};

QBTankGame::QBTankGame()
{
}

QBTankGame::~QBTankGame()
{
}

PPGameTextureInfo* QBTankGame::TextureInfo()
{
	return texList;
}

void QBTankGame::drawTitle()
{
	Color(15);
	Locate(4,0);
	Print("HI-SCORE %d",hiscore);

	Color(12);
	Offset(0+24-8,3*16+4);
	Print2("TANK GAME");
	Color(14);
	Offset(0-4+24-8,3*16+4-4);
	Print2("TANK GAME");
}

void QBTankGame::gameInit()
{
	InitBGM(1,"QBTankGamePlay");
	InitBGM(2,"QBTankGameEnding");
	hiscore = GetParam("QBTankGameHIScore",10);
}

void QBTankGame::gameStart()
{
	tankx = 5;
	tanky = 10;
	tankc = 8;
	tamax = 0;
	tamay = 0;
	tamad = -1;
	tamat = 0;
	memset(bg,0,sizeof(bg));
	for (int y=0;y<BG_H;y++) {
		bg[0][y] = 2;
		bg[BG_W-1][y] = 2;
	}
	timer = 0;
	step = STEP_GAME;
	score = 0;
	distance = 1000;
	PlayBGM(1);
}

void QBTankGame::gameContinue()
{
	int dist = distance;
	gameStart();
	distance = (dist/100+1)*100;
}

int QBTankGame::gameIdle()
{
	static int tbl[] = {
		1,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,1,1,1,
		1,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,1,1,1,
		4,5,6,7
	};
	
	rand();

	switch (step) {
	case STEP_GAME:
		/* キー入力 */
		{
			if (KeyWait(PAD_UP)) {
				/*if (tankc == 8)*/ tanky --;
				tankc = 8;
				if (tanky < 0) tanky = 0;
				if (!IsPlaying()) Play(SD_MOVE);
			}
			if (KeyWait(PAD_DOWN)) {
				/*if (tankc == 10)*/ tanky ++;
				tankc = 8;
				if (tanky >= BG_H-3) tanky = BG_H-1-3;
				if (!IsPlaying()) Play(SD_MOVE);
			}
			if (KeyWait(PAD_LEFT)) {
				/*if (tankc == 11)*/ tankx --;
				tankc = 11;
				if (!IsPlaying()) Play(SD_MOVE);
			}
			if (KeyWait(PAD_RIGHT)) {
				/*if (tankc == 9)*/ tankx ++;
				tankc = 9;
				if (!IsPlaying()) Play(SD_MOVE);
			}
			if (KeyWait(PAD_A) || KeyWait(PAD_B)) {
				if (tamad < 0) {
					tamax = tankx;
					tamay = tanky;
					tamad = tankc;
					tamat = 5;
					Play(SD_SHOT);
				}
			}
		}

		/* スクロール */
		{
			timer ++;
			if (timer > 16) {
				timer = 0;
				for (int y=BG_H-1;y>0;y--) {
					for (int x=1;x<BG_W-1;x++) {
						bg[x][y] = bg[x][y-1];
					}
				}
				for (int x=1;x<BG_W-1;x++) {
					if (distance <= BG_H) {
						if (x == 4 || x == 5 || x == 6) {
							bg[x][0] = 0;
						} else {
							bg[x][0] = 2;
						}
					} else
					if (distance <= BG_H+2) {
						bg[x][0] = 0;
					} else {
						bg[x][0] = tbl[rand() % (sizeof(tbl)/sizeof(int))];
					}
				}
				distance --;
				if (distance < 0) {
					distance = 0;
					PlayBGM(2,false);
					step = STEP_ENDING;
				}
			}
		}

		/* 弾 */
		tamat ++;
		if (tamat > 4) {
			tamat = 0;
			while (tamad >= 0) {
				if (bg[tamax][tamay] >= 4) {
					bg[tamax][tamay] = 0;
					tamad = -1;
					score ++;
					Stop(); Play(SD_HIT);
				} else
				if (bg[tamax][tamay] > 1 && bg[tamax][tamay] != 3) {
					tamad = -1;
					break;
				}
				switch (tamad) {
				case 8:
					tamay --;
					break;
				case 9:
					tamax ++;
					break;
				case 10:
					tamay ++;
					break;
				case 11:
					tamax --;
					break;
				}
				if (tamay < 0) {
					tamad = -1;
					break;
				}
				if (bg[tamax][tamay] >= 4) {
					bg[tamax][tamay] = 0;
					tamad = -1;
					score ++;
					Stop(); Play(SD_HIT);
				} else
				if (bg[tamax][tamay] > 1 && bg[tamax][tamay] != 3) {
					tamad = -1;
				}
				break;
			}
		}
		
		/* 当たり判定 */
		{
			if (bg[tankx][tanky] == 3) {
				bg[tankx][tanky] = 0;
				score ++;
				Play(SD_GET);
			} else
			if (bg[tankx][tanky] > 1) {
				SetParam("QBTankGameHIScore",hiscore);
				step = STEP_GAMEOVER;
				StopBGM();
				Play(SD_BANG);
			}
		}
		
		/* 画面の描画 */
		{
			for (int y=0;y<BG_H;y++) {
				for (int x=0;x<BG_W;x++) {
					if (bg[x][y]) Put(x*32-16,y*32,bg[x][y],0,QBCHAR_TANK);
				}
			}
			Put(tankx*32-16,tanky*32,tankc,0,QBCHAR_TANK);
			if (tamad >= 0) Put(tamax*32-16,tamay*32,12,0,QBCHAR_TANK);
		}
		
		/* スコアの表示 */
		{
			Color(15);
			if (hiscore <= score) {
				Locate(0,0);
				Print("HI-SCORE %4d DST.%d",score,distance);
			} else {
				Locate(0,0);
				Print("SCORE %4d DST.%d",score,distance);
			}
		}
		
		if (hiscore < score) {
			hiscore = score;
		}

		break;
	case STEP_GAMEOVER:

		if (!IsPlaying() && (KeyWait(PAD_A,true) || KeyWait(PAD_B,true))) {
			return -1;
		}

		/* 画面の描画(爆発) */
		{
			for (int y=0;y<BG_H;y++) {
				for (int x=0;x<BG_W;x++) {
					if (bg[x][y]) Put(x*32-16,y*32,bg[x][y],0,QBCHAR_TANK);
				}
			}
			Put(tankx*32-16+(rand() % 64)-32,tanky*32+(rand() % 64)-32,13,0,QBCHAR_TANK);
		}

		/* スコアの表示 */
		{
			Color(15);
			if (hiscore <= score) {
				Locate(0,0);
				Print("HI-SCORE %4d DST.%d",score,distance);
			} else {
				Locate(0,0);
				Print("SCORE %4d DST.%d",score,distance);
			}
		}

		Color(15);
		Offset((320-9*32)/2,10*16+4);
		Print2("GAME OVER");

		break;
	case STEP_ENDING:
		if (!IsPlaying() && (KeyWait(PAD_A,true) || KeyWait(PAD_B,true))) {
			StopBGM();
			return -1;
		}

		Color(15);
		Offset((320-16*16)/2,10*16+4);
		Print("CONGURATULATION!");
		break;
	}
	
	return 0;
}

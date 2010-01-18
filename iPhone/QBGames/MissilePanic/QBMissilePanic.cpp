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

#include "QBMissilePanic.h"
#include "font.h"
#include "font2.h"
#include "key.h"

static PPGameTextureInfo texList[] = {
	{"MissilePanic.png",NULL,false},
	{"font.png",font_group,false},
	{"font2.png",font2_group,false},
	{"key.png",key_group,false},
	{NULL,NULL,false}
};

#define OBJ_MAX QBMISSILEPANIC_OBJ_MAX
#define BG_W QBMISSILEPANIC_MAP_WIDTH
#define BG_H QBMISSILEPANIC_MAP_HEIGHT
#define SMOKE_MAX QBMISSILEPANIC_SMOKE_MAX

//#define SD_START "t255M0o5l16v32acde"
#define SD_TIMER "t255M0o5l32v32cf"
#define SD_TARGET "t255M5o7l32v32gfagfagfv28accv8ccv15ccv6ccv15cv4cv3cv2cv1ccccc"
#define SD_ENERGY "t255M0o4l32v32efgabefgab"
#define SD_FLAG "t255M0l32v32o5gabo4cdefgabo5gabo4cdefgab"
#define SD_BANG "t255M5o6l32v32acav28cacacv24acacav20cacacav18cacacv14acacacacv12acacv8acacac"
#define SD_MOVE "t255l64M0o5v32dcr"
#define SD_START "t255M0o4l64v32CDEFGAB>C<"
#define SD_BIG_BANG "t255M5o6l16v32CACAv26CACACACA22CACAv20CACACAC16ACACACACAv10CACACACv5ACACACACA"
#define SD_PI "T256M0v32L32O5ECO4A"

#define FINAL_STAGE 32

static unsigned char bangTable[10][10] = {
	{15,14,14,13,12,11,11,10,10, 9},
	{14,13,13,12,11,10,10, 9, 9, 8},
	{14,13,12,11,10, 9, 9, 8, 8, 7},
	{13,12,11,10, 9, 8, 8, 7, 7, 6},
	{12,11,10, 9, 8, 7, 7, 6, 6, 5},
	{11,10, 9, 8, 7, 6, 6, 5, 5, 4},
	{11,10, 9, 8, 7, 6, 5, 4, 4, 3},
	{10, 9, 8, 7, 6, 5, 4, 3, 3, 2},
	{10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
	{ 9, 8, 7, 6, 5, 4, 3, 2, 1, 0}
};

static unsigned char spaceTable[12][10] = {
	{4,0,0,2,0,0,3,0,0,2},
	{2,3,2,4,2,2,2,2,0,0},
	{0,2,0,0,0,0,0,0,0,4},
	{3,0,0,0,3,0,0,3,0,4},
	{0,0,0,0,0,0,0,0,0,0},
	{2,0,4,0,0,0,0,0,3,0},
	{0,0,0,3,0,0,2,0,0,0},
	{2,0,0,0,0,0,2,3,2,2},
	{0,2,0,0,0,0,0,4,0,0},
	{0,4,0,0,0,2,3,2,2,0},
	{2,0,0,2,2,0,0,0,0,0},
	{0,2,2,0,0,3,0,0,0,2}
};

typedef struct _stage_st {
	const char* fname;
	int start_speed;
	int start_smoke;
} stage_st;

static stage_st stage_data_table[FINAL_STAGE] = {
	{"QBMissilePanic01.plist",16,3},
	{"QBMissilePanic02.plist",16,3},
	{"QBMissilePanic03.plist",16,3},
	{"QBMissilePanic04.plist",16,3},
	{"QBMissilePanic22.plist",16,3},
	{"QBMissilePanic05.plist",16,3},
	{"QBMissilePanic06.plist",16,3},
	{"QBMissilePanic07.plist",16,3},
	{"QBMissilePanic08.plist",16,3},
	{"QBMissilePanic09.plist",16,3},
	{"QBMissilePanic10.plist",16,3},
	{"QBMissilePanic12.plist",16,3},
	{"QBMissilePanic14.plist",16,3},
	{"QBMissilePanic16.plist",16,3},
	{"QBMissilePanic17.plist",16,3},
	{"QBMissilePanic18.plist",16,3},
	{"QBMissilePanic19.plist",16,3},
	{"QBMissilePanic20.plist",16,3},
	{"QBMissilePanic21.plist",16,3},
	{"QBMissilePanic23.plist",16,3},
	{"QBMissilePanic24.plist",16,3},
	{"QBMissilePanic25.plist",16,3},
	{"QBMissilePanic26.plist",16,3},
	{"QBMissilePanic27.plist",16,3},
	{"QBMissilePanic28.plist",16,3},
	{"QBMissilePanic29.plist",16,3},
	{"QBMissilePanic30.plist",16,3},
	{"QBMissilePanic11.plist",16,3},
	{"QBMissilePanic13.plist",16,3},
	{"QBMissilePanic15.plist",16,3},
	{"QBMissilePanic31.plist",16,3},
	{"QBMissilePanic32.plist",16,3},
};

enum {
	C_NONE,
	C_SPACE,
	C_RED_BAR = 6,
	C_GREEN_BAR,

	C_MISSLE_UP,
	C_MISSLE_RIGHT,
	C_MISSLE_DOWN,
	C_MISSLE_LEFT,

	C_ENEMY_UP,
	C_ENEMY_RIGHT,
	C_ENEMY_DOWN,
	C_ENEMY_LEFT,

	C_YELLOW,
	C_BLUE,
	C_RED,
	C_GREEN,

	C_FLAG = C_YELLOW+8,
	C_TARGET,
	C_ENERGY,
	C_SMOKE,
	C_PIPE,
	C_ENEMY_OBJ,
	C_ENEMY_OBJ_LEFT = C_ENEMY_OBJ,
	C_ENEMY_OBJ_RIGHT,
	
	C_BG_OBJECT = C_FLAG+8
};

enum {
	O_NONE,
	O_ENEMY,
	O_FLAG,
	O_TARGET,
	O_ENERGY,
	
	O_FIRE_UP,
	O_FIRE_UP_RIGHT,
	O_FIRE_UP_LEFT,
	O_FIRE_DOWN,
	O_FIRE_DOWN_RIGHT,
	O_FIRE_DOWN_LEFT,
	O_FIRE_LEFT,
	O_FIRE_RIGHT,
	
	O_ENEMY_UP,
	O_ENEMY_DOWN,
	O_ENEMY_LEFT,
	O_ENEMY_RIGHT,

	O_ENEMY_OBJ_RIGHT,
	O_ENEMY_OBJ_LEFT,
};

enum {
	P_UP,
	P_RIGHT,
	P_DOWN,
	P_LEFT
};

QBMissilePanic::QBMissilePanic()
{
	scr_w = 11;
	scr_h = 13;
	scr_offset_x = -16;
	scr_offset_y = -16;
	step = 0;
	InitBGM(1,"QBMissilePanicBGM");
	InitBGM(2,"QBMissilePanicClear");
	InitBGM(3,"QBMissilePanicEnding");
}

QBMissilePanic::~QBMissilePanic()
{
}

PPGameTextureInfo* QBMissilePanic::TextureInfo()
{
	return texList;
}

int QBMissilePanic::KeySize()
{
	if (bigKeyArea) {
		return 96*96;
	}
	return 64*64;
}

unsigned long QBMissilePanic::VisibleButton()
{
	if (!key_disp) return 0;
	return QBGAME_CONTROLLER_ENABLE_ARROW | QBGAME_CONTROLLER_ENABLE_A | QBGAME_CONTROLLER_ENABLE_B;
}

void QBMissilePanic::PutFont(int x,int y,int pat,unsigned char r,unsigned char g,unsigned char b,unsigned char a)
{
	QBGame::Put(x,y,pat,PG_FONT,QBCHAR_FONT,r,g,b,a);
}

void QBMissilePanic::Put(int x,int y,int pat)
{
	QBGame::Put(x,y,pat,0,QBCHAR_MISSILEPANIC);
}

void QBMissilePanic::StartGame()
{
//	stage %= FINAL_STAGE;
	if (stage >= FINAL_STAGE-1) stage = FINAL_STAGE-1;

	LoadMap(stage_data_table[stage].fname);
	
	missile_dir = 0;
	missile_energy_max = 3;
	missile_energy = 3;
	missile_count = 0;

	missile_start_speed = stage_data_table[stage].start_speed;
	missile_speed_up_count = 1;
	missile_speed = missile_start_speed;
	
	target_count = 0;
	flag_count = 0;
	energy_count = 0;
	
	bang_count = 0;
	bang_timer = 0;
	
	play_start_time = 30*100;
	play_time = play_start_time;
	
	flash_count = 0;
	
	smoke_start_c = stage_data_table[stage].start_smoke;
	smoke_c = smoke_start_c;
	
	for (int i=0;i<OBJ_MAX;i++) {
		otype[i] = O_NONE;
	}

	map_w = GetMapData(0)->width;
	map_h = GetMapData(0)->height;
	{
		int i=0;
		if (GetMapData(i)) {
			for (int y=0;y<GetMapData(i)->height&&y<BG_H;y++) {
				for (int x=0;x<GetMapData(i)->width&&x<BG_W;x++) {
					bg[0][y][x] = GetMapData(i)->map[x+y*GetMapData(i)->width];
					bg[1][y][x] = 0;
//printf("%02X,",bg[0][y][x]);
					if (bg[0][y][x] == C_NONE) {
						switch (rand() % 5) {
						case 1:
							bg[0][y][x] = C_SPACE;
							break;
						case 2:
							bg[0][y][x] = C_SPACE+1;
							break;
						case 3:
							bg[0][y][x] = C_SPACE+2;
							break;
						default:
							break;
						}
					}
					if (bg[0][y][x] >= C_BG_OBJECT && ((bg[0][y][x] - C_BG_OBJECT) % 8) == 0) {
						int o = (stage / 8) % 4;
						switch (o) {
						case 0:
							break;
						case 1:
							bg[0][y][x] += 3;
							break;
						case 2:
							bg[0][y][x] += 1;
							break;
						case 3:
							bg[0][y][x] += 2;
							break;
						}
					}
				}
//				printf("\n");
			}
		}
//		printf("\n");
	}
	{
		int i=1;
		if (GetMapData(i)) {
			for (int y=0;y<GetMapData(i)->height&&y<BG_H;y++) {
				for (int x=0;x<GetMapData(i)->width&&x<BG_W;x++) {
					switch (GetMapData(i)->map[x+y*GetMapData(i)->width]) {
					case C_MISSLE_UP:
						missile_dir = 0;
						missile_x = x;
						missile_y = y;
						break;
					case C_MISSLE_DOWN:
						missile_dir = 2;
						missile_x = x;
						missile_y = y;
						break;
					case C_MISSLE_RIGHT:
						missile_dir = 1;
						missile_x = x;
						missile_y = y;
						break;
					case C_MISSLE_LEFT:
						missile_dir = 3;
						missile_x = x;
						missile_y = y;
						break;
					case C_ENEMY_UP:
						ochar[SetObj(O_ENEMY_UP,x,y)] = C_ENEMY_UP;
						break;
					case C_ENEMY_RIGHT:
						ochar[SetObj(O_ENEMY_RIGHT,x,y)] = C_ENEMY_RIGHT;
						break;
					case C_ENEMY_DOWN:
						ochar[SetObj(O_ENEMY_DOWN,x,y)] = C_ENEMY_DOWN;
						break;
					case C_ENEMY_LEFT:
						ochar[SetObj(O_ENEMY_LEFT,x,y)] = C_ENEMY_LEFT;
						break;
					case C_FLAG:
						ochar[SetObj(O_FLAG,x,y)] = C_FLAG;
						flag_count ++;
						break;
					case C_TARGET:
						ochar[SetObj(O_TARGET,x,y)] = C_TARGET;
						target_count ++;
						break;
					case C_ENERGY:
						ochar[SetObj(O_ENERGY,x,y)] = C_ENERGY;
						energy_count ++;
						break;
					case C_ENEMY_OBJ_RIGHT:
						ochar[SetObj(O_ENEMY_OBJ_RIGHT,x,y)] = C_ENEMY_OBJ;
						break;
					case C_ENEMY_OBJ_LEFT:
						ochar[SetObj(O_ENEMY_OBJ_LEFT,x,y)] = C_ENEMY_OBJ;
						break;
					}
				}
			}
		}
	}
	
	for (int i=0;i<SMOKE_MAX;i++) {
		smoke_x[i] = missile_x;
		smoke_y[i] = missile_y;
	}
	
	Play(SD_START);
}

void QBMissilePanic::DrawInfo()
{
	//画面上部の非表示//
	{
		Fill(0,0,320,16,0);
	}

/*
	//スコア//
	{
		Locate(0,1);
		Color(15);
		Print("%d",score);
	}
*/

	//ライフゲージの表示//
	{
		int x,y;
		x = 0;
		y = 480-96-32+6;
		Offset(x,y+8);
		Color(15);
		Print("ENERGY");
		x+=16*6;
		for (int i=0;i<missile_energy_max;i++) {
			if (i < missile_energy) {
				Put(x,y,C_GREEN_BAR);
			} else {
				Put(x,y,C_RED_BAR);
			}
			x += 32;
		}
	}

	//エネルギー数の表示//
	{
		int x,y;
		x = 320-64-64-64;
		y = 32;//480-96;
		PutHalf(x+16,y-32,C_ENERGY);
		Offset(x+32,y-32);
		Color(15);
		if (energy_count < 10) {
			Print("x%d",energy_count);
		} else {
			Print("%2d",energy_count);
		}
	}

	//フラッグ数の表示//
	{
		int x,y;
		x = 320-64-64;
		y = 32;//480-96;
		PutHalf(x+16,y-32,C_FLAG);
		Offset(x+32,y-32);
		Color(15);
		if (flag_count < 10) {
			Print("x%d",flag_count);
		} else {
			Print("%2d",flag_count);
		}
	}

	//ターゲット数の表示//
	{
		int x,y;
		x = 320-64;
		y = 32;//480-96;
		PutHalf(x+16,y-32,C_TARGET);
		Offset(x+32,y-32);
		Color(15);
		if (target_count < 10) {
			Print("x%d",target_count);
		} else {
			Print("%2d",target_count);
		}
	}

	//タイマーの表示//
	{
		Locate(0,0);
		Color(15);
		int t = (play_time % 100);
		if (play_time < 5*100) {
			if (missile_energy > 0) {
				if (t == 99) {
					Play(SD_TIMER);
				}
				if (t == 74) {
					Play(SD_TIMER);
				}
				if (t == 49) {
					Play(SD_TIMER);
				}
				if (t == 24) {
					Play(SD_TIMER);
				}
			}
			if (t > 90) {
				Color(12);
			} else
			if (t > 65 && t < 75) {
				Color(12);
			} else
			if (t > 40 && t < 50) {
				Color(12);
			} else
			if (t > 15 && t < 25) {
				Color(12);
			}
		} else
		if (play_time < 10*100) {
			if (missile_energy > 0) {
				if (t == 99) {
					Play(SD_TIMER);
				}
				if (t == 49) {
					Play(SD_TIMER);
				}
			}
			if (t > 90) {
				Color(12);
			} else
			if (t > 40 && t < 50) {
				Color(12);
			}
		} else
		if (play_time < 20*100) {
			if (missile_energy > 0) {
				if (t == 99) {
					Play(SD_TIMER);
				}
			}
			if (t > 90) {
				Color(12);
			}
		} else
		if (play_time < play_start_time-10) {
			if (missile_energy > 0) {
				if ((play_time % (100*10)) == 999) {
					Play(SD_TIMER);
				}
			}
			if ((play_time % (100*10)) > 990) {
				Color(12);
			}
		}
		Print("TIME",play_time / 100,(play_time % 100)/10);
		Offset(16*4+8,0);
		Print("%d.%d",play_time / 100,(play_time % 100)/10);
	}

	//画面下部の非表示//
	{
		Fill(0,480-96,320,96,0);
	}
}

int QBMissilePanic::CalcPosX(int x)
{
	while (x < 0) x += map_w;
	return x;
}

int QBMissilePanic::CalcPosY(int y)
{
	while (y < 0) y += map_h;
	return y;
}

int QBMissilePanic::SetObj(int type,int x,int y)
{
	int i;
	for (i=0;i<OBJ_MAX-1;i++) {
		if (otype[i] == O_NONE) {
			otype[i] = type;
			ox[i] = x;
			oy[i] = y;
			oct[i] = 0;
			ost[i] = 0;
			return i;
		}
	}
	return OBJ_MAX-1;
}

int QBMissilePanic::LimitX(int x)
{
	while (x < 0) x += map_w;
	while (x >= map_w) x -= map_w;
	return x;
}

int QBMissilePanic::LimitY(int y)
{
	while (y < 0) y += map_h;
	while (y >= map_h) y -= map_h;
	return y;
}

void QBMissilePanic::MoveObj()
{
	for (int i=0;i<OBJ_MAX;i++) {
		switch (otype[i]) {
		case O_FIRE_UP:
		case O_FIRE_UP_LEFT:
		case O_FIRE_UP_RIGHT:
		case O_FIRE_DOWN:
		case O_FIRE_DOWN_LEFT:
		case O_FIRE_DOWN_RIGHT:
		case O_FIRE_LEFT:
		case O_FIRE_RIGHT:
			{
				int dx=0;
				int dy=0;
				oct[i] ++;
				if (oct[i] > 4) {
					oct[i] = 0;
					switch (otype[i]) {
					case O_FIRE_UP:
						dx = 0;
						dy = -1;
						break;
					case O_FIRE_UP_LEFT:
						dx = -1;
						dy = -1;
						break;
					case O_FIRE_UP_RIGHT:
						dx = 1;
						dy = -1;
						break;
					case O_FIRE_DOWN:
						dx = 0;
						dy = 1;
						break;
					case O_FIRE_DOWN_LEFT:
						dx = -1;
						dy = 1;
						break;
					case O_FIRE_DOWN_RIGHT:
						dx = 1;
						dy = 1;
						break;
					case O_FIRE_LEFT:
						dx = -1;
						dy = 0;
						break;
					case O_FIRE_RIGHT:
						dx = 1;
						dy = 0;
						break;
					}
					ox[i] += dx;
					oy[i] += dy;
					ox[i] = LimitX(ox[i]);
					oy[i] = LimitY(oy[i]);
					if (bg[0][oy[i]][ox[i]] == C_PIPE) {
						bg[0][oy[i]][ox[i]] = 1;
					} else {
						otype[i] = 0;
					}
				}
			}
			break;
		case O_ENEMY_UP:
		case O_ENEMY_DOWN:
		case O_ENEMY_LEFT:
		case O_ENEMY_RIGHT:
			{
				int dx=0;
				int dy=0;
				oct[i] ++;
				if (oct[i] > 32) {
					oct[i] = 0;
					while (1) {
						switch (otype[i]) {
						case O_ENEMY_UP:
							dx = 0;
							dy = -1;
							break;
						case O_ENEMY_DOWN:
							dx = 0;
							dy = 1;
							break;
						case O_ENEMY_LEFT:
							dx = -1;
							dy = 0;
							break;
						case O_ENEMY_RIGHT:
							dx = 1;
							dy = 0;
							break;
						}
						{
							ox[i] += dx;
							oy[i] += dy;
							ox[i] = LimitX(ox[i]);
							oy[i] = LimitY(oy[i]);
						}
						if (bg[0][oy[i]][ox[i]] >= C_RED_BAR) {
							dx = -dx;
							dy = -dy;
							ox[i] += dx;
							oy[i] += dy;
							switch (otype[i]) {
							case O_ENEMY_UP:
								otype[i] = O_ENEMY_DOWN;
								ochar[i] = C_ENEMY_DOWN;
								break;
							case O_ENEMY_DOWN:
								otype[i] = O_ENEMY_UP;
								ochar[i] = C_ENEMY_UP;
								break;
							case O_ENEMY_LEFT:
								otype[i] = O_ENEMY_RIGHT;
								ochar[i] = C_ENEMY_RIGHT;
								break;
							case O_ENEMY_RIGHT:
								otype[i] = O_ENEMY_LEFT;
								ochar[i] = C_ENEMY_LEFT;
								break;
							}
						} else {
							break;
						}
					}
				}
			}
			break;
		case O_ENEMY_OBJ_RIGHT:
			oct[i] ++;
			if (oct[i] > 16) {
				oct[i] = 0;
				switch (ost[i]) {
				case 0:
					{
						{
							int x = LimitX(ox[i]  );
							int y = LimitY(oy[i]+1);
							if (bg[0][y][x] >= C_FLAG) {
								ost[i] = 3;
							}
						}
						{
							int x = LimitX(ox[i]+1);
							int y = LimitY(oy[i]  );
							if (bg[0][y][x] >= C_FLAG) {
								ost[i] = 2;
							}
						}
						{
							int x = LimitX(ox[i]  );
							int y = LimitY(oy[i]-1);
							if (bg[0][y][x] >= C_FLAG) {
								ost[i] = 1;
							}
						}
						{
							int x = LimitX(ox[i]-1);
							int y = LimitY(oy[i]  );
							if (bg[0][y][x] >= C_FLAG) {
								ost[i] = 4;
							}
						}
					}
					break;
				case 1:		//左へ移動//
					{
						int x = LimitX(ox[i]  );
						int y = LimitY(oy[i]-1);
						if (bg[0][y][x] >= C_FLAG) {
							int x = LimitX(ox[i]-1);
							int y = LimitY(oy[i]  );
							if (bg[0][y][x] >= C_FLAG) {
								int x = LimitX(ox[i]  );
								int y = LimitY(oy[i]+1);
								if (bg[0][y][x] >= C_FLAG) {
									ox[i] += 1;
									ox[i] = LimitX(ox[i]  );
									ost[i] = 3;
								} else {
									oy[i] += 1;
									oy[i] = LimitY(oy[i]  );
									ost[i] = 4;
								}
							} else {
								ox[i] -= 1;
								ox[i] = LimitX(ox[i]  );
								ost[i] = 1;
							}
						} else {
							oy[i] -= 1;
							oy[i] = LimitY(oy[i]  );
							ost[i] = 2;
						}
					}
					break;
				case 2:		//上へ移動//
					{
						int x = LimitX(ox[i]+1);
						int y = LimitY(oy[i]  );
						if (bg[0][y][x] >= C_FLAG) {
							int x = LimitX(ox[i]  );
							int y = LimitY(oy[i]-1);
							if (bg[0][y][x] >= C_FLAG) {
								int x = LimitX(ox[i]-1);
								int y = LimitY(oy[i]  );
								if (bg[0][y][x] >= C_FLAG) {
									oy[i] += 1;
									oy[i] = LimitY(oy[i]  );
									ost[i] = 4;
								} else {
									ox[i] -= 1;
									ox[i] = LimitX(ox[i]  );
									ost[i] = 1;
								}
							} else {
								oy[i] -= 1;
								oy[i] = LimitY(oy[i]  );
								ost[i] = 2;
							}
						} else {
							ox[i] += 1;
							ox[i] = LimitX(ox[i]  );
							ost[i] = 3;
						}
					}
					break;
				case 3:		//右へ移動//
					{
						int x = LimitX(ox[i]  );
						int y = LimitY(oy[i]+1);
						if (bg[0][y][x] >= C_FLAG) {
							int x = LimitX(ox[i]+1);
							int y = LimitY(oy[i]  );
							if (bg[0][y][x] >= C_FLAG) {
								int x = LimitX(ox[i]  );
								int y = LimitY(oy[i]-1);
								if (bg[0][y][x] >= C_FLAG) {
									ox[i] -= 1;
									ox[i] = LimitX(ox[i]  );
									ost[i] = 1;
								} else {
									oy[i] -= 1;
									oy[i] = LimitY(oy[i]  );
									ost[i] = 2;
								}
							} else {
								ox[i] += 1;
								ox[i] = LimitX(ox[i]  );
								ost[i] = 3;
							}
						} else {
							oy[i] += 1;
							oy[i] = LimitY(oy[i]  );
							ost[i] = 4;
						}
					}
					break;
				case 4:		//下へ移動//
					{
						int x = LimitX(ox[i]-1);
						int y = LimitY(oy[i]  );
						if (bg[0][y][x] >= C_FLAG) {
							int x = LimitX(ox[i]  );
							int y = LimitY(oy[i]+1);
							if (bg[0][y][x] >= C_FLAG) {
								int x = LimitX(ox[i]+1);
								int y = LimitY(oy[i]  );
								if (bg[0][y][x] >= C_FLAG) {
									oy[i] -= 1;
									oy[i] = LimitY(oy[i]  );
									ost[i] = 2;
								} else {
									ox[i] += 1;
									ox[i] = LimitX(ox[i]  );
									ost[i] = 3;
								}
							} else {
								oy[i] += 1;
								oy[i] = LimitY(oy[i]  );
								ost[i] = 4;
							}
						} else {
							ox[i] -= 1;
							ox[i] = LimitX(ox[i]  );
							ost[i] = 1;
						}
					}
					break;
				}
			}
			break;
		case O_ENEMY_OBJ_LEFT:
			oct[i] ++;
			if (oct[i] > 16) {
				oct[i] = 0;
				switch (ost[i]) {
				case 0:
					{
						{
							int x = LimitX(ox[i]  );
							int y = LimitY(oy[i]+1);
							if (bg[0][y][x] >= C_FLAG) {
								ost[i] = 1;
							}
						}
						{
							int x = LimitX(ox[i]+1);
							int y = LimitY(oy[i]  );
							if (bg[0][y][x] >= C_FLAG) {
								ost[i] = 4;
							}
						}
						{
							int x = LimitX(ox[i]  );
							int y = LimitY(oy[i]-1);
							if (bg[0][y][x] >= C_FLAG) {
								ost[i] = 3;
							}
						}
						{
							int x = LimitX(ox[i]-1);
							int y = LimitY(oy[i]  );
							if (bg[0][y][x] >= C_FLAG) {
								ost[i] = 2;
							}
						}
					}
					break;
				case 1:		//左へ移動//
					{
						int x = LimitX(ox[i]  );
						int y = LimitY(oy[i]+1);
						if (bg[0][y][x] >= C_FLAG) {
							int x = LimitX(ox[i]-1);
							int y = LimitY(oy[i]  );
							if (bg[0][y][x] >= C_FLAG) {
								int x = LimitX(ox[i]  );
								int y = LimitY(oy[i]-1);
								if (bg[0][y][x] >= C_FLAG) {
									ox[i] += 1;
									ox[i] = LimitX(ox[i]  );
									ost[i] = 3;
								} else {
									oy[i] -= 1;
									oy[i] = LimitY(oy[i]  );
									ost[i] = 2;
								}
							} else {
								ox[i] -= 1;
								ox[i] = LimitX(ox[i]  );
								ost[i] = 1;
							}
						} else {
							oy[i] += 1;
							oy[i] = LimitY(oy[i]  );
							ost[i] = 4;
						}
					}
					break;
				case 2:		//上へ移動//
					{
						int x = LimitX(ox[i]-1);
						int y = LimitY(oy[i]  );
						if (bg[0][y][x] >= C_FLAG) {
							int x = LimitX(ox[i]  );
							int y = LimitY(oy[i]-1);
							if (bg[0][y][x] >= C_FLAG) {
								int x = LimitX(ox[i]+1);
								int y = LimitY(oy[i]  );
								if (bg[0][y][x] >= C_FLAG) {
									oy[i] += 1;
									oy[i] = LimitY(oy[i]  );
									ost[i] = 4;
								} else {
									ox[i] += 1;
									ox[i] = LimitX(ox[i]  );
									ost[i] = 3;
								}
							} else {
								oy[i] -= 1;
								oy[i] = LimitY(oy[i]  );
								ost[i] = 2;
							}
						} else {
							ox[i] -= 1;
							ox[i] = LimitX(ox[i]  );
							ost[i] = 1;
						}
					}
					break;
				case 3:		//右へ移動//
					{
						int x = LimitX(ox[i]  );
						int y = LimitY(oy[i]-1);
						if (bg[0][y][x] >= C_FLAG) {
							int x = LimitX(ox[i]+1);
							int y = LimitY(oy[i]  );
							if (bg[0][y][x] >= C_FLAG) {
								int x = LimitX(ox[i]  );
								int y = LimitY(oy[i]+1);
								if (bg[0][y][x] >= C_FLAG) {
									ox[i] -= 1;
									ox[i] = LimitX(ox[i]  );
									ost[i] = 1;
								} else {
									oy[i] += 1;
									oy[i] = LimitY(oy[i]  );
									ost[i] = 4;
								}
							} else {
								ox[i] += 1;
								ox[i] = LimitX(ox[i]  );
								ost[i] = 3;
							}
						} else {
							oy[i] -= 1;
							oy[i] = LimitY(oy[i]  );
							ost[i] = 2;
						}
					}
					break;
				case 4:		//下へ移動//
					{
						int x = LimitX(ox[i]+1);
						int y = LimitY(oy[i]  );
						if (bg[0][y][x] >= C_FLAG) {
							int x = LimitX(ox[i]  );
							int y = LimitY(oy[i]+1);
							if (bg[0][y][x] >= C_FLAG) {
								int x = LimitX(ox[i]-1);
								int y = LimitY(oy[i]  );
								if (bg[0][y][x] >= C_FLAG) {
									oy[i] -= 1;
									oy[i] = LimitY(oy[i]  );
									ost[i] = 2;
								} else {
									ox[i] -= 1;
									ox[i] = LimitX(ox[i]  );
									ost[i] = 1;
								}
							} else {
								oy[i] += 1;
								oy[i] = LimitY(oy[i]  );
								ost[i] = 4;
							}
						} else {
							ox[i] += 1;
							ox[i] = LimitX(ox[i]  );
							ost[i] = 3;
						}
					}
					break;
				}
			}
			break;
		}
	}
}

void QBMissilePanic::DrawObj()
{
	for (int i=1;i<smoke_c;i++) {
		Put((CalcPosX(smoke_x[i]-scr_x))*32+scr_offset_x,(CalcPosY(smoke_y[i]-scr_y))*32+scr_offset_y,C_SMOKE);
	}
	if (missile_energy <= 0) {
		Put(scr_w/2*32+scr_offset_x+(rand() % 32)-16,scr_h/2*32+scr_offset_y+(rand() % 32)-16,C_SMOKE);
		Put(scr_w/2*32+scr_offset_x+(rand() % 32)-16,scr_h/2*32+scr_offset_y+(rand() % 32)-16,C_SMOKE);
		Put(scr_w/2*32+scr_offset_x+(rand() % 32)-16,scr_h/2*32+scr_offset_y+(rand() % 32)-16,C_SMOKE);
	} else {
		Put(scr_w/2*32+scr_offset_x,scr_h/2*32+scr_offset_y,C_MISSLE_UP+missile_dir);
	}
	for (int i=0;i<OBJ_MAX;i++) {
		if (otype[i]) {
			switch (otype[i]) {
			case O_FIRE_UP:
			case O_FIRE_UP_LEFT:
			case O_FIRE_UP_RIGHT:
			case O_FIRE_DOWN:
			case O_FIRE_DOWN_LEFT:
			case O_FIRE_DOWN_RIGHT:
			case O_FIRE_LEFT:
			case O_FIRE_RIGHT:
				Put((CalcPosX(ox[i]-scr_x))*32+scr_offset_x+(rand() % 32)-16,(CalcPosY(oy[i]-scr_y))*32+scr_offset_y+(rand() % 32)-16,ochar[i]);
				break;
			default:
				Put((CalcPosX(ox[i]-scr_x))*32+scr_offset_x,(CalcPosY(oy[i]-scr_y))*32+scr_offset_y,ochar[i]);
				break;
			}
		}
	}
}

void QBMissilePanic::HitCheck(int f)
{
	for (int i=0;i<OBJ_MAX;i++) {
		if (otype[i]) {
			if (missile_x == ox[i] && missile_y == oy[i]) {
				switch (otype[i]) {
				case O_FLAG:
					Stop();
					Play(SD_FLAG);
					missile_energy = missile_energy_max;
					missile_speed = missile_start_speed;
					play_time = play_start_time;
					flag_count --;
					otype[i] = 0;
/*
					score += 100;
					if (hi_score < score) {
						hi_score = score;
						SetParam("PPMisslePanicHIScore",hi_score);
					}
*/
					break;
				case O_ENERGY:
					Stop();
					Play(SD_ENERGY);
					missile_energy ++;
					if (missile_energy > missile_energy_max) {
						missile_energy_max = missile_energy;
					}
					smoke_c ++;
					missile_speed -= missile_speed_up_count;
					if (missile_speed < 5) missile_speed = 5;
					otype[i] = 0;
					energy_count --;
/*
					score += 10;
					if (hi_score < score) {
						hi_score = score;
						SetParam("PPMisslePanicHIScore",hi_score);
					}
*/
					break;
				case O_TARGET:
					Stop();
					Play(SD_TARGET);
					target_count --;
					missile_energy --;
					missile_speed -= missile_speed_up_count;
					if (missile_speed < 5) missile_speed = 5;
					smoke_c ++;
					flash_count = 4;
					ochar[SetObj(O_FIRE_UP,ox[i],oy[i])] = C_SMOKE;
					ochar[SetObj(O_FIRE_UP_LEFT,ox[i],oy[i])] = C_SMOKE;
					ochar[SetObj(O_FIRE_UP_RIGHT,ox[i],oy[i])] = C_SMOKE;
					ochar[SetObj(O_FIRE_DOWN,ox[i],oy[i])] = C_SMOKE;
					ochar[SetObj(O_FIRE_DOWN_LEFT,ox[i],oy[i])] = C_SMOKE;
					ochar[SetObj(O_FIRE_DOWN_RIGHT,ox[i],oy[i])] = C_SMOKE;
					ochar[SetObj(O_FIRE_LEFT,ox[i],oy[i])] = C_SMOKE;
					ochar[SetObj(O_FIRE_RIGHT,ox[i],oy[i])] = C_SMOKE;
					otype[i] = 0;
/*
					score += 50;
					if (hi_score < score) {
						hi_score = score;
						SetParam("PPMisslePanicHIScore",hi_score);
					}
*/
					break;
				case O_ENEMY_UP:
				case O_ENEMY_DOWN:
				case O_ENEMY_LEFT:
				case O_ENEMY_RIGHT:
				case O_ENEMY_OBJ_LEFT:
				case O_ENEMY_OBJ_RIGHT:
					missile_energy = 0;
					otype[i] = 0;
					break;
				}
			}
		}
	}
	
	if (f==0) {
		for (int i=1;i<smoke_c;i++) {
			if (missile_x == smoke_x[i] && missile_y == smoke_y[i]) {
				missile_energy = 0;
			}
		}
		if (bg[0][missile_y][missile_x] >= C_BG_OBJECT) {
			missile_energy = 0;
		}
		if (bg[0][missile_y][missile_x] == C_PIPE) {
			missile_energy = 0;
		}
	}
}

void QBMissilePanic::MoveMissle()
{
	if (KeyWait(PAD_UP)) {
		Play(SD_MOVE);
		if (missile_dir == P_UP) {
			missile_count = missile_speed;
		}
		missile_dir = P_UP;
	}
	if (KeyWait(PAD_DOWN)) {
		Play(SD_MOVE);
		if (missile_dir == P_DOWN) {
			missile_count = missile_speed;
		}
		missile_dir = P_DOWN;
	}
	if (KeyWait(PAD_LEFT)) {
		Play(SD_MOVE);
		if (missile_dir == P_LEFT) {
			missile_count = missile_speed;
		}
		missile_dir = P_LEFT;
	}
	if (KeyWait(PAD_RIGHT)) {
		Play(SD_MOVE);
		if (missile_dir == P_RIGHT) {
			missile_count = missile_speed;
		}
		missile_dir = P_RIGHT;
	}

	missile_count ++;
	if (missile_count >= missile_speed) {
		missile_count = 0;
		
		switch (missile_dir) {
		case P_UP:
			missile_y --;
			break;
		case P_RIGHT:
			missile_x ++;
			break;
		case P_DOWN:
			missile_y ++;
			break;
		case P_LEFT:
			missile_x --;
			break;
		}
		
		while (missile_x < 0) missile_x += map_w;
		while (missile_y < 0) missile_y += map_h;
		while (missile_x >= map_w) missile_x -= map_w;
		while (missile_y >= map_h) missile_y -= map_h;
		
		for (int i=SMOKE_MAX-1;i>0;i--) {
			smoke_x[i] = smoke_x[i-1];
			smoke_y[i] = smoke_y[i-1];
		}
		smoke_x[0] = missile_x;
		smoke_y[0] = missile_y;
		
		HitCheck(0);
	}

	HitCheck(1);
}

void QBMissilePanic::DrawBG()
{
	while (scr_x < 0) scr_x += map_w;
	while (scr_y < 0) scr_y += map_h;
	for (int y=0;y<scr_h;y++) {
		for (int x=0;x<scr_w;x++) {
			int sx = (x+scr_x) % map_w;
			int sy = (y+scr_y) % map_h;
			for (int i=0;i<2;i++) {
				if (bg[i][sy][sx]) Put(x*32+scr_offset_x,y*32+scr_offset_y,bg[i][sy][sx]);
			}
		}
	}
}

void QBMissilePanic::DrawBang()
{
	bang_timer ++;
	if (bang_timer > 4) {
		bang_timer = 0;
		bang_count ++;
	}
	for (int y=0;y<10;y++) {
		for (int x=0;x<10;x++) {
			if (bang_count > bangTable[x][y]) {
				int c = ((15-bangTable[x][y]+bang_count) % 4)+C_YELLOW;
				Put((x-4)*32+scr_offset_x,(y-3)*32+scr_offset_y,c);
				Put((18-x-4)*32+scr_offset_x,(18-y-3)*32+scr_offset_y,c);
				Put((x-4)*32+scr_offset_x,(18-y-3)*32+scr_offset_y,c);
				Put((18-x-4)*32+scr_offset_x,(y-3)*32+scr_offset_y,c);
			}
		}
	}
}

void QBMissilePanic::DrawFlash()
{
	if (flash_count > 0) {
		flash_count --;
		if (flash_count % 2) {
			Fill(0,0,320,480-96,15);
		}
	}
}

void QBMissilePanic::CalcScreenPos()
{
	scr_x = missile_x - scr_w/2;
	scr_y = missile_y - scr_h/2;
}

enum {
	STEP_INIT,
	STEP_TITLE_INIT,
	STEP_TITLE_INIT2,
	STEP_TITLE,
	STEP_SETTING,
	STEP_START,
	STEP_STAGE_START,
	STEP_GAME,
	STEP_GAMEOVER,
	STEP_GAMEOVER_MESSAGE,
	STEP_CLEAR,
	STEP_CLEAR_MESSAGE,
	STEP_ENDING
};

void QBMissilePanic::drawTitle()
{
	{
		for (int y=0;y<12;y++) {
			for (int x=0;x<10;x++) {
				Put(x*32,y*32,spaceTable[y][x]);
			}
		}
	}

	DrawScore();
	DrawHiScore();

	Color(1);
	Offset((320-7*32)/2-32+4,3*16+4);
	TPrint("MISSILE");
	Offset((320-7*32)/2+32+64+4,5*16+4);
	TPrint("PANIC");

	Color(12);
	Offset((320-7*32)/2-32,3*16);
	TPrint("MISSILE");
	Offset((320-7*32)/2+32+64,5*16);
	TPrint("PANIC");
}

void QBMissilePanic::doTitle(int touch)
{
	drawTitle();
	Color(15);
	Offset((320-9*16)/2,22*16);
	Print("MISSION %2d",stage+1);
	Color(15);
	curMenu = 0;
	menu[curMenu].x = 4;
	menu[curMenu].y = 9;
	menu[curMenu].w = 12;
	menu[curMenu].h = 12;
	menu[curMenu].string[0] = "START";
	menu[curMenu].string[1] = "CONTINUE";
	menu[curMenu].string[2] = "OPTION";
	menu[curMenu].string[3] = "EXIT";
	menu[curMenu].string[4] = NULL;
	menu[curMenu].draw(this);

	if (touch) {
		menu[curMenu].touchOK = true;
		if (KeyWait(PAD_UP)) {
			menu[curMenu].up();
		} else
		if (KeyWait(PAD_DOWN)) {
			menu[curMenu].down();
		}
		if (KeyWait(PAD_A,true) || KeyWait(PAD_B,true) || menu[curMenu].select) {
			if (menu[curMenu].cur == 0) {
				score = 0;
				stage = 0;
				SetParam("PPMisslePanicStage",stage);
				Play(SD_START);
				SetStep(STEP_START);
			} else
			if (menu[curMenu].cur == 1) {
				score = 0;
				Play(SD_START);
				SetStep(STEP_START);
			} else
			if (menu[curMenu].cur == 2) {
				menu[curMenu].cur = 0;
				menu[curMenu].play();
				SetStep(STEP_SETTING);
			} else {
				//exitGame = true;
				Exit();
			}
		}
	} else {
		menu[curMenu].touchOK = false;
	}
}

void QBMissilePanic::doSetting()
{
	drawTitle();
	Color(15);
	curMenu = 1;
	menu[curMenu].x = 3;
	menu[curMenu].y = 7;
	menu[curMenu].w = 14;
	menu[curMenu].h = 15;
	menu[curMenu].string[0] = "EXIT";
	menu[curMenu].string[1] = "BGM PLAY";
	menu[curMenu].string[2] = "BGM CLEAR";
	menu[curMenu].string[3] = "BGM ENDING";
	menu[curMenu].string[4] = "BGM RESET";
	menu[curMenu].string[5] = NULL;
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
			menu[curMenu].cur = 2;
			menu[curMenu].play();
			SetStep(STEP_TITLE);
			StopBGM();
			break;
		case 4:
			ResetBGM();
			StopBGM();
			menu[curMenu].cur = 2;
			menu[curMenu].playDispose();
			SetStep(STEP_TITLE);
			break;
		default:
			SelectBGM(menu[curMenu].cur);
			menu[curMenu].play();
			menu[curMenu].cur ++;
			break;
		}
	}
}

int QBMissilePanic::Idle()
{
	Clear();
	switch (Step()) {
	case STEP_INIT:
		key_disp = true;
		stage = 0;
		hi_score = GetParam("PPMisslePanicHIScore",100);
		stage = GetParam("PPMisslePanicStage",0);
		if (stage > 0) {
			menu[curMenu].cur = 1;
		} else {
			menu[curMenu].cur = 0;
		}
		if (stage >= FINAL_STAGE-1) stage = FINAL_STAGE-1;
		SetStep(STEP_TITLE);
		break;
	case STEP_TITLE_INIT:
		StopBGM();
		doTitle(0);
		count ++;
		if (count > 30) SetStep(STEP_TITLE);
		if (TouchCount() == 0 && touchChange != TouchCount()) {
			SetStep(STEP_TITLE);
		}
		break;
	case STEP_TITLE:
		bigKeyArea = false;
		doTitle(1);
		break;
	case STEP_SETTING:
		doSetting();
		break;
	case STEP_START:
		bigKeyArea = true;
		StartGame();
		PlayBGM(1);
		NextStep();
		count = 0;
		break;
	case STEP_STAGE_START:
		bigKeyArea = true;
		CalcScreenPos();
		DrawBG();
		DrawObj();
		//DrawFlash();
		DrawInfo();

		Color(15);

		if (stage >= FINAL_STAGE-1) {
			Offset((320-13*16)/2,(12*32-16)/2-64);
			Print("FINAL MISSION");
		} else {
			Offset((320-10*16)/2,(12*32-16)/2-64);
			Print("MISSION %2d",stage+1);
		}

		Offset((320-6*16)/2,(12*32-16)/2+64);
		Print("READY!");

		count ++;
		if (count > 64*2) {
			NextStep();
		}
		break;
	case STEP_GAME:
		play_time --;
		if (play_time < 0) play_time = 0;
		MoveMissle();
		MoveObj();
		CalcScreenPos();
		DrawBG();
		DrawObj();
		//DrawFlash();
		DrawInfo();
		if (target_count <= 0) {
			key_wait = 0;
			SetStep(STEP_CLEAR);
			Stop();
			Play(SD_BIG_BANG);
			SetParam("PPMisslePanicStage",stage+1);
			StopBGM();
		} else
		if (missile_energy <= 0 || play_time <= 0) {
			missile_energy = 0;
			key_wait = 0;
			SetStep(STEP_GAMEOVER_MESSAGE);
			Stop();
			Play(SD_BANG);
			StopBGM();
		}
		break;
	case STEP_GAMEOVER:
		DrawBG();
		DrawObj();
		DrawBang();
		DrawInfo();
		if (bang_count > 32) {
			SetStep(STEP_GAMEOVER_MESSAGE);
		}
		break;
	case STEP_GAMEOVER_MESSAGE:
		DrawBG();
		DrawObj();
		DrawInfo();
		Offset((320-9*16)/2,(12*32-16)/2-32);
		Color(12);
		if (play_time <= 0) {
			Print("TIME OVER");
		} else {
			Print("GAME OVER");
		}
		if (key_wait < 16) key_wait ++;
		if (key_wait >= 16) {
			if (KeyWait(0xff) || (TouchCount() > 0 && touchChange != TouchCount())) {
				menu[curMenu].cur = 1;
				count = 0;
				SetStep(STEP_TITLE_INIT);
/*
				stage = GetParam("PPMisslePanicStage",0);
				if (stage > 0) {
					menu[curMenu].cur = 1;
				} else {
					menu[curMenu].cur = 0;
				}
*/
			}
		}
		break;
	case STEP_CLEAR:
		DrawBG();
		DrawObj();
		DrawBang();
		DrawInfo();
		if (bang_count > 32) {
			SetStep(STEP_CLEAR_MESSAGE);
			Play(SD_PI);
			PlayBGM(2,false);
		}
		break;
	case STEP_CLEAR_MESSAGE:
		StageClear();
		break;
	case STEP_ENDING:
		key_disp = false;
		{
			for (int y=0;y<16;y++) {
				for (int x=0;x<10;x++) {
					Put(x*32,y*32,spaceTable[y % 12][x]);
				}
			}
		}
		Color(14);
		Offset((320-15*16)/2,(12*32-16)/2-32-64);
		Print("CONGRATULATION!");
		Color(15);
		Offset((320-13*16)/2,(12*32-16)/2+32-64);
		Print("YOU DESTROYED");
		Offset((320-18*16)/2,(12*32-16)/2+32+32-64);
		Print("ALL ENEMY'S BASES.");
		Offset((320-6*16)/2,(12*32-16)/2+32+32+32-64);
		Print("GREAT!");

		Color(15);
		Offset((320-12*16)/2,(12*32-16)/2+32+32+32-64+48+32);
		Print("PRESENTED BY");
		Color(12);
		Offset((320-8*16)/2,(12*32-16)/2+32+32+32-64+48+32+32);
		Print("YAMAGAME");
		if (key_wait < 16) key_wait ++;
		if (key_wait >= 16) {
			if (TouchCount() > 0 && touchChange != TouchCount()) {
				key_disp = true;
				menu[curMenu].cur = 1;
				if (stage >= FINAL_STAGE-1) stage = FINAL_STAGE-1;
				count = 0;
				SetStep(STEP_TITLE_INIT);
/*
				stage = GetParam("PPMisslePanicStage",0);
				if (stage > 0) {
					menu[curMenu].cur = 1;
				} else {
					menu[curMenu].cur = 0;
				}
*/
			}
		}
		break;
	}
	
	touchChange = TouchCount();
	
	return 0;
}

void QBMissilePanic::FontPut(int x,int y,int pat,int scale)
{
	int i = polyCount;
	unsigned char r,g,b;
	
	GetRGBColor(color,&r,&g,&b);

	poly[i].r = r;
	poly[i].g = g;
	poly[i].b = b;
	poly[i].alpha = 255;
	poly[i++].splite(x,y,pat,PG_FONT2,QBCHAR_FONT2,2);
	
	polyCount++;
}

void QBMissilePanic::PutHalf(int x,int y,int pat)
{
	int i = polyCount;
	unsigned char r,g,b;
	
	GetRGBColor(color,&r,&g,&b);

	poly[i].r = r;
	poly[i].g = g;
	poly[i].b = b;
	poly[i].alpha = 255;
	poly[i++].splite(x,y,pat,0,QBCHAR_MISSILEPANIC,0.5);
	
	polyCount++;
}

void QBMissilePanic::TPrint(const char* format,...)
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

void QBMissilePanic::DrawScore()
{
	char str[256];
	sprintf(str,"SCORE %d0",score);
	Offset((320-strlen(str)*16)/2,16);
	Color(15);
	Print(str);
}

void QBMissilePanic::DrawHiScore()
{
	char str[256];
	sprintf(str,"HI-SCORE %d0",hi_score);
	Offset((320-strlen(str)*16)/2,0);
	Color(15);
	Print(str);
}

void QBMissilePanic::StageClear()
{
	//DrawBG();
	//DrawObj();
	//DrawInfo();
	{
		for (int y=0;y<12;y++) {
			for (int x=0;x<10;x++) {
				Put(x*32,y*32,spaceTable[y][x]);
			}
		}
	}
	if (key_wait < 3000) key_wait ++;

	DrawHiScore();
	DrawScore();

//	Offset((320-11*16)/2,(12*32-16)/2-32);
	Color(14);
//	Print("MISSION %2d CLEAR!",);
//	Color(15);
	if (stage+1 > FINAL_STAGE-1) {
		Offset((320-20*16)/2,(12*32-16)/2-32-32);
		Print("FINAL MISSION CLEAR!");
	} else {
		Offset((320-16*16)/2,(12*32-16)/2-32-32);
		Print("MISSION %2d CLEAR!",stage+1);
	}

	if (key_wait == 32*2) Play(SD_PI);
	if (key_wait >= 32*2) {
		Color(15);
		Offset((320-10*16)/2,(12*32-16)/2+16);
		Print("TIME BONUS");
	}

	if (key_wait == 32*4) {
		Play(SD_PI);
		score += play_time;
		if (hi_score < score) {
			hi_score = score;
			SetParam("PPMisslePanicHIScore",hi_score);
		}
	}
	if (key_wait >= 32*4) {
		Color(12);
		Offset((320-4*16)/2,(12*32-16)/2+64);
		Print("%4d0",play_time);
	}

	if (key_wait == 32*6) Play(SD_PI);
	if (key_wait >= 32*6) {
		Color(15);
		if (stage+1 > FINAL_STAGE-1) {
		} else {
			Offset((320-10*16)/2,(12*32-16)/2+64+64);
			Print("GOTO NEXT !",stage+1);
		}
	}
	if (key_wait >= 32*5) {
		if (KeyWait(0xff) || (TouchCount() > 0 && touchChange != TouchCount())) {
			stage ++;
			if (stage > FINAL_STAGE-1) {
				stage = FINAL_STAGE-1;
				SetStep(STEP_ENDING);
				PlayBGM(3,false);
			} else {
				SetStep(STEP_START);
			}
		}
	} else
	if (key_wait >= 16) {
		if (KeyWait(0xff) || (TouchCount() > 0 && touchChange != TouchCount())) {
			key_wait = 32*5;
		}
	}
}

int QBMissilePanic::Step()
{
	return step;
}

int QBMissilePanic::SetStep(int istep)
{
	step = istep;
	return 0;
}

int QBMissilePanic::NextStep()
{
	SetStep(Step()+1);
	return 0;
}

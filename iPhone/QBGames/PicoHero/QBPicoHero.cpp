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

#include "QBPicoHero.h"

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

typedef struct _enemy {
	int eds,edd,edl,ede;
} enemy;

static enemy local_enemy_d[] = {
	//	攻撃		防御		ライフ		経験値
	{	1,		0,		3,			1},		//グリーンスライム

	{	3,		0,		30,			3},		//スネーク
	{	5,		0,		50,			5},		//レッドスライム
	{	7,		0,		70,			7},		//ラット

	{	1,		1,		5,			1},		//バット
	{	2,		2,		10,			2},		//オクトパス

	{	13,		2,		130,		13},	//マッシュルーム
	{	15,		4,		150,		15},	//オーク
	{	17,		6,		300,		17},	//シャドーエッグ

	{	5,		3,		24,			4},		//ゾンビ

	{	21,		0,		210,		21},	//オークL2
	{	23,		2,		230,		23},	//ウィザード
	{	25,		4,		250,		25},	//スケルトン
	{	27,		6,		270,		27},	//ドラキュラ

	{	10,		5,		50,			16},	//ファイヤー

	{	31,		0,		410,		31},	//キングオーク
	{	33,		2,		330,		33},	//スカルナイト
	{	35,		4,		350,		35},	//デビル
	{	37,		6,		370,		37},	//デス

	{	10,		5,		140,		0},		//シャドー

	{	250,	0,		15000,		0},		//ドラゴン

	{	3,		0,		30,			3},		//Gスネーク
	{	11,		0,		110,		11},	//Gオクトパス
	{	25,		4,		250,		25},	//Gスケルトン
};
	
bool QBPicoHero::ScrollMap()
{
	return true;
}

bool QBPicoHero::ShadowEnding()
{
	return true;
}

const char* QBPicoHero::MapDataName()
{
	return "QBPicoHero.plist";
}

const char* QBPicoHero::SaveDataName()
{
	return NULL;
}

void QBPicoHero::DrawItemIcon()
{
	for (int i=0;i<QBSHADOW_ITEM_MAX;i++) {
		if (i > 4) {
			Put(i * 16 + itemx-16, itemy, (it[i] == 1) * (i + P_SHIELD));
		} else
		if (i >= 3 && i <= 4) {
		} else {
			Put(i * 16 + itemx+16, itemy, (it[i] == 1) * (i + P_SHIELD));
		}
	}
}

void QBPicoHero::InitPlayer()
{
	pmax = 10; plife = pmax; 
	//pstr = 100; pdef = 100; 
	pstr = 1; pdef = 0; 
	pexp = 0; pnexp = 5;
}

void QBPicoHero::LevelUp()
{
	level ++;
	pmax = pmax + 10;
	pstr = pstr + 1;
	pdef = pdef + 1;
	pnexp = pnexp * 2;
	Play("t255l16v32o5cceegg");
}

void* QBPicoHero::EnemyData()
{
	return local_enemy_d;
}

bool QBPicoHero::Continue()
{
	return false;
}

void QBPicoHero::InitBGMSetting()
{
	InitBGM(1,"shadowBusterLiteOverWorld");
	InitBGM(2,"shadowBusterLiteEnding");
}

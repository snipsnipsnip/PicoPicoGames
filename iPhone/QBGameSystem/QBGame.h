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

#pragma once

#include "PPGamePoly.h"
#include "PPGameDef.h"
#include "PPGameMapData.h"
#include "PPGameUtil.h"
#include "QBMenu.h"
#include "QBUtil.h"
#include "PPGameText.h"

enum {
	QBGAME_CONTROLLER_NORMAL,
	QBGAME_CONTROLLER_HIDE,
};

enum {
	QBGAME_LOCALE_OTHER,
	QBGAME_LOCALE_JAPANESE
};

enum {
	QBGAME_CONTROLLER_TYPE_VERTICAL_CENTER = 0,
	QBGAME_CONTROLLER_TYPE_VERTICAL,
	QBGAME_CONTROLLER_TYPE_HORIZONTAL,

	QBGAME_CONTROLLER_TYPE_V_LEFT,
	QBGAME_CONTROLLER_TYPE_V_RIGHT,
	QBGAME_CONTROLLER_TYPE_H_LEFT,
	QBGAME_CONTROLLER_TYPE_H_RIGHT,
};

enum {
	QBGAME_CONTROLLER_ENABLE_ARROW = 0x01,
	QBGAME_CONTROLLER_ENABLE_A = 0x02,
	QBGAME_CONTROLLER_ENABLE_B = 0x04,
};

#define QBGAME_MAX_MENU 10
#define QBGAME_MAX_TOUCH 5
#define QBGAME_MAX_COLORTABLE 256

class QBGame {
public:
	QBGame();
	virtual ~QBGame();
	
	virtual PPGameTextureInfo* TextureInfo();		//使用するテクスチャの情報 オーバーライドして使う//
	virtual int Idle();								//ゲームのアイドル オーバーライドして使う//
	virtual void Exit();							//ゲーム終了//

	//ゲーム固有のintパラメータのSave/Load//
	virtual int GetParam(const char* key,int defaultValue=0);
	virtual void SetParam(const char* key,int value);

	//ゲーム固有のバイナリデータのSave/Load//
	virtual unsigned char* GetData(const char* key,int* dataSize);
	virtual void SetData(const char* key,unsigned char* data,int dataSize);

	//キー入力関連//
	virtual bool KeyWait(unsigned long mask,bool releaseHit=false);//ボタン入力待ち//
	virtual void RepeatKey(int firstDelay=20,int secondDelay=15);
	virtual bool Key(unsigned long mask);			//ボタン入力判定//
	virtual unsigned long GetKey();					//ボタン入力//
	virtual void SetKey(unsigned long key);
	
	//文字描画関連//
	virtual void Clear();							//画面クリア//
	virtual void Color(int color);					//文字の色を指定//
	virtual void Locate(int x,int y);				//文字の位置をキャラクタ単位で指定//
	virtual void Offset(int x,int y);				//文字の位置をドット単位で指定//
	virtual void Print(const char* format,...);		//文字描画//

	//文字の描画 オーバーライドして使う//
	virtual void PutFont(int x,int y,int pat,unsigned char r=0xff,unsigned char g=0xff,unsigned char b=0xff,unsigned char a=0xff);

	//色の取得/設定//
	unsigned char* GetRGBColor(int color,unsigned char* r,unsigned char* g,unsigned char* b);
	int SetRGBColor(int color,unsigned char r,unsigned char g,unsigned char b);
	
	//パターンの描画//
	virtual void Put(int x,int y,int pat,int group,int texture,unsigned char r=0xff,unsigned char g=0xff,unsigned char b=0xff,unsigned char a=0xff);

	//図形の描画//
	virtual void Line(int x1,int y1,int x2,int y2,int color);
	virtual void Fill(int x,int y,int width,int height,int color);
	virtual void Box(int x,int y,int width,int height,int color);

	//ローカライズ//
	virtual int Locale();							//日本語(QBGAME_LOCALE_JAPANESE)、それ以外(QBGAME_LOCALE_OTHER)//
	
	//画面のタッチ判定//
	int TouchCount() {return touchCount;}			//タッチ位置の取得//
	int TouchXPosition(int i) {return touchX[i];}	//タッチ位置(X座標)//
	int TouchYPosition(int i) {return touchY[i];}	//タッチ位置(Y座標)//

	//効果音関連//
	void Play(const char* mml,int ch=0);			//サウンドのMML再生//
	void Stop(int ch=0);							//サウンドの停止//
	bool IsPlaying(int ch=0);						//サウンド再生中か？//

	//BGM関連//
	int InitBGM(int no,const char* key);			//BGMの初期化//
	void PlayBGM(int no,bool repeat=true);			//BGMの再生//
	void StopBGM();									//BGMの停止//
	void SelectBGM(int no);							//BGMの選択//
	void ResetBGM();								//BGMの初期化//

	//画面設定 オーバーライドして使う//
	virtual unsigned long WindowLayout();			//画面の方向とボタンの配置//
	virtual unsigned long VisibleButton();			//表示するボタン//

	//メニュー//
	int curMenu;									//アクティブなメニュー//
	QBMenu menu[QBGAME_MAX_MENU];					//メニュー//

	//マップデータ//
	virtual bool LoadMap(const char* mapname);		//マップの読み込み//
	PPGameMapData* GetMapData(int layer) {return map[layer];}

	//描画系パラメータ//
	int locatex,locatey;							//文字描画位置//
	int color;										//描画カラー (16色)//
	int polyCount;									//描画ポリゴン格納数//
	PPGamePoly* poly;								//描画ポリゴン格納バッファ//

	//その他設定 オーバーライドして使う//
	virtual int KeyTextureID() {return -1;}			//十字キーテクスチャのID//
	virtual int KeySize();							//十字キーの当たり範囲 64*64:小 96*96:大//
	virtual int KeyCount();							//4方向 or 8方向//

	//システムフォントの描画 iPhoneでのみ有効//
	virtual int StringTextureID() {return -1;}		//文字テクスチャ(システムフォント)のID オーバーライドして使う//
	void String(const char* format,...);			//文字の描画(システムフォント)//
	
	/* 以下、使わないで */
	int gameIdle(unsigned long key,PPGamePoly* poly,PPGameSplite* g,void* controller);
	void releaseBGM();
	void idleBGM(void* controller);
	void setTouchCount(int count) {touchCount = count;}
	void setTouchPosition(int x,int y) {touchX[touchCount]=x;touchY[touchCount]=y;touchCount++;}
	PPGameText* text;
	bool exitGame;

private:
	int touchX[QBGAME_MAX_TOUCH];
	int touchY[QBGAME_MAX_TOUCH];
	int touchCount;

	int chooseBGM;									//BGMの選択//
	int playBGM;									//BGMの再生(>=0)/ストップ(-1)//
	bool playBGMOneTime;							//BGMのループ指定(true:1 false:ループ)//
	
	unsigned long key;
	unsigned long prekey;
	unsigned long keyRepeatTimer;
	unsigned char colortable[QBGAME_MAX_COLORTABLE][4];

	PPGameMapData* map[3];							//マップデータ//
};

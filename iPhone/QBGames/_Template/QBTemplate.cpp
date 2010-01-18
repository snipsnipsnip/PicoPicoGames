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

#include "QBTemplate.h"
#include <stdio.h>
#include <stdlib.h>
#include "font.h"
#include "font2.h"
#include "key.h"

enum {
	QBCHAR_FONT,
	QBCHAR_FONT2,
	QBCHAR_KEY,
	QBCHAR_TEMPLATE,
};

static PPGameTextureInfo texList[] = {
	{"font.png",font_group,false},		//QBCHAR_FONT
	{"font2.png",font2_group,false},	//QBCHAR_FONT2
	{"key.png",key_group,false},		//QBCHAR_KEY
	{"QBTemplate.png",NULL,false},		//QBCHAR_TEMPLATE
	{NULL,NULL,false}
};

QBTemplate::QBTemplate()
{
}

QBTemplate::~QBTemplate()
{
}

/* テクスチャ情報 */
PPGameTextureInfo* QBTemplate::TextureInfo()
{
	return texList;
}

/* タイトルの描画 */
void QBTemplate::drawTitle()
{
	Color(12);
	Offset(0+4+24,3*16+4);
	Print2("TEMPLATE");
	Color(15);
	Offset(0-4+4+24,3*16+4-4);
	Print2("TEMPLATE");
}

/* ゲーム起動時の初期化 */
void QBTemplate::gameInit()
{
	InitBGM(1,"QBTemplatePlay");
	InitBGM(2,"QBTemplateEnding");
}

/* ゲーム開始時の初期化 */
void QBTemplate::gameStart()
{
}

/* ゲーム中の処理 */
int QBTemplate::gameIdle()
{
	return 0;
}

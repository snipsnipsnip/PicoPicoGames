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

#include "QBGrafSample.h"
#include "font2.h"
#include "key.h"

static PPGameTextureInfo texList[] = {
	{"font2.png",font2_group,false},
	{"key.png",key_group,false},
	{"MissilePanic.png",NULL,false},
	{"",NULL,true},
	{NULL,NULL,false}
};

QBGrafSample::QBGrafSample()
{
	y = 0;
}

PPGameTextureInfo* QBGrafSample::TextureInfo()
{
	return texList;
}

unsigned long QBGrafSample::WindowLayout()
{
	return QBGAME_CONTROLLER_TYPE_HORIZONTAL;
}

unsigned long QBGrafSample::VisibleButton()
{
	return 0;
}

void QBGrafSample::PutFont(int x,int y,int pat,unsigned char r,unsigned char g,unsigned char b)
{
	QBGame::Put(x,y,pat,PG_FONT2,QBCHAR_FONT2,r,g,b,0xff);
}

int QBGrafSample::Idle()
{
	Clear();

	for (int i=0;i<16;i++) {
		Fill(0,32+i*16-y,240,16,i);
		Box(0,32+i*16-y,240,16,15);
		Box(240,32+i*16-y,240,16,i);
		Line(240,32+i*16-y,240,16,i);
	}

	Offset(1,8+1);
	Color(12);
	Print("ABCDEFG");
	Offset(0,8);
	Color(15);
	Print("ABCDEFG");

	if (KeyWait(PAD_UP)) {
		y = 0;
	}

	if (KeyWait(PAD_DOWN)) {
		y = 10;
	}

	return 0;
}

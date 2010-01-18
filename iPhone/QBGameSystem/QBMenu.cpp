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

#include "QBMenu.h"
#include "QBGame.h"
#include "PPGameSound.h"

#include <string.h>

QBMenu::QBMenu()
{
	Init();
}

void QBMenu::Init()
{
	for (int i=0;i<QBMENU_MAX_LINE;i++) {
		string[i] = NULL;
	}
	offsetx = 0;
	offsety = 0;
	x = 0;
	y = 0;
	w = 0;
	h = 0;
	frameColor = 15;
	fillColor = 0;
	enable = true;

	touch = false;
	pretouch = false;
	select = false;
	cur = 0;
	precur = 0;
	
	touchOK = true;
}

QBMenu::~QBMenu()
{
}

int QBMenu::draw(QBGame* game)
{
	disp = true;
	if (fillColor >= 0) {
		game->Fill(x*8*2+offsetx,y*8*2+offsety,w*8*2,h*8*2,fillColor);
	}
	if (frameColor >= 0) {
		game->Box(x*8*2+offsetx,y*8*2+offsety,w*8*2,h*8*2,frameColor);
	}
	for (int i=0;string[i]!=NULL;i++) {
		if (i == cur) {
			if (enable) {
				game->Color(12);
			} else {
				game->Color(7);
			}
			game->Offset((x+1)*16+offsetx,(i*3+y+1)*16+offsety);
			game->Print(">");
		} else {
			if (enable) {
				game->Color(15);
			} else {
				game->Color(8);
			}
		}
		game->Offset((x+2+1)*16+offsetx,(i*3+y+1)*16+offsety);
		game->Print(string[i]);
	}
	return 0;
}

bool QBMenu::touchRect(float* pos,int c)
{
	float px,py;
	float pw,ph;
	int n=0;
	
	px = x*8*2+offsetx;
	py = y*8*2+c*3*8*2+offsety;
	pw = w*8*2;
	ph = 3*8*2;//h*8*2;

	pos[n++] = px;
	pos[n++] = py;

	pos[n++] = px+pw;
	pos[n++] = py;
	
	pos[n++] = px+pw;
	pos[n++] = py+ph;

	pos[n++] = px;
	pos[n++] = py;
	
	pos[n++] = px;
	pos[n++] = py;

	pos[n++] = px+pw;
	pos[n++] = py+ph;

	pos[n++] = px;
	pos[n++] = py+ph;
	
	pos[n++] = px;
	pos[n++] = py;
	
	return false;
}

void QBMenu::play()
{
	if (PPGame_GetSoundEnable()) PPGameMML_Play("t255l64M0o5v32dcr",0);
	//PPGameMML_Play("t255l32M0v32o5cd",0);
}

void QBMenu::playDispose()
{
	if (PPGame_GetSoundEnable()) PPGameMML_Play("t255l64M0v32o2cdefedco2cdefffg",0);
}

void QBMenu::up()
{
	int i=0;
	for (i=0;string[i]!=NULL;i++) ;
	cur -= 1;
	if (cur < 0) cur = i-1;
	play();
}

void QBMenu::down()
{
	int i=0;
	for (i=0;string[i]!=NULL;i++) ;
	cur += 1;
	if (cur >= i) cur = 0;
	play();
}

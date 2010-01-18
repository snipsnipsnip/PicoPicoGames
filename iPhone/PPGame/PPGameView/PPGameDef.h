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

#define QBFILL_TAG (-1)
#define QBLINE_TAG (-2)
#define QBFONT_TAG (-3)
#define QBCHAR_TAG (-4)
//#define QBPOLY_TAG (-5)

#define PPGAME_MAX_POLY 3000

typedef struct _PPGameImage {
	short* pat;
	short width;
	short height;
	short chipWidth;
	short texWidth;
	short texHeight;
} PPGameImage;

/*
typedef struct _QB_POLY {
	int type;
	int color;
	int x,y;
	float w,h;
	int cliph;
	int pat;
	int group;
	int blendType;
	float rot;
	float alpha;
} QB_POLY;
*/

typedef struct _PPGameTextureInfo {
	const char* name;
	PPGameImage* group;
	bool parge;
} PPGameTextureInfo;

enum {
	PPGameBlend_None,
	PPGameBlend_Light,
	PPGameBlend_BlackMask,
	PPGameBlend_Flash,
	PPGameBlend_Red,
	PPGameBlend_NoMask,
	PPGameBlend_Color
};

#define PAD_UP		0x01
#define PAD_DOWN	0x02
#define PAD_LEFT	0x04
#define PAD_RIGHT	0x08
#define PAD_A		0x0010
#define PAD_B		0x0020
#define PAD_ANY		(-1)

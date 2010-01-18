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

#include "PPGamePoly.h"
#include "PPGameDef.h"
#include <stdio.h>

void PPGamePoly::init()
{
	type = 0;
	x = 0;//*resolutionValue();
	y = 0;//*resolutionValue();
	w = 1.0;
	h = 1.0;
	cx = 0;
	cy = 0;
	pat = 0;
	r = 255;
	g = 255;
	b = 255;
	alpha = 255;
	cliph = -1;
	rot = 0;
	blend = PPGameBlend_None;
}

void PPGamePoly::splite(int _x,int _y,int _pat,int _group,int _type,float _scale)
{
	type = _type;
	x = _x;//*resolutionValue();
	y = _y;//*resolutionValue();
	pat = _pat;
//	color = 15;//_color;
//	r = 255;
//	g = 255;
//	b = 255;
	w = _scale;//_fscalex*scale;
	h = _scale;//_fscaley*scale;
//	cliph = -1;
//	rot = 0;//_rotate;
	group = _group;
//	alpha = 255;//_alpha;
//	blend = PPGameBlend_None;//_blendType;
// printf("x %d,y %d,type %d,pat %d,w %f,h %f,group %d\n",x,y,type,pat,w,h,group);
}

void PPGamePoly::line(int x1,int y1,int x2,int y2)
{
	type = QBLINE_TAG;
	x = x1;//*resolutionValue();
	y = y1;//*resolutionValue();
	w = x2;//*resolutionValue();
	h = y2;//*resolutionValue();
//	color = 15;
//	r = 255;
//	g = 255;
//	b = 255;
}

void PPGamePoly::fill(int x1,int y1,int x2,int y2)
{
	type = QBFILL_TAG;
	x = x1;//*resolutionValue();
	y = y1;//*resolutionValue();b
	w = x2;//*resolutionValue();
	h = y2;//*resolutionValue();
//	color = 15;
//	r = 255;
//	g = 255;
//	b = 255;
//	alpha = 255;
}

void PPGamePoly::color(unsigned char _r,unsigned char _g,unsigned char _b)
{
	r = _r;
	g = _g;
	b = _b;
}

/*
void PPGamePoly::alpha(unsigned char _alpha)
{
	alpha = _alpha;
}
*/

/*
void PPGamePoly::poly(int x1,int y1,int x2,int y2,int _x3,int _y3)
{
	type = QBPOLY_TAG;
	x = x1;//*resolutionValue();
	y = y1;//*resolutionValue();
	w = x2;//*resolutionValue();
	h = y2;//*resolutionValue();
	x3 = _x3;//*resolutionValue();
	y3 = _y3;//*resolutionValue();
}
*/

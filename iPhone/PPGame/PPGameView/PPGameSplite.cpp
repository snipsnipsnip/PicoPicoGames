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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "PPGameSplite.h"
#include "PPGameUtil.h"

#define VN 2

static GLubyte colorTable[][4] = {
	{0x00,0x00,0x00,0xff},	//0
	{0x00,0x00,0xa0,0xff},	//1
	{0x00,0xa0,0x00,0xff},	//2
	{0x00,0xa0,0xa0,0xff},	//3
	{0xa0,0x00,0x00,0xff},	//4
	{0xa0,0x00,0xa0,0xff},	//5
	{0xa0,0xa0,0x00,0xff},	//6
	{0xa0,0xa0,0xa0,0xff},	//7
	{0x40,0x40,0x40,0xff},	//8
	{0x00,0x00,0xff,0xff},	//9
	{0x00,0xff,0x00,0xff},	//10
	{0x00,0xff,0xff,0xff},	//11
	{0xff,0x00,0x00,0xff},	//12
	{0xff,0x00,0xff,0xff},	//13
	{0xff,0xff,0x00,0xff},	//14
	{0xff,0xff,0xff,0xff},	//15
	{0x00,0x00,0x00,0xff},	//16
	{0x00,0x00,0xff,0xff},	//17
	{0x00,0xff,0x00,0xff},	//18
	{0x00,0xff,0xff,0xff},	//19
	{0xff,0x00,0x00,0xff},	//20
	{0xff,0x00,0xff,0xff},	//21
	{0xff,0x80,0x00,0xff},	//22
	{0x80,0x80,0x80,0xff},	//23
	{0x40,0x40,0x40,0xff},	//24
	{0x40,0x40,0xff,0xff},	//25
	{0x40,0xff,0x40,0xff},	//26
	{0x40,0xff,0xff,0xff},	//27
	{0xff,0x40,0x40,0xff},	//28
	{0xff,0x40,0xff,0xff},	//29
	{0xff,0xff,0x40,0xff},	//30
	{0xff,0xff,0xff,0xff},	//31
	{0x00,0x20,0xff,0xff},	//32
	{0x00,0x40,0xff,0xff},	//33
	{0x00,  96,0xff,0xff},	//34
	{0x00,0x80,0xff,0xff},	//35
	{0x00, 160,0xff,0xff},	//36
	{0x00, 192,0xff,0xff},	//37
	{0x00, 224,0xff,0xff}	//38
};

#pragma mark -

PPGameTexture::PPGameTexture()
{
	pixel = NULL;
	bind = false;
	nofree = false;
	group = NULL;
	name = NULL;
}

PPGameTexture::~PPGameTexture()
{
	if (!nofree) {
		if (pixel) free(pixel);
	}
}

#pragma mark -

PPGameSplite::PPGameSplite()
{
	patternMaster = NULL;
	for (int i=0;i<38;i++) {
		for (int j=0;j<4;j++) {
			for (int k=0;k<4;k++) {
				colors[i][j*4+k] = colorTable[i][k];
			}
		}
	}
	for (int i=0;i<PPGAME_MAX_TEXTURE;i++) {
		texture[i] = NULL;
	}
	_sx = 1;
	_sy = 1;
	_alpha = 1.0;
	_rotx = 0;
	_roty = 0;
	_rot = 0;
	_blendType = PPGameBlend_None;
	screenWidth = 320;
	screenHeight = 200;
	b_ptr = 0;
	b_stack_ptr = 0;
	b_start_ptr = 0;
	s_type = 1000;
	s_texId = 1000;
	s_alphaValue = 1.0;
	s_blendType = 1;
	_offsetX = 0;
	_offsetY = 0;
	initLoadTex = false;
	reloadTex = false;
	b_ptr=0;
}

PPGameSplite::~PPGameSplite()
{
	for (int i=0;i<PPGAME_MAX_TEXTURE;i++) {
		if (texture[i]) delete texture[i];
		texture[i] = NULL;
	}
}

int PPGameSplite::Init()
{
	return 0;
}

int PPGameSplite::Exit()
{
	return 0;
}

int PPGameSplite::RegistTexture(unsigned char* pixel,int width,int height,int rowbytes,bool nofree)
{
	for (int i=0;i<PPGAME_MAX_TEXTURE;i++) {
		if (texture[i]==NULL) {
			texture[i] = new PPGameTexture();
			if (texture[i]) {
				texture[i]->pixel = pixel;
				texture[i]->width = width;
				texture[i]->height = height;
				texture[i]->rowbytes = rowbytes;
				texture[i]->bind = false;
				texture[i]->nofree = nofree;
//printf("RegistTexture %d,%d\n",texture[i]->width,texture[i]->height);
				return i;
			}
			break;
		}
	}
	return -1;
}

int PPGameSplite::UnbindAllTexture()
{
	for (int i=0;i<PPGAME_MAX_TEXTURE;i++) {
		if (texture[i]) {
			if (texture[i]->bind == false) {
			} else {
				glDeleteTextures(1,&texture[i]->texid);
			}
			texture[i]->bind = false;
		}
	}
	return 0;
}

int PPGameSplite::ReloadAllTexture()
{
	for (int i=0;i<PPGAME_MAX_TEXTURE;i++) {
		if (texture[i]) {
			if (texture[i]->bind == false) {
			} else {
				glDeleteTextures(1,&texture[i]->texid);
				texture[i]->bind = false;
				LoadTexture(i);
			}
		}
	}
	return 0;
}

int PPGameSplite::BindTexture(int textureid)
{
	int i=textureid;
	if (i >= 0) {
		if (texture[i]) {
			if (texture[i]->bind == false) {
				glGenTextures(1,&texture[i]->texid);
				glBindTexture(GL_TEXTURE_2D,texture[i]->texid);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,texture[i]->width,texture[i]->height,0,GL_RGBA,GL_UNSIGNED_BYTE,texture[i]->pixel);
				texture[i]->bind = true;
//printf("texture[%d]->texid=%d\n",i,texture[i]->texid);
			} else {
				glBindTexture(GL_TEXTURE_2D,texture[i]->texid);
			}
			texId = i;
		}
	}
	return 0;
}

int PPGameSplite::DeleteTexture(int textureid)
{
	int i=textureid;
	if (i >= 0) {
		if (texture[i]) {
			if (texture[i]->bind) {
				glDeleteTextures(1,&texture[i]->texid);
				texture[i]->bind = false;
			}
			delete texture[i];
			texture[i] = NULL;
		}
	}
	return 0;
}

int PPGameSplite::InitOT()
{
	glEnable(GL_TEXTURE_2D);
	ResetTextureState();
	BlendOff();
	return 0;
}

int PPGameSplite::MoveTo(int x,int y)
{
	_x = x;
	_y = y;
	return 0;
}

int PPGameSplite::Rot(float x,float y,float rot)
{
	_rotx = x;
	_roty = y;
	_rot = rot*2*M_PI/360;
	return 0;
}

/*
int PPGameSplite::DrawCore(int textureid,int px,int py,int pw,int ph,int dx,int dy,int col,int height)
{
	if (texture[textureid]) {
		
		float x1,y1,x2,y2;
		float x3,y3,x4,y4;
		GLfloat u1,v1,u2,v2;
		
		GLfloat* vert = &b_vert[b_ptr*VN];
		GLfloat* uv = &b_uv[b_ptr*2];
		GLubyte* color = &b_color[b_ptr*4];
		GLushort* index = &b_index[b_ptr*VN];
		//b_ptr ++;
		
		int hh = height;
		if (hh < 0) {
			hh = 0;
		} else {
			hh = hh-(dy+ph*_sy);
			if (hh > 0) hh = 0;
			if (ph < -hh) hh = -ph;
		}
		
		float p1x,p1y,p2x,p2y;
		float p3x,p3y,p4x,p4y;
		
		p1x = _x+dx+_offsetX;
		p1y = _y+dy+_offsetY;

		p2x = _x+dx+pw*_sx+_offsetX;
		p2y = _y+dy+_offsetY;

		p3x = _x+dx+pw*_sx+_offsetX;
		p3y = _y+dy+ph*_sy+hh*_sy+_offsetY;
		
		p4x = _x+dx+_offsetX;
		p4y = _y+dy+ph*_sy+hh*_sy+_offsetY;
		
		if (_rot != 0) {
			p1x = p1x-_rotx;
			p1y = p1y-_roty;
			p2x = p2x-_rotx;
			p2y = p2y-_roty;
			p3x = p3x-_rotx;
			p3y = p3y-_roty;
			p4x = p4x-_rotx;
			p4y = p4y-_roty;
			
			GLfloat x,y;
			x = p1x*cos(_rot)-p1y*sin(_rot);
			y = p1x*sin(_rot)+p1y*cos(_rot);
			p1x = x+_rotx;
			p1y = y+_roty;

			x = p2x*cos(_rot)-p2y*sin(_rot);
			y = p2x*sin(_rot)+p2y*cos(_rot);
			p2x = x+_rotx;
			p2y = y+_roty;

			x = p3x*cos(_rot)-p3y*sin(_rot);
			y = p3x*sin(_rot)+p3y*cos(_rot);
			p3x = x+_rotx;
			p3y = y+_roty;

			x = p4x*cos(_rot)-p4y*sin(_rot);
			y = p4x*sin(_rot)+p4y*cos(_rot);
			p4x = x+_rotx;
			p4y = y+_roty;
		}

		x1 = (p1x)/(screenWidth/2)-1.0f;
		y1 = -(p1y)/(screenHeight/2)+1.0f;

		x2 = (p2x)/(screenWidth/2)-1.0f;
		y2 = -(p2y)/(screenHeight/2)+1.0f;

		x3 = (p3x)/(screenWidth/2)-1.0f;
		y3 = -(p3y)/(screenHeight/2)+1.0f;

		x4 = (p4x)/(screenWidth/2)-1.0f;
		y4 = -(p4y)/(screenHeight/2)+1.0f;

		float tw = texture[textureid]->width;
		float th = texture[textureid]->height;
		float dd = 0;//tw / 512.0 * 0.0005;
		if (tw > 0 && th > 0) {
			u1 = (px)/tw;
			v1 = (py)/th;
			u2 = (px+pw)/tw;
			v2 = (py+ph+hh)/th;

			{
				int p=0;
				vert[p++] = x1;
				vert[p++] = y1;
//				vert[p++] = 0;
				vert[p++] = x2;
				vert[p++] = y2;
//				vert[p++] = 0;
				vert[p++] = x4;
				vert[p++] = y4;
//				vert[p++] = 0;

				vert[p++] = x2;
				vert[p++] = y2;
//				vert[p++] = 0;
				vert[p++] = x4;
				vert[p++] = y4;
//				vert[p++] = 0;
				vert[p++] = x3;
				vert[p++] = y3;
//				vert[p++] = 0;
			}
			
			{
				int p=0;
				index[p++] = b_ptr+0;
				index[p++] = b_ptr+1;
				index[p++] = b_ptr+2;
				index[p++] = b_ptr+3;
				index[p++] = b_ptr+4;
				index[p++] = b_ptr+5;
			}

			{
				int p=0;
				uv[p++] = u1+dd;
				uv[p++] = v1+dd;
				uv[p++] = u2-dd;
				uv[p++] = v1+dd;
				uv[p++] = u1+dd;
				uv[p++] = v2-dd;

				uv[p++] = u2-dd;
				uv[p++] = v1+dd;
				uv[p++] = u1+dd;
				uv[p++] = v2-dd;
				uv[p++] = u2-dd;
				uv[p++] = v2-dd;
			}

			{
				colors[col][3] = _alpha*255;
				colors[col][7] = _alpha*255;
				colors[col][11] = _alpha*255;
				colors[col][15] = _alpha*255;
				GLubyte* b = &colors[col][0];
				for (int i=0;i<6*4;i++) {
					color[i] = b[i % 4];
				}
			}
			
		}
	}
	return 0;
}
*/

int PPGameSplite::DrawCore(PPGamePoly* poly,int px,int py,int pw,int ph,int dx,int dy)
{
	int textureid = poly->type;
	if (texture[textureid]) {
		
		float x1,y1,x2,y2;
		float x3,y3,x4,y4;
		GLfloat u1,v1,u2,v2;
		
		GLfloat* vert = &b_vert[b_ptr*VN];
		GLfloat* uv = &b_uv[b_ptr*2];
		GLubyte* color = &b_color[b_ptr*4];
		
		int hh = poly->cliph;
		if (hh < 0) {
			hh = 0;
		} else {
			hh = hh-(dy+ph*_sy);
			if (hh > 0) hh = 0;
			if (ph < -hh) hh = -ph;
		}

		float p1x,p1y,p2x,p2y;
		float p3x,p3y,p4x,p4y;
		
		p1x = _x+dx+_offsetX;
		p1y = _y+dy+_offsetY;

		p2x = _x+dx+pw*_sx+_offsetX;
		p2y = _y+dy+_offsetY;

		p3x = _x+dx+pw*_sx+_offsetX;
		p3y = _y+dy+ph*_sy+hh*_sy+_offsetY;
		
		p4x = _x+dx+_offsetX;
		p4y = _y+dy+ph*_sy+hh*_sy+_offsetY;
		
		if (_rot != 0) {
			p1x = p1x-_rotx;
			p1y = p1y-_roty;
			p2x = p2x-_rotx;
			p2y = p2y-_roty;
			p3x = p3x-_rotx;
			p3y = p3y-_roty;
			p4x = p4x-_rotx;
			p4y = p4y-_roty;
			
			GLfloat x,y;
			x = p1x*cos(_rot)-p1y*sin(_rot);
			y = p1x*sin(_rot)+p1y*cos(_rot);
			p1x = x+_rotx;
			p1y = y+_roty;

			x = p2x*cos(_rot)-p2y*sin(_rot);
			y = p2x*sin(_rot)+p2y*cos(_rot);
			p2x = x+_rotx;
			p2y = y+_roty;

			x = p3x*cos(_rot)-p3y*sin(_rot);
			y = p3x*sin(_rot)+p3y*cos(_rot);
			p3x = x+_rotx;
			p3y = y+_roty;

			x = p4x*cos(_rot)-p4y*sin(_rot);
			y = p4x*sin(_rot)+p4y*cos(_rot);
			p4x = x+_rotx;
			p4y = y+_roty;
		}

		x1 = (p1x)/(screenWidth/2)-1.0f;
		y1 = -(p1y)/(screenHeight/2)+1.0f;

		x2 = (p2x)/(screenWidth/2)-1.0f;
		y2 = -(p2y)/(screenHeight/2)+1.0f;

		x3 = (p3x)/(screenWidth/2)-1.0f;
		y3 = -(p3y)/(screenHeight/2)+1.0f;

		x4 = (p4x)/(screenWidth/2)-1.0f;
		y4 = -(p4y)/(screenHeight/2)+1.0f;

		float tw = texture[textureid]->width;
		float th = texture[textureid]->height;
		float dd = 0;//tw / 512.0 * 0.0005;
		if (tw > 0 && th > 0) {
			u1 = (px)/tw;
			v1 = (py)/th;
			u2 = (px+pw)/tw;
			v2 = (py+ph+hh)/th;

			{
				int p=0;
				vert[p++] = x1;
				vert[p++] = y1;
//				vert[p++] = 0;
				vert[p++] = x2;
				vert[p++] = y2;
//				vert[p++] = 0;
				vert[p++] = x4;
				vert[p++] = y4;
//				vert[p++] = 0;

				vert[p++] = x2;
				vert[p++] = y2;
//				vert[p++] = 0;
				vert[p++] = x4;
				vert[p++] = y4;
//				vert[p++] = 0;
				vert[p++] = x3;
				vert[p++] = y3;
//				vert[p++] = 0;
			}
			
			{
				int p=0;
				uv[p++] = u1+dd;
				uv[p++] = v1+dd;
				uv[p++] = u2-dd;
				uv[p++] = v1+dd;
				uv[p++] = u1+dd;
				uv[p++] = v2-dd;

				uv[p++] = u2-dd;
				uv[p++] = v1+dd;
				uv[p++] = u1+dd;
				uv[p++] = v2-dd;
				uv[p++] = u2-dd;
				uv[p++] = v2-dd;
			}

			{
				for (int i=0;i<6*4;i+=4) {
					color[i+0] = poly->r;
					color[i+1] = poly->g;
					color[i+2] = poly->b;
					color[i+3] = poly->alpha;
				}
			}
			
		}
	}
	return 0;
}

/*
int PPGameSplite::DrawTexture(int textureid,int dx,int dy,int col,int height)
{
	if (texture[textureid]) {
		int pw = texture[textureid]->width;
		int ph = texture[textureid]->height;
		return DrawCore(textureid,0,0,pw,ph,dx,dy,col,height);
	}
	return 0;
}
*/

/*
int PPGameSplite::Draw(int textureid,int groupid,int patternid,int col,int height)
{
	if (patternMaster) {
		int ppp=patternid;
		
		short* pat;
		pat = patternMaster[groupid].pat;
		
//		if (pat == NULL) return 0;

		int px = pat[ppp*9+7];
		int py = pat[ppp*9+8];
		int pw = pat[ppp*9+3];
		int ph = pat[ppp*9+4];

		int dx = pat[ppp*9+5];
		int dy = pat[ppp*9+6];
		
		return DrawCore(textureid,px,py,pw,ph,dx,dy,col,height);
	}
	return 0;
}
*/

int PPGameSplite::Draw(PPGamePoly* poly)
{
	if (patternMaster) 
	{
		int ppp=poly->pat;
		
		short* pat;
		pat = patternMaster[poly->group].pat;
		
//		if (pat == NULL) return 0;

		int px = pat[ppp*9+7];
		int py = pat[ppp*9+8];
		int pw = pat[ppp*9+3];
		int ph = pat[ppp*9+4];

		int dx = pat[ppp*9+5];
		int dy = pat[ppp*9+6];
		
		return DrawCore(poly,px,py,pw,ph,dx,dy);
	} else {
		int ppp=poly->pat;
		int textureid = poly->type;
		if (texture[textureid]) {
			int tw = texture[textureid]->width;
//			int th = texture[textureid]->height;
			
			int px = (ppp % (tw/32))*32;
			int py = (ppp / (tw/32))*32;
			int pw = 32;
			int ph = 32;

			int dx = 0;
			int dy = 0;

			return DrawCore(poly,px,py,pw,ph,dx,dy);
		}
	}
	return 0;
}

//static float t[10000];

/*
int PPGameSplite::Poly(float* points,unsigned char* colors,int num)
{
	for (int i=0;i<num*3;i+=3) {
		t[i+0] = (points[i+0]+_offsetX)/(screenWidth/2)-1.0f;
		t[i+1] = -(points[i+1]+_offsetY)/(screenHeight/2)+1.0f;
		t[i+2] = points[i+2];
	}
	glVertexPointer(3,GL_FLOAT,0,t);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColorPointer(4,GL_UNSIGNED_BYTE,0,colors);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glPushMatrix();
	glDrawArrays(GL_TRIANGLE_STRIP,0,num);
	glPopMatrix();
	return 0;
}
*/

/*
int PPGameSplite::Poly(PPGamePoly* poly)
{
	GLfloat x1,y1,x2,y2,x3,y3;
	int _x1,_y1,_x2,_y2,_x3,_y3;
	
	_x1 = poly->x;
	_y1 = poly->y;
	_x2 = poly->w;
	_y2 = poly->h;
	_x3 = poly->x3;
	_y3 = poly->y3;

	GLfloat* vert = &b_vert[b_ptr*VN];
	//GLfloat* uv = &b_uv[b_ptr*12];
	GLubyte* color = &b_color[b_ptr*4];
	//b_ptr ++;

	_x1 += _offsetX;
	_y1 += _offsetY;
	_x2 += _offsetX;
	_y2 += _offsetY;
	_x3 += _offsetX;
	_y3 += _offsetY;

	x1 = (_x1)/(screenWidth/2)-1.0f;
	y1 = -(_y1)/(screenHeight/2)+1.0f;
	x2 = (_x2)/(screenWidth/2)-1.0f;
	y2 = -(_y2)/(screenHeight/2)+1.0f;
	x3 = (_x3)/(screenWidth/2)-1.0f;
	y3 = -(_y3)/(screenHeight/2)+1.0f;

	{
		int p=0;
		vert[p++] = x1;
		vert[p++] = y1;
//		vert[p++] = 0;
		vert[p++] = x2;
		vert[p++] = y2;
//		vert[p++] = 0;
		vert[p++] = x3;
		vert[p++] = y3;
//		vert[p++] = 0;
	}

	{
		for (int i=0;i<3*4;i+=4) {
			color[i+0] = poly->r;
			color[i+1] = poly->g;
			color[i+2] = poly->b;
			color[i+3] = poly->alpha;
		}
	}

	return 0;
}
*/

/*
int PPGameSplite::Fill(int _x1,int _y1,int _x2,int _y2,int col)
{
	GLfloat x1,y1,x2,y2;

	GLfloat* vert = &b_vert[b_ptr*VN];
	//GLfloat* uv = &b_uv[b_ptr*12];
	GLubyte* color = &b_color[b_ptr*4];
	//b_ptr ++;

	_x1 += _offsetX;
	_y1 += _offsetY;
	_x2 += _offsetX;
	_y2 += _offsetY;

	x1 = (_x1)/(screenWidth/2)-1.0f;
	y1 = -(_y1)/(screenHeight/2)+1.0f;
	x2 = (_x2)/(screenWidth/2)-1.0f;
	y2 = -(_y2)/(screenHeight/2)+1.0f;

	{
		int p=0;
		vert[p++] = x1;
		vert[p++] = y1;
//		vert[p++] = 0;
		vert[p++] = x2;
		vert[p++] = y1;
//		vert[p++] = 0;
		vert[p++] = x1;
		vert[p++] = y2;
//		vert[p++] = 0;

		vert[p++] = x2;
		vert[p++] = y1;
//		vert[p++] = 0;
		vert[p++] = x1;
		vert[p++] = y2;
//		vert[p++] = 0;
		vert[p++] = x2;
		vert[p++] = y2;
//		vert[p++] = 0;
	}

	{
		colors[col][3] = _alpha*255;
		colors[col][7] = _alpha*255;
		colors[col][11] = _alpha*255;
		colors[col][15] = _alpha*255;
		GLubyte* b = &colors[col][0];
		for (int i=0;i<6*4;i++) {
			color[i] = b[i % 4];
		}
	}

	return 0;
}
*/

int PPGameSplite::Fill(PPGamePoly* poly)
{
	GLfloat x1,y1,x2,y2;
	int _x1,_y1,_x2,_y2;
	
	_x1 = poly->x;
	_y1 = poly->y;
	_x2 = poly->w;
	_y2 = poly->h;

	GLfloat* vert = &b_vert[b_ptr*VN];
	//GLfloat* uv = &b_uv[b_ptr*12];
	GLubyte* color = &b_color[b_ptr*4];
	//b_ptr ++;

	_x1 += _offsetX;
	_y1 += _offsetY;
	_x2 += _offsetX;
	_y2 += _offsetY;

	x1 = (_x1)/(screenWidth/2)-1.0f;
	y1 = -(_y1)/(screenHeight/2)+1.0f;
	x2 = (_x2)/(screenWidth/2)-1.0f;
	y2 = -(_y2)/(screenHeight/2)+1.0f;

	{
		int p=0;
		vert[p++] = x1;
		vert[p++] = y1;
//		vert[p++] = 0;
		vert[p++] = x2;
		vert[p++] = y1;
//		vert[p++] = 0;
		vert[p++] = x1;
		vert[p++] = y2;
//		vert[p++] = 0;

		vert[p++] = x2;
		vert[p++] = y1;
//		vert[p++] = 0;
		vert[p++] = x1;
		vert[p++] = y2;
//		vert[p++] = 0;
		vert[p++] = x2;
		vert[p++] = y2;
//		vert[p++] = 0;
	}

	{
		for (int i=0;i<6*4;i+=4) {
			color[i+0] = poly->r;
			color[i+1] = poly->g;
			color[i+2] = poly->b;
			color[i+3] = poly->alpha;
		}
	}

	return 0;
}

/*
int PPGameSplite::Line(int _x1,int _y1,int _x2,int _y2,int color)
{
	return 0;
}
*/

int PPGameSplite::Line(PPGamePoly* poly)
{
	GLfloat x1,y1,x2,y2;
	int _x1,_y1,_x2,_y2;
	
	_x1 = poly->x;
	_y1 = poly->y;
	_x2 = poly->w;
	_y2 = poly->h;

	GLfloat* vert = &b_vert[b_ptr*VN];
	//GLfloat* uv = &b_uv[b_ptr*12];
	GLubyte* color = &b_color[b_ptr*4];
	//b_ptr ++;

	_x1 += _offsetX;
	_y1 += _offsetY;
	_x2 += _offsetX;
	_y2 += _offsetY;

	x1 = (_x1)/(screenWidth/2)-1.0f;
	y1 = -(_y1)/(screenHeight/2)+1.0f;
	x2 = (_x2)/(screenWidth/2)-1.0f;
	y2 = -(_y2)/(screenHeight/2)+1.0f;

	{
		int p=0;
		vert[p++] = x1;
		vert[p++] = y1;
//		vert[p++] = 0;
		vert[p++] = x2;
		vert[p++] = y2;
//		vert[p++] = 0;
	}

	{
		for (int i=0;i<2*4;i+=4) {
			color[i+0] = poly->r;
			color[i+1] = poly->g;
			color[i+2] = poly->b;
			color[i+3] = poly->alpha;
		}
	}

	return 0;
}

int PPGameSplite::DrawOT()
{
	int p = 0;
	if (b_stack_ptr > 0) b_stack[b_stack_ptr-1].ptr = b_ptr;
	for (int i=0;i<b_stack_ptr;i++) {
		switch (b_stack[i].type) {
		case QBFILL_TAG:
//		case QBPOLY_TAG:
			glDisable(GL_TEXTURE_2D);
			glVertexPointer(VN,GL_FLOAT,0,&b_vert[p*VN]);
			glEnableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glColorPointer(4,GL_UNSIGNED_BYTE,0,&b_color[p*4]);
			glEnableClientState(GL_COLOR_ARRAY);
			BlendOn(b_stack[i].alphaValue,b_stack[i].blendType);
			glPushMatrix();
			glDrawArrays(GL_TRIANGLES,0,(b_stack[i].ptr-p));
			glPopMatrix();
			break;
		case QBLINE_TAG:
			glDisable(GL_TEXTURE_2D);
			glVertexPointer(VN,GL_FLOAT,0,&b_vert[p*VN]);
			glEnableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4,GL_UNSIGNED_BYTE,0,&b_color[p*4]);
			glEnableClientState(GL_COLOR_ARRAY);
			BlendOn(b_stack[i].alphaValue,b_stack[i].blendType);
			glPushMatrix();
			glLineWidth(2);
			glDrawArrays(GL_LINES,0,(b_stack[i].ptr-p));
			glPopMatrix();
			break;
		default:
			if (b_stack[i].type >= 0) 
			{

				glEnable(GL_TEXTURE_2D);
				BindTexture(b_stack[i].type);
				glVertexPointer(VN,GL_FLOAT,0,&b_vert[p*VN]);
				glEnableClientState(GL_VERTEX_ARRAY);
				glTexCoordPointer(2,GL_FLOAT,0,&b_uv[p*2]);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glColorPointer(4,GL_UNSIGNED_BYTE,0,&b_color[p*4]);
				glEnableClientState(GL_COLOR_ARRAY);
				BlendOn(b_stack[i].alphaValue,b_stack[i].blendType);
				glPushMatrix();
				glDrawArrays(GL_TRIANGLES,0,(b_stack[i].ptr-p));
				glPopMatrix();

			}
			break;
		}
		p = b_stack[i].ptr;
	}

	s_type = 1000;
	s_texId = 1000;
	s_alphaValue = 1.0;
	s_blendType = 1;
	b_ptr = 0;
	b_stack_ptr = 0;
	b_start_ptr = 0;
	return 0;
}

int PPGameSplite::BlendOn(float alpha,int type)
{
	_alpha = alpha/255.0;

	switch (type) {
	case PPGameBlend_NoMask:
		glDisable(GL_BLEND);
		glDisable(GL_FOG);
		break;
	case PPGameBlend_None:
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glDisable(GL_FOG);
		break;
	case PPGameBlend_Color:
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glDisable(GL_FOG);
		break;
	case PPGameBlend_Light:
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		glEnable(GL_BLEND);
		glDisable(GL_FOG);
		break;
	case PPGameBlend_Flash:
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_FOG);
#if TARGET_OS_IPHONE
		glFogx(GL_FOG_MODE,GL_LINEAR);
#else
		glFogi(GL_FOG_MODE,GL_LINEAR);
#endif
		glFogf(GL_FOG_START, -1.0f);
		glFogf(GL_FOG_END,0.0f);
		{
			static GLfloat fogc[]={1.0f,1.0f,1.0f,1.0f};
			glFogfv(GL_FOG_COLOR,fogc);
		}
		break;
	case PPGameBlend_Red:
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_FOG);
#if TARGET_OS_IPHONE
		glFogx(GL_FOG_MODE,GL_LINEAR);
#else
		glFogi(GL_FOG_MODE,GL_LINEAR);
#endif
		glFogf(GL_FOG_START, -1.0f);
		glFogf(GL_FOG_END,0.0f);
		{
			static GLfloat fogc[]={1.0f,0.0f,0.0f,1.0f};
			glFogfv(GL_FOG_COLOR,fogc);
		}
		break;
	case PPGameBlend_BlackMask:
		glBlendFunc(GL_SRC_ALPHA_SATURATE,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glDisable(GL_FOG);
		break;
	}

	return 0;
}

int PPGameSplite::BlendOff()
{
	_alpha = 1;
	//glDisable(GL_LIGHTING);
	//glDisable(GL_LIGHT0);
	glDisable(GL_FOG);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	return 0;
}

int PPGameSplite::Offset(int x,int y)
{
	_offsetX = x;
	_offsetY = y;
	return 0;
}

void PPGameSplite::ResetTextureState()
{
	texId=-1;
	glDisable(GL_TEXTURE_2D);
}

void PPGameSplite::DisableTexture()
{
	if (texId >= 0) {
		texId=-1;
	}
}

void PPGameSplite::EnableTexture(int texNo)
{
	if (texId != texNo) {
		texId=texNo;
	}
}

int PPGameSplite::PushStack(int type,float alphaValue,int blendType)
{
	if (type != s_type || alphaValue != s_alphaValue || blendType != s_blendType) {
		s_type = type;
		s_alphaValue = alphaValue;
		s_blendType = blendType;
		if (b_stack_ptr > 0) {
			b_start_ptr = b_ptr;
			b_stack[b_stack_ptr-1].ptr = b_ptr;
		}
		b_stack[b_stack_ptr].type = type;
		b_stack[b_stack_ptr].ptr = b_ptr;
		b_stack[b_stack_ptr].alphaValue = alphaValue;
		b_stack[b_stack_ptr].blendType = blendType;
		b_stack_ptr ++;
	}
	if (type == QBLINE_TAG) {
		b_ptr += 2;
/*
	} else
	if (type == QBPOLY_TAG) {
		b_ptr += 3;
*/
	} else {
		b_ptr += 6;
	}
	return 0;
}

int PPGameSplite::DrawPoly(PPGamePoly* poly)
{
	switch (poly->type) {
	case QBFILL_TAG:
		DisableTexture();
		BlendOn(poly->alpha,poly->blend);
		Fill(poly);
		PushStack(poly->type);
		break;
	case QBLINE_TAG:
		DisableTexture();
		BlendOn(poly->alpha,poly->blend);
		Line(poly);
		PushStack(poly->type);
		break;
/*
	case QBPOLY_TAG:
		DisableTexture();
		BlendOn(poly->alpha,poly->blend);
		Poly(poly);
		PushStack(poly->type);
		break;
*/
	default:
		if (poly->type >= 0) {
			EnableTexture(poly->type);
			patternMaster = texture[poly->type]->group;
			MoveTo(poly->x,poly->y);
			Scale(poly->w,poly->h);
			Rot(poly->x+poly->cx,poly->y+poly->cx,poly->rot);
			BlendOn(poly->alpha,poly->blend);
			Draw(poly);
			Scale(1,1);
			PushStack(poly->type,poly->alpha,poly->blend);
		}
		break;
	}
	return 0;
}

int PPGameSplite::SetTexture(int index,unsigned char* pixel,unsigned long width,unsigned long height,unsigned long bytesPerRow)
{
printf("SetTexture %d\n",index);
	int i=index;
	if (texture[i]!=NULL) {
		if (pixel) {
printf("SetTexture %d\n",i);
			texture[i]->pixel = pixel;
			texture[i]->width = width;
			texture[i]->height = height;
			texture[i]->rowbytes = bytesPerRow;
			texture[i]->bind = false;
			texture[i]->nofree = false;
			BindTexture(i);
//			free(pixel);
			texture[i]->pixel = NULL;
		} else {
printf("can't set texture %s\n",texture[i]->name);
		}
	} else {
printf("can't set texture\n");
	}
	return 0;
}

int PPGameSplite::LoadTexture(int index)
{
	int i=index;
	if (texture[i]!=NULL) {
		if (texture[i]->bind == false) {
			if (PPGameResourcePath(texture[i]->name)) {
				unsigned char* pixel;
				unsigned long width,height,bytesPerRow;
				pixel = PPGame_LoadPNG(PPGameResourcePath(texture[i]->name),&width,&height,&bytesPerRow);
//printf("loading texture %s\n",QBGameResource(texture[i]->name));
				if (pixel) {
					texture[i]->pixel = pixel;
					texture[i]->width = width;
					texture[i]->height = height;
					texture[i]->rowbytes = bytesPerRow;
					texture[i]->bind = false;
					texture[i]->nofree = false;
					BindTexture(i);
					free(pixel);
					texture[i]->pixel = NULL;
				} else {
printf("can't load texture %s\n",texture[i]->name);
				}
			}
		}
	}
	return 0;
}

int PPGameSplite::FreeTexture(int index)
{
printf("FreeTexture %d\n",index);
	int i=index;
	if (i >= 0) {
		if (texture[i]) {
			if (texture[i]->bind) {
printf("delete %d\n",i);
				glDeleteTextures(1,&texture[i]->texid);
				texture[i]->bind = false;
			}
		}
	}
	return 0;
}

int PPGameSplite::SetTextureList(PPGameTextureInfo* texList)
{
	for (int i=0;texList[i].name!=NULL;i++) {
		if (texture[i]==NULL) {
printf("setTextureList %d\n",i);
			texture[i] = new PPGameTexture();
			if (texture[i]) {
				texture[i]->name = texList[i].name;
				texture[i]->group = texList[i].group;
			}
		} else {
			texture[i]->name = texList[i].name;
			texture[i]->group = texList[i].group;
		}
	}
	return 0;
}

int PPGameSplite::ClearScreen2D(GLfloat r,GLfloat g,GLfloat b)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

#if TARGET_OS_IPHONE
	glOrthof(-1.0f,1.0f,-1.0f,1.0f,-1.0f,1.0f);
#else
	glOrtho(-1.0f,1.0f,-1.0f,1.0f,-1.0f,1.0f);
#endif
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	return 0;
}

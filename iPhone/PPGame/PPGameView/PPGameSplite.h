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

#ifdef _WIN32
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#elif TARGET_OS_IPHONE
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif
#include "PPGameDef.h"
#include "PPGamePoly.h"

#define PPGAME_MAX_TEXTURE 64
#define PPGAME_VBO_SIZE PPGAME_MAX_POLY
#define PPGAME_VBO_NUM 512

class PPGameTexture {
public:
	PPGameTexture();
	virtual ~PPGameTexture();
	
	unsigned char* pixel;
	int width;
	int height;
	int rowbytes;
	bool bind;
	bool nofree;
	
	GLuint texid;
	PPGameImage* group;
	const char* name;
};

typedef struct _PPGamePolyStack {
	int ptr;
	int type;
	float alphaValue;
	int blendType;
} PPGamePolyStack;

class PPGameSplite {
public:
	PPGameSplite();
	virtual ~PPGameSplite();
	int RegistTexture(unsigned char* pixel,int width,int height,int rowbytes,bool nofree=false);
	int BindTexture(int textureid);
	int DeleteTexture(int textureid);
	int Init();
	int Exit();
	int InitOT();

	int MoveTo(int x,int y);
	int Scale(float x,float y) {_sx = x;_sy=y;return 0;}
	
	//int DrawCore(int textureid,int px,int py,int pw,int ph,int dx,int dy,int col=15,int height=-1);
	//int DrawTexture(int textureid,int dx,int dy,int col=15,int height=-1);
	//int Draw(int textureid,int groupid,int patternid,int color=15,int height=-1);

	int DrawCore(PPGamePoly* poly,int px,int py,int pw,int ph,int dx,int dy);
	int Draw(PPGamePoly* poly);

	int DrawOT();
	//int Line(int x1,int y1,int x2,int y2,int color=15);
	int Line(PPGamePoly* poly);
	//int Fill(int x1,int y1,int x2,int y2,int color=15);
	int Fill(PPGamePoly* poly);
	//int Poly(float* points,unsigned char* colors,int num);
	//int Poly(PPGamePoly* poly);
	int BlendOn(float alpha=1.0,int type=0);
	int BlendOff();
	int Rot(float x,float y,float rot);
	int Offset(int x,int y);
	int DrawPoly(PPGamePoly* poly);
	int PushStack(int type,float alphaValue=1.0,int blendType=0);
	int ClearScreen2D(GLfloat r,GLfloat g,GLfloat b);

	int SetTexture(int index,unsigned char* pixel,unsigned long width,unsigned long height,unsigned long bytesPerRow);
	int LoadTexture(int index);
	int FreeTexture(int index);
	int SetTextureList(PPGameTextureInfo* texList);
	int UnbindAllTexture();
	int ReloadAllTexture();
	
	void ResetTextureState();
	void DisableTexture();
	void EnableTexture(int texNo=0);
	
	PPGameTexture* texture[PPGAME_MAX_TEXTURE];
	PPGameImage* patternMaster;
	//GLubyte colors[256][16];
	
	int _x;
	int _y;
	float _sx;
	float _sy;
	float _alpha;
	int _blendType;
	
	float _rotx;
	float _roty;
	float _rot;
	
	double _offsetX;
	double _offsetY;
	
	double screenWidth;
	double screenHeight;
	
	int texId;
	
	int s_type;
	int s_texId;
	float s_alphaValue;
	int s_blendType;
	
	bool initLoadTex;
	bool reloadTex;

	GLubyte colors[40][16];

	GLfloat b_vert[6*2*PPGAME_VBO_SIZE];
	GLfloat b_uv[6*2*PPGAME_VBO_SIZE];
	GLubyte b_color[6*4*PPGAME_VBO_SIZE];

	PPGamePolyStack b_stack[PPGAME_VBO_NUM];
	int b_stack_ptr;
	int b_start_ptr;
	int b_ptr;
};

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

#include "PPGameSplite.h"
//#include "QBGame.h"

class PPGameText {
public:
	PPGameText();
	virtual ~PPGameText();
	
	int print(PPGamePoly* poly,const char* string,int x,int y);
	void idle(PPGameSplite* g,int texID);
	
	void setBGColor(unsigned char r,unsigned char g,unsigned char b,unsigned char a=255);
	void setColor(unsigned char r,unsigned char g,unsigned char b,unsigned char a=255);

	//void setFontSize(float size);
	//void drawPoly(PPGameSplite* g);

	PPGameImage group[2];

private:
	
	int findFontIndex(unsigned short c);
	int newFontIndex(unsigned short c);
	
	unsigned char pixel[256*256*4];
	unsigned short ctable[16*16];
	int cflag[16*16];
	short imgWidthArray[256][9];
	
	PPGameSplite* g;
	int texID;
	bool updateTexture;
	
	float fontSize;
	
	unsigned char bg_color[4];
	unsigned char ft_color[4];
/*
#if TARGET_OS_IPHONE
	UIColor* bgColor;
	UIColor* ftColor;
#else
	NSColor* bgColor;
	NSColor* ftColor;
#endif
*/
};

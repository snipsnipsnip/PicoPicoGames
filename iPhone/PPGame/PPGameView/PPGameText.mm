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

#include "PPGameText.h"
#if TARGET_OS_IPHONE
#include "UIImage-FontTexture.h"
#endif

/*
static short colordata[] = {
	//0,0,0,0,0,0,0,0,0,2,5,6,1,7,4,0,0,0,0,0,0,0,3,0
	0,2,5,6,1,7,4,3,0,32,33,34,35,36,37,38
};
static short colordata2[] = {
	//0,0,0,0,0,0,0,0,0,2,5,6,1,7,4,0,0,0,0,0,0,0,3,0
	0,9,10,11,12,13,28,14,15,32,33,34,35,36,37,38
};
*/

//static unichar ctable[16*16] = {0};
//static int cflag[16*16] = {0};
//static short imgWidthArray[256][9] = {0};
static PPGameImage sMaster[]={
    {NULL,256,256,1,256,256},
    {NULL,0,0,0,0,0}
};
static unichar ftable[128];
static NSString* fstring[8] = {
	@"　　　　　　　　　　　　　　　　",
	@"　　　　　　　　　　　　　　　　",
	@"　！”＃＄％＆’（）＊＋，ー．／",
	@"０１２３４５６７８９：；＜＝＞？",
	@"＠ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯ",
	@"ＰＱＲＳＴＵＶＷＸＹＺ［￥］＾＿",
	@"｀ａｂｃｄｅｆｇｈｉｊｋｌｍｎｏ",
	@"ｐｑｒｓｔｕｖｗｘｙｚ｛｜｝〜　",
};

PPGameText::PPGameText()
{
	updateTexture = false;
	
	group[0] = sMaster[0];
	group[1] = sMaster[1];
	group[0].pat = imgWidthArray[0];

	for (int i=0;i<16*16;i++) {
		ctable[i] = 0;
		cflag[i] = 0;
		for (int j=0;j<9;j++) {
			imgWidthArray[i][j] = 0;
		}
	}
	
	for (int i=0;i<256;i++) {
		imgWidthArray[i][0] = i;
		imgWidthArray[i][1] = 0;//(i % 16)*16;
		imgWidthArray[i][2] = 0;//(i / 16)*16;
		imgWidthArray[i][3] = 16;
		imgWidthArray[i][4] = 16;
		imgWidthArray[i][5] = 0;//(i % 16)*16;
		imgWidthArray[i][6] = 0;//(i / 16)*16;
		imgWidthArray[i][7] = (i % 16)*16;
		imgWidthArray[i][8] = 256-(i / 16)*16-16;
	}
	
	for (int i=0;i<128;i++) {
		ftable[i] = [fstring[i/16] characterAtIndex:(i%16)];
	}
/*
	for (int i=0;i<256*256*4;i+=4) {
		pixel[i+0] = 0xff;
		pixel[i+1] = i % 0xff;
		pixel[i+2] = 0xff;
		pixel[i+3] = 0xff;
	}
*/
	fontSize = 15;
//	bgColor = nil;
//	ftColor = nil;

	ft_color[0] = 0xff;
	ft_color[1] = 0xff;
	ft_color[2] = 0xff;
	ft_color[3] = 0xff;

	bg_color[0] = 0;
	bg_color[1] = 0;
	bg_color[2] = 0;
	bg_color[3] = 0;
}

PPGameText::~PPGameText()
{
}

int PPGameText::findFontIndex(unsigned short c)
{
	int n=(c % 256);
	for (int i=0;i<256;i++) {
		if (ctable[(n + i) % 256] == c) {
			return n+i;
		}
	}
	return -1;
}

int PPGameText::newFontIndex(unsigned short c)
{
	int n=(c % 256);
	for (int i=0;i<256;i++) {
		if (cflag[(n + i) % 256] == 0) {
			return n+i;
		}
	}
	return -1;
}

int PPGameText::print(PPGamePoly* poly,const char* string,int x,int y)
{
	int n=0;
#if TARGET_OS_IPHONE
	UIColor* bgColor;
	UIColor* ftColor;
	bgColor = [[UIColor colorWithRed:bg_color[0]/255.0 green:bg_color[1]/255.0 blue:bg_color[2]/255.0 alpha:bg_color[3]/255.0] retain];
	ftColor = [[UIColor whiteColor] retain];//[[UIColor colorWithRed:ft_color[0]/255.0 green:ft_color[1]/255.0 blue:ft_color[2]/255.0 alpha:ft_color[3]/255.0] retain];
#else
	NSColor* bgColor=nil;
	NSColor* ftColor=nil;
//	bgColor = [[NSColor colorWithRed:bg_color[0]/255.0 green:bg_color[1]/255.0 blue:bg_color[2]/255.0 alpha:bg_color[3]/255.0] retain];
//	ftColor = [[NSColor colorWithRed:ft_color[0]/255.0 green:ft_color[1]/255.0 blue:ft_color[2]/255.0 alpha:ft_color[3]/255.0] retain];
#endif


	//if (g) 
	{
		size_t imgWidth;
#if TARGET_OS_IPHONE
		size_t imgHeight;
#endif
		NSString* s = [NSString stringWithUTF8String:string];
		for (int i=0;i<s.length;i++) {
			int fontIndex = -1;
			unichar c = [s characterAtIndex:i];

			if (c < 128) {
				c = ftable[c];
			}

#if 1
			//for (int j=0;j<16*16;j++) {
			//	if (ctable[j] == c) {
			{
					int j = findFontIndex(c);
					if (j >= 0) {
						//見つけた
						fontIndex = j;
						cflag[j] ++;
						imgWidth = imgWidthArray[j][3];
					}
					//break;
			}
			//	}
			//}
			if (fontIndex < 0) {
				int j = newFontIndex(c);
				if (j >= 0) {
				//for (int j=0;j<16*16;j++) {
				//	if (cflag[j] == 0) {
//NSLog([s substringWithRange:NSMakeRange(i,1)]);
						//フォント作成
						imgWidth = 16;
#if TARGET_OS_IPHONE
						{
							[UIImage setFontImageBGColor:bgColor];
							[UIImage setFontColor:ftColor];
							[UIImage setFontSize:fontSize];
							UIImage* fontImage = [UIImage fontImage:[NSString stringWithCharacters:&c length:1]];
							imgWidth = CGImageGetWidth(fontImage.CGImage);
							imgHeight = CGImageGetHeight(fontImage.CGImage);
							float x = (j % 16)*16;
							float y = (j / 16)*16;
							CGContextRef spriteContext = CGBitmapContextCreate(pixel, 256, 256, 8, 256 * 4, CGImageGetColorSpace(fontImage.CGImage), kCGImageAlphaPremultipliedLast);
							CGContextDrawImage(spriteContext, CGRectMake(x, y, (CGFloat)imgWidth, (CGFloat)imgHeight), fontImage.CGImage);
							CGContextRelease(spriteContext);
						}
#else
						{
							
						}
#endif
						fontIndex = j;
						ctable[j] = c;
						cflag[j] ++;
						imgWidthArray[j][3] = imgWidth;
						updateTexture = true;
				}
				//		break;
				//	}
				//}
			}
#else
			fontIndex = 0;
#endif

			if (fontIndex >= 0) {
				poly[n].init();
				poly[n].r = ft_color[0];
				poly[n].g = ft_color[1];
				poly[n].b = ft_color[2];
				poly[n].alpha = ft_color[3];
				poly[n].splite(x,y,fontIndex,0,texID);
				n ++;
				x += imgWidthArray[fontIndex][3];
			}
		}
	}
	
	[bgColor release];
	[ftColor release];
	return n;
}

void PPGameText::idle(PPGameSplite* _g,int _texID)
{
	g = _g;
	texID = _texID;
	for (int i=0;i<16*16;i++) {
		if (cflag[i] > 0) {
			cflag[i] --;
		}
	}

	if (updateTexture) {
		g->FreeTexture(texID);
		g->SetTexture(texID,pixel,256,256,256*4);
		updateTexture = false;
	}
}

/*
void PPGameText::drawPoly(PPGameSplite* g)
{
}
*/

void PPGameText::setBGColor(unsigned char r,unsigned char g,unsigned char b,unsigned char a)
{
	bg_color[0] = r;
	bg_color[1] = g;
	bg_color[2] = b;
	bg_color[3] = a;
/*
	[bgColor release];
#if TARGET_OS_IPHONE
	bgColor = [[UIColor colorWithRed:r/255.0 green:g/255.0 blue:b/255.0 alpha:a/255.0] retain];
#endif
*/
}

void PPGameText::setColor(unsigned char r,unsigned char g,unsigned char b,unsigned char a)
{
	ft_color[0] = r;
	ft_color[1] = g;
	ft_color[2] = b;
	ft_color[3] = a;
/*
	[ftColor release];
#if TARGET_OS_IPHONE
	ftColor = [[UIColor colorWithRed:r/255.0 green:g/255.0 blue:b/255.0 alpha:a/255.0] retain];
#endif
*/
}

/*
void PPGameText::setFontSize(float size)
{
}
*/
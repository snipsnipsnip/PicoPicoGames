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

#include "PPGameUtil.h"
#include "PPGameBGM.h"
#include "PPGameSound.h"
#include "QBGame.h"

#define PPGAME_MAX_BGM 5

static int keySide = 0;
static PPGameBGM* bgm[PPGAME_MAX_BGM] = {0};

const char* PPGameResourcePath(const char* name)
{
	NSString* n = [NSString stringWithCString:name];

//NSLog([n stringByDeletingPathExtension]);
//NSLog([n pathExtension]);

	const char* f = [[[NSBundle mainBundle] pathForResource:[n stringByDeletingPathExtension] ofType:[n pathExtension]] fileSystemRepresentation];
	if (f == NULL) return "";
	return f;
}

int PPGame_SetDefault(const char* name)
{
	if (strcmp(name,PPGAME_KEY_SIDE_RIGHT) == 0) {
		keySide = 1;
	} else
	if (strcmp(name,PPGAME_KEY_SIDE_LEFT) == 0) {
		keySide = 0;
	}
	return keySide;
}

int PPGame_GetDefault(const char* name)
{
	if (strcmp(name,PPGAME_KEY_SIDE_RIGHT) == 0) {
		if (keySide == 1) return 1;
	} else
	if (strcmp(name,PPGAME_KEY_SIDE_LEFT) == 0) {
		if (keySide == 0) return 1;
	}
	return 0;
}


unsigned char* PPGame_LoadPNG(const char* path,unsigned long* width,unsigned long* height,unsigned long* bytesPerRow)
{
	unsigned char* pixel = NULL;
#if TARGET_OS_IPHONE
	UIImage* image = [UIImage imageWithContentsOfFile:[[NSFileManager defaultManager] stringWithFileSystemRepresentation:path length:strlen(path)]];
	CGImageRef cgImage = image.CGImage;
#else
	NSImage* image = [[[NSImage alloc] initWithContentsOfFile:[[NSFileManager defaultManager] stringWithFileSystemRepresentation:path length:strlen(path)]] autorelease];
	NSBitmapImageRep* bitmapImage=[NSBitmapImageRep imageRepWithData:[image TIFFRepresentation]];
	CGImageRef cgImage = [bitmapImage CGImage];
#endif
	if (cgImage) {
		CGContextRef spriteContext;
		int w = CGImageGetWidth(cgImage);
		int h = CGImageGetHeight(cgImage);
		pixel = (unsigned char*) malloc(w*h*4);
		spriteContext = CGBitmapContextCreate(pixel,w,h,8,w*4,CGImageGetColorSpace(cgImage),kCGImageAlphaPremultipliedLast);
		CGContextDrawImage(spriteContext,CGRectMake(0.0,0.0,(CGFloat)w,(CGFloat)h),cgImage);
		CGContextRelease(spriteContext);
		*width = w;
		*height = h;
		*bytesPerRow = w*4;
	}
	return pixel;
}

int PPGame_GetParam(const char* key,int defaultValue)
{
	id v = [[NSUserDefaults standardUserDefaults] objectForKey:[NSString stringWithUTF8String:key]];
	if (v != nil) {
		return [v intValue];
	}
	return defaultValue;
}

void PPGame_SetParam(const char* key,int value)
{
	[[NSUserDefaults standardUserDefaults] setObject:[NSNumber numberWithInt:value] forKey:[NSString stringWithUTF8String:key]];
}

unsigned char* PPGame_GetData(const char* key,int* dataSize)
{
	NSData* data = [[NSUserDefaults standardUserDefaults] objectForKey:[NSString stringWithUTF8String:key]];
	*dataSize = [data length];
	return (unsigned char*)[data bytes];
}

void PPGame_SetData(const char* key,unsigned char* data,int dataSize)
{
	[[NSUserDefaults standardUserDefaults] setObject:[NSData dataWithBytes:data length:dataSize] forKey:[NSString stringWithUTF8String:key]];
}

int PPGame_InitBGM(int no,const char* key)
{
	if (bgm[no-1] == nil) {
		bgm[no-1] = [[PPGameBGM alloc] init];
	}
	if (bgm[no-1] != nil) {
		[bgm[no-1] reset];
		bgm[no-1].key = [NSString stringWithUTF8String:key];
		[bgm[no-1] loadForKey:[NSString stringWithUTF8String:key]];
		bgm[no-1].selectedPlay = PPGame_GetSoundEnable();
	}
	return 0;
}

void PPGame_ReleaseBGM()
{
	for (int i=0;i<PPGAME_MAX_BGM;i++) {
		[bgm[i] release];
		bgm[i] = nil;
	}
}

void PPGame_IdleBGM(void* controller,int playBGM,bool playBGMOneTime,int chooseBGM)
{
	for (int no=1;no<=PPGAME_MAX_BGM;no++) {
		if (bgm[no-1]) {
			if (chooseBGM != 0) {
				if (chooseBGM == no) {
#if TARGET_OS_IPHONE
					[bgm[no-1] selectBGM:(UIViewController*)controller];
#endif
				} else
				if (chooseBGM < 0) {
					[bgm[no-1] reset];
					[bgm[no-1] saveForKey:bgm[no-1].key];
				}
			}
			if (playBGM != 0) {
				if (playBGM == no) {
					if (playBGMOneTime) {
						if (PPGame_GetSoundEnable()) [bgm[no-1] playOneTime];
						playBGMOneTime = false;
					} else {
						if (PPGame_GetSoundEnable()) [bgm[no-1] play];
					}
				} else
				if (playBGM < 0) {
					if (PPGame_GetSoundEnable()) [bgm[no-1] stop];
				}
			}
		}
	}
}

int PPGame_GetLocale()
{
	//NSLog([[NSLocale currentLocale] objectForKey:NSLocaleLanguageCode]);
	NSString* code = NSLocalizedString(@"language",@"");
	if ([code isEqualToString:@"jp"]) {
		return QBGAME_LOCALE_JAPANESE;
	}
	return QBGAME_LOCALE_OTHER;
}

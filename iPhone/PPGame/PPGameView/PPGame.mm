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

#import "PPGame.h"

@implementation PPGame

@synthesize controller;
@synthesize initFlag;

- (id)init
{
	if (self = [super init]) {
		for (int i=0;i<PPGAME_MAX_POLY;i++) {
			poly[i].init();
		}
	}
	return self;
}

- (void)dealloc
{
	[self exit];
	[controller release];
	[super dealloc];
}

- (void)setView:(PPGameView*)_view
{
	view = _view;
}

- (bool)start
{
	return true;
}

- (int)idle
{
	return 0;
}

- (int)draw
{
	return 0;
}

- (int)drawPost
{
	return 0;
}

- (int)drawOT
{
	PPGameSplite* g = view.g;
	g->InitOT();
	for (int i=0;i<polyCount;i++) {
		g->DrawPoly(&poly[i]);
	}
	[self drawPost];
	g->DrawOT();
	return 0;
}

- (bool)exit
{
	return true;
}

- (void)resignActive
{
}

- (void)becomeActive
{
}

- (PPGameTextureInfo*)textures
{
	return NULL;
}

- (void)textureIdle
{
	PPGameSplite* g = view.g;
	
	if ([self textures]) {
		if (!g->initLoadTex) {
			if (g->SetTextureList([self textures]) == 0) {
				PPGameTextureInfo* list = [self textures];
				for (int i=0;list[i].name!=NULL;i++) {
					if (list[i].parge == false) {
						g->LoadTexture(i);
					} else {
						g->FreeTexture(i);
					}
				}
				g->initLoadTex = true;
			}
		}
	}
}

- (void)closeGame
{
	[[NSNotificationCenter defaultCenter] postNotificationName:@"closeGame" object:nil];
}

- (BOOL)horizontalView
{
	return NO;
}

/*
- (int)arrowKeyCount
{
	return 4;
}
*/

- (int)crosskeyAreaSize
{
	return 64*64;
}

@end

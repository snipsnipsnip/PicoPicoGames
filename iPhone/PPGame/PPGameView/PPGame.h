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

#import <Foundation/Foundation.h>
#import "PPGamePoly.h"
#import "PPGameSplite.h"
#if TARGET_OS_IPHONE
#import "PPGameView.h"
#import "PPGameText.h"
#else
#import "PPGameView_App.h"
#endif

@interface PPGame : NSObject {
	int polyCount;
	PPGamePoly poly[PPGAME_MAX_POLY];
	PPGameView* view;
#if TARGET_OS_IPHONE
	UIViewController* controller;
#else
	id controller;
#endif
	bool initFlag;
}

#if TARGET_OS_IPHONE
@property (nonatomic,retain) UIViewController* controller;
#else
@property (nonatomic,retain) id controller;
#endif
@property (nonatomic,assign) bool initFlag;

- (void)setView:(PPGameView*)view;

- (bool)start;
- (int)idle;
- (int)draw;
- (int)drawPost;
- (bool)exit;
- (void)resignActive;
- (void)becomeActive;

- (PPGameTextureInfo*)textures;

- (int)drawOT;
- (void)textureIdle;

//- (void)loadTexture:(int)index;
//- (void)unloadTexture:(int)index;

- (void)closeGame;

- (BOOL)horizontalView;
//- (int)arrowKeyCount;

- (int)crosskeyAreaSize;

@end

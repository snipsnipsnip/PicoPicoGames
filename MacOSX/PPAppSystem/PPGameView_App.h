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

#import <Cocoa/Cocoa.h>
#import "PPGameSplite.h"
#import "PPGameButton.h"

@class PPGame;

@interface PPGameView : NSOpenGLView {
	IBOutlet id controller;
	float deepZ;
	PPGameSplite*		g;
	PPGame* game;
	bool _initialize;
	float adx;
	float ady;
	unsigned long staticKey;
	CGPoint	touchLocation;
	bool	touchScreen;
	NSMutableSet* touchesSet;
}

@property (nonatomic,readonly)  PPGameSplite* g;
@property (nonatomic,retain) PPGame* game;
@property (nonatomic,readonly) CGPoint touchLocation;
@property (nonatomic,readonly) bool touchScreen;

- (int)arrowKey:(GLButton*)button count:(int)count;
- (unsigned long)arrowKey:(GLButton*)button;
- (unsigned long)arrowKey4:(GLButton*)button;
- (unsigned long)arrowKey8:(GLButton*)button;
- (bool)touchCheck:(GLButton*)button;
- (bool)touchCheckWithPoly:(float*)keyPos x:(float)x y:(float)y;
- (unsigned long)staticButton;

@end

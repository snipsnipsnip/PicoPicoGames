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

#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import "PPGameSplite.h"
#import "QBSound.h"
#import "PPGameButton.h"

//#define MAX_POLY 2000


@class PPGame;

/*
//void PPGame_Splite(PPGamePoly* poly,int x,int y,int group,int pat,int type,float scale=1.0);
//bool PPGame_TouchCheck(PPGameView* view,PPGameImage* patternMaster,int group,int pat);
short PPGame_CharDeltaX(PPGameImage* patternMaster,int group,int pat);
short PPGame_CharDeltaY(PPGameImage* patternMaster,int group,int pat);
short PPGame_CharDeltaW(PPGameImage* patternMaster,int group,int pat);
short PPGame_CharDeltaH(PPGameImage* patternMaster,int group,int pat);

class GLButton {
public:
	GLButton(PPGameImage* _group,int _image,int _pat) {
		group = _group;
		image = _image;
		pat = _pat;
		dx = 0;
		dy = 0;
	}
	PPGameImage* group;
	int image;
	int pat;
	float dx;
	float dy;
};
*/

/*
This class wraps the CAEAGLLayer from CoreAnimation into a convenient UIView subclass.
The view content is basically an EAGL surface you render your OpenGL scene into.
Note that setting the view non-opaque will only work if the EAGL surface has an alpha channel.
*/
@interface PPGameView : UIView {
    
	PPGameSplite*		g;
	PPGame*				game;

	CGPoint				touchLocation;
	bool				touchScreen;

    /* The pixel dimensions of the backbuffer */
    GLint				width;
    GLint				height;
	
	NSMutableSet*		touchesSet;

@private
    
    EAGLContext *context;
    
    /* OpenGL names for the renderbuffer and framebuffers used to render to this view */
    GLuint viewRenderbuffer, viewFramebuffer;
    
    /* OpenGL name for the depth buffer that is attached to viewFramebuffer, if it exists (0 if it does not exist) */
    GLuint depthRenderbuffer;
    
    NSTimer *animationTimer;
    NSTimeInterval animationInterval;
	
	//bool initGameFlag;
}

@property NSTimeInterval animationInterval;
@property (nonatomic,readonly) CGPoint touchLocation;
@property (nonatomic,readonly) bool touchScreen;
@property (nonatomic,readonly)  GLint width;
@property (nonatomic,readonly)  GLint height;
@property (nonatomic,readonly)  PPGameSplite* g;
@property (nonatomic,retain) PPGame* game;

- (void)startAnimation;
- (void)stopAnimation;
- (void)drawView;

//- (bool)initGame;
//- (int)idle;
//- (int)draw;
//- (int)drawPost;
//- (bool)exitGame;

//- (void)resignActive;
//- (void)becomeActive;

- (void)reloadTexture;

- (int)arrowKey:(GLButton*)button count:(int)count;
- (unsigned long)arrowKey:(GLButton*)button;
- (unsigned long)arrowKey4:(GLButton*)button;
- (unsigned long)arrowKey8:(GLButton*)button;
- (bool)touchCheck:(GLButton*)button;
- (bool)touchCheckWithPoly:(float*)keyPos x:(float)x y:(float)y;
- (unsigned long)staticButton;
- (NSMutableSet*)touchesSet;

@end

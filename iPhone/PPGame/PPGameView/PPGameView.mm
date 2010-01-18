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

#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>
#import "PPGame.h"
#import "PPGameView.h"
#import "QBRect.h"

/*
static float keyPos[][2] = {
	{95,378},		//UP
	{224,378},
	{159,430},
	{95,378},
	
	{95,378},		//LEFT
	{159,430},
	{95,479},
	{95,378},
	
	{95,479},		//DOWN
	{159,430},
	{224,479},
	{95,479},
	
	{224,378},		//RIGHT
	{224,479},
	{159,430},
	{224,378},
};
*/

#define USE_DEPTH_BUFFER 0

/*
void PPGame_Splite(PPGamePoly* poly,int x,int y,int group,int pat,int type,float scale)
{
	poly->type = type;
	poly->x = x;//*resolutionValue();
	poly->y = y;//*resolutionValue();
	poly->pat = pat;
	poly->color = 15;//_color;
	poly->w = scale;//_fscalex*scale;
	poly->h = scale;//_fscaley*scale;
	poly->cliph = -1;
	poly->rot = 0;//_rotate;
	poly->group = group;
	poly->alpha = 1;//_alpha;
	poly->blendType = PPGameBlend_None;//_blendType;
}
*/

// A class extension to declare private methods
@interface PPGameView ()

@property (nonatomic, retain) EAGLContext *context;
@property (nonatomic, assign) NSTimer *animationTimer;

- (BOOL) createFramebuffer;
- (void) destroyFramebuffer;

@end


@implementation PPGameView

@synthesize context;
@synthesize animationTimer;
@synthesize animationInterval;
@synthesize game;
@synthesize g;
@synthesize width,height;
@synthesize touchLocation,touchScreen;

// You must implement this method
+ (Class)layerClass {
    return [CAEAGLLayer class];
}

#if 0
//ビューの回転サンプル
- (void)awakeFromNib
{
	CGAffineTransform transform = self.transform;

	// Rotate the view 90 degrees. 
	//if ([[UIDevice currentDevice] orientation] == UIDeviceOrientationLandscapeRight) {
		transform = CGAffineTransformRotate(transform, (M_PI / 2.0));
	//} else {
	//	transform = CGAffineTransformRotate(transform, (M_PI * 1.5));
	//}
/*
    UIScreen *screen = [UIScreen mainScreen];
    // Translate the view to the center of the screen
    transform = CGAffineTransformTranslate(transform, 
        ((screen.bounds.size.height) - (self.bounds.size.height))/2, 
        0);
*/
	self.transform = transform;
/*	
	CGRect newFrame = self.frame;
	newFrame.origin.x = 190;
	self.frame = newFrame;
*/
}
#endif

//The GL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:
- (id)initWithCoder:(NSCoder*)coder {
    
    if ((self = [super initWithCoder:coder])) {
        // Get the layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        
        eaglLayer.opaque = YES;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
        
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
        
        if (!context || ![EAGLContext setCurrentContext:context]) {
            [self release];
            return nil;
        }
        
        animationInterval = 1.0 / 60.0;
		
		g = new PPGameSplite();

		QBSound_Reset();
    }
    return self;
}

- (void)drawView {
	[game setView:self];
    [EAGLContext setCurrentContext:context];

	glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
	glViewport(0, 0, width, height);

	//if (initGameFlag == false) {
	if (game.initFlag == NO) {
		[game start];
		game.initFlag = YES;
	}
	//	initGameFlag = true;
	//}
	if ([game idle] != 0) {
		
	}
	if (g) {
		[game textureIdle];
	
		g->screenWidth = self.width;//160;
		g->screenHeight = self.height;//416/2;
	
		g->ClearScreen2D(0.0f, 0.0f, 0.0f);

		if ([game draw] == 0) {
			[game drawOT];
		}
	}

    glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
    [context presentRenderbuffer:GL_RENDERBUFFER_OES];
}


- (void)layoutSubviews {
    [EAGLContext setCurrentContext:context];
    [self destroyFramebuffer];
    [self createFramebuffer];
    [self drawView];
}


- (BOOL)createFramebuffer {
    
    glGenFramebuffersOES(1, &viewFramebuffer);
    glGenRenderbuffersOES(1, &viewRenderbuffer);
    
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
    [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*)self.layer];
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, viewRenderbuffer);
    
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &width);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &height);

//NSLog(@"width %d",width);
//NSLog(@"height %d",height);

    if (USE_DEPTH_BUFFER) {
        glGenRenderbuffersOES(1, &depthRenderbuffer);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
        glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, width, height);
        glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);
    }
    
    if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES) {
        NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
        return NO;
    }
    
    return YES;
}


- (void)destroyFramebuffer {
    
    glDeleteFramebuffersOES(1, &viewFramebuffer);
    viewFramebuffer = 0;
    glDeleteRenderbuffersOES(1, &viewRenderbuffer);
    viewRenderbuffer = 0;
    
    if(depthRenderbuffer) {
        glDeleteRenderbuffersOES(1, &depthRenderbuffer);
        depthRenderbuffer = 0;
    }
}


- (void)startAnimation {
    self.animationTimer = [NSTimer scheduledTimerWithTimeInterval:animationInterval target:self selector:@selector(drawView) userInfo:nil repeats:YES];
}


- (void)stopAnimation {
    self.animationTimer = nil;
}


- (void)setAnimationTimer:(NSTimer *)newTimer {
    [animationTimer invalidate];
    animationTimer = newTimer;
}


- (void)setAnimationInterval:(NSTimeInterval)interval {
    
    animationInterval = interval;
    if (animationTimer) {
        [self stopAnimation];
        [self startAnimation];
    }
}

- (void)dealloc {
//NSLog(@"PPGameView dealloc");
    [self stopAnimation];
    
    if ([EAGLContext currentContext] == context) {
        [EAGLContext setCurrentContext:nil];
    }
    
    [context release];  
	
	delete g;
	g = NULL;
	
	//[self exitGame];
	[game release];
	
    [super dealloc];
}

/*
- (int)idle
{
	return [game idle:self];
}

- (int)draw
{
	return [game draw:self];
}

- (int)drawPost
{
	return [game drawPost:self];
}

- (bool)initGame
{
	return [game start:self];
}

- (bool)exitGame
{
	return [game exit];
}

- (void)resignActive
{
	[game resignActive];
}

- (void)becomeActive
{
	[game becomeActive];
}
*/

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	//[touchesSet release];
	//touchesSet = [touches retain];

	if (touchesSet == nil) touchesSet = [[NSMutableSet set] retain];
	[touchesSet unionSet:touches];

//NSLog(@"touchesBegan %@",[touchesSet description]);

	CGRect				bounds = [self bounds];
    UITouch*	touch = [[event touchesForView:self] anyObject];
	touchLocation = [touch locationInView:self];
	touchScreen = true;
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	//[touchesSet release];
	//touchesSet = [touches retain];

	if (touchesSet == nil) touchesSet = [[NSMutableSet set] retain];
	[touchesSet unionSet:touches];

	CGRect				bounds = [self bounds];
    UITouch*	touch = [[event touchesForView:self] anyObject];
	touchLocation = [touch locationInView:self];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
//	[touchesSet release];
//	touchesSet = [touches retain];

	if (touchesSet == nil) touchesSet = [[NSMutableSet set] retain];
	[touchesSet minusSet:touches];

//NSLog(@"touchesEnded %@",[touchesSet description]);

	CGRect				bounds = [self bounds];
    UITouch*	touch = [[event touchesForView:self] anyObject];
	touchLocation = [touch locationInView:self];
	touchScreen = false;
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
//	[touchesSet release];
//	touchesSet = [touches retain];

	if (touchesSet == nil) touchesSet = [[NSMutableSet set] retain];
	[touchesSet minusSet:touches];

	CGRect				bounds = [self bounds];
    UITouch*	touch = [[event touchesForView:self] anyObject];
	touchLocation = [touch locationInView:self];
	touchScreen = false;
}

- (unsigned long)arrowKey4:(GLButton*)button
{
	unsigned long key = 0;
	int n = [self arrowKey:button count:4];
	if (n >= 0) {
		switch ((n+1) % 4) {
		case 0:
			key |= (PAD_RIGHT);
			break;
		case 1:
			key |= (PAD_UP);
			break;
		case 2:
			key |= (PAD_LEFT);
			break;
		case 3:
			key |= (PAD_DOWN);
			break;
		}
	}
	return key;
}

- (unsigned long)arrowKey8:(GLButton*)button
{
	unsigned long key = 0;
	int n = [self arrowKey:button count:8];
	if (n >= 0) {
		switch ((n+2) % 8) {
		case 0:
			key |= (PAD_RIGHT);
			break;
		case 1:
			key |= (PAD_UP | PAD_RIGHT);
			break;
		case 2:
			key |= (PAD_UP);
			break;
		case 3:
			key |= (PAD_UP | PAD_LEFT);
			break;
		case 4:
			key |= (PAD_LEFT);
			break;
		case 5:
			key |= (PAD_DOWN | PAD_LEFT);
			break;
		case 6:
			key |= (PAD_DOWN);
			break;
		case 7:
			key |= (PAD_DOWN | PAD_RIGHT);
			break;
		}
	}
	return key;
}

- (unsigned long)arrowKey:(GLButton*)button
{
	unsigned long key = 0;
	int n = [self arrowKey:button count:4];
	if (n >= 0) {
		switch ((n+1) % 4) {
		case 0:
			key |= (PAD_RIGHT);
			break;
		case 1:
			key |= (PAD_UP);
			break;
		case 2:
			key |= (PAD_LEFT);
			break;
		case 3:
			key |= (PAD_DOWN);
			break;
		}
	}
	return key;
}

- (int)arrowKey:(GLButton*)button count:(int)count
{
	int n=-1;

	{
		NSEnumerator* e = [touchesSet objectEnumerator];
		UITouch* t;
		while (t = [e nextObject]) {
			CGPoint location = [t locationInView:self];
			CGPoint pos;
			{
				int hx,hy,hw,hh;
				hx = PPGame_CharDeltaX(button->group,button->image,button->pat);
				hy = PPGame_CharDeltaY(button->group,button->image,button->pat);
				hw = PPGame_CharDeltaW(button->group,button->image,button->pat);
				hh = PPGame_CharDeltaH(button->group,button->image,button->pat);
				pos.x = location.x - (hx+hw/2);
				pos.y = location.y - (hy+hh/2);
			}
			int len = 96*96;
			if (game) {
				len = [game crosskeyAreaSize];
			}
			float l = pos.x*pos.x+pos.y*pos.y;
			if (l < 8*8) {
			} else
			if (l > len) {
			} else {
				float dt = 360.0f/((float)count);
				float q = atan2f(pos.x,pos.y)*180/M_PI+180+dt/2;
				n = (int)(q / dt);
				if (n >= 8) n = 0;
				break;
			}
		}
	}
	
	return n;
}

- (bool)touchCheck:(GLButton*)button
{
	NSEnumerator* e = [touchesSet objectEnumerator];
	UITouch* t;
	while (t = [e nextObject]) {
		int hx,hy,hw,hh;
		int x,y;
		CGPoint location = [t locationInView:self];
		x = location.x;
		y = location.y;
		{
			hx = PPGame_CharDeltaX(button->group,button->image,button->pat);
			hy = PPGame_CharDeltaY(button->group,button->image,button->pat);
			hw = PPGame_CharDeltaW(button->group,button->image,button->pat);
			hh = PPGame_CharDeltaH(button->group,button->image,button->pat);
			if (x-button->dx > hx && y-button->dy > hy && x-button->dx < hx+hw && y-button->dy < hy+hh) {
				return true;
			}
		}
	}
	return false;
}

- (bool)touchCheckWithPoly:(float*)keyPos x:(float)dx y:(float)dy
{
	bool touch = false;

	//if (touchScreen) 
	{
		NSEnumerator* e = [touchesSet objectEnumerator];
		UITouch* t;
		while (t = [e nextObject]) {
			QBRect r;
			int x,y;
			CGPoint location = [t locationInView:self];
			x = location.x;
			y = location.y;
			if (r.tri_hitcheck(keyPos,x-dx,y-dy)) {
				touch = true;
			}
		}
	}
	
	return touch;
}


- (void)reloadTexture
{
	if (g) {
		g->initLoadTex = false;
	}
}

- (unsigned long)staticButton
{
	return 0;
}

- (NSMutableSet*)touchesSet
{
	return touchesSet;
}

@end

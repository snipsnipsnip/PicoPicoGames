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

#include <Carbon/Carbon.h>
#import "PPGameView_App.h"
#import <OpenGL/gl.h>
#import <OpenGL/glu.h>
//#include "SIXAXIS.h"
#include <IOKit/hid/IOHIDUsageTables.h>
//#include "HID_Utilities_External.h"
#include "QBSound.h"
#import "PPGame.h"
#import "QBRect.h"

#define kOurVendorID		0x054c
#define kOurProductID		0x0268

#define JOY_PAD_UP		0x01
#define JOY_PAD_DOWN	0x02
#define JOY_PAD_LEFT	0x04
#define JOY_PAD_RIGHT	0x08
#define JOY_PAD_Z		0x10
#define JOY_PAD_X		0x20
#define JOY_PAD_C		0x40
#define JOY_PAD_SPACE	0x80
#define JOY_PAD_1		0x0100
#define JOY_PAD_2		0x0200
#define JOY_PAD_3		0x0400
#define JOY_PAD_4		0x0800
#define JOY_PAD_5		0x1000
#define JOY_PAD_6		0x2000
#define JOY_PAD_7		0x4000
#define JOY_PAD_8		0x8000
#define JOY_PAD_9		0x10000
#define JOY_PAD_ESC		0x20000
#define JOY_PAD_RETURN	0x40000
#define JOY_PAD_START	0x40000
#define JOY_PAD_SHIFT	0x80000
#define JOY_PAD_CONTROL	0x100000
#define JOY_PAD_SELECT	0x100000
#define JOY_PAD_OPTION	0x200000
//#define PAD_ANY		0x400000
#define JOY_PAD_X_UP	0x800000
#define JOY_PAD_X_DOWN	0x1000000

#define JOY_UP1 5
#define JOY_DOWN1 7
#define JOY_LEFT1 8
#define JOY_RIGHT1 6
#define JOY_UP2 13
#define JOY_DOWN2 15
#define JOY_LEFT2 16
#define JOY_RIGHT2 14
#define JOY_L1 11
#define JOY_L2 10
#define JOY_R1 12
#define JOY_R2 9
#define JOY_START 4
#define JOY_SELECT 1

#define JOY_POV 57
#define JOY_AX1 48
#define JOY_AY1 49
#define JOY_AX2 50
#define JOY_AY2 53

//static int joyf[256];
static int joy[256];

int QBGameHitButtonIndex()
{
	for (int i=0;i<100;i++) {
		if (joy[i] != 0) return i;
	}
	return -1;
}

int QBGameButton(int index)
{
	if (index >= 0 && index < 256) {
		return joy[index];
	}
	return 0;
}

static unsigned long H_GetImdKey(void)
{
	unsigned long retval;
	retval = 0;
#if 1
	{
		struct TPADTBL {
			int no;
			unsigned long keymask;
			unsigned long padbit;
		};
		struct TPADTBL PadTbl[] = {
#if __BIG_ENDIAN__
			{3,0x00000040,JOY_PAD_UP},		//UP
			{3,0x00000020,JOY_PAD_DOWN},	//DOWN
			{3,0x00000010,JOY_PAD_RIGHT},	//RIGHT
			{3,0x00000008,JOY_PAD_LEFT},	//LEFT
			{0,0x40000000,JOY_PAD_Z},		//Z
			{0,0x80000000,JOY_PAD_X},		//X
			{0,0x00010000,JOY_PAD_CONTROL},	//C
			{0,0x00020000,JOY_PAD_CONTROL},	//V
			{1,0x00000200,JOY_PAD_SPACE},	//SPACE
			{1,0x00002000,JOY_PAD_ESC},		//ESC
			{1,0x10000000,JOY_PAD_RETURN},	//RETURN
			{1,0x00000001,JOY_PAD_SHIFT},	//SHIFT
			{1,0x00000008,JOY_PAD_CONTROL},	//CONTROL
			{2,0x00000800,JOY_PAD_LEFT},	//テンキーの1
			{2,0x00000800,JOY_PAD_DOWN},	//テンキーの1
			{2,0x00000800,JOY_PAD_1},		//テンキーの1
			{2,0x00001000,JOY_PAD_DOWN},	//テンキーの2
			{2,0x00001000,JOY_PAD_2},		//テンキーの2
			{2,0x00002000,JOY_PAD_RIGHT},	//テンキーの3
			{2,0x00002000,JOY_PAD_DOWN},	//テンキーの3
			{2,0x00002000,JOY_PAD_3},		//テンキーの3
			{2,0x00004000,JOY_PAD_LEFT},	//テンキーの4
			{2,0x00004000,JOY_PAD_4},		//テンキーの4
			{2,0x00008000,JOY_PAD_5},		//テンキーの5
			{2,0x00008000,JOY_PAD_DOWN},	//テンキーの5
			{2,0x00000001,JOY_PAD_RIGHT},	//テンキーの6
			{2,0x00000001,JOY_PAD_6},		//テンキーの6
			{2,0x00000002,JOY_PAD_LEFT},	//テンキーの7
			{2,0x00000002,JOY_PAD_UP},		//テンキーの7
			{2,0x00000002,JOY_PAD_7},		//テンキーの7
			{2,0x00000008,JOY_PAD_UP},		//テンキーの8
			{2,0x00000008,JOY_PAD_8},		//テンキーの8
			{2,0x00000010,JOY_PAD_RIGHT},	//テンキーの9
			{2,0x00000010,JOY_PAD_UP},		//テンキーの9
			{2,0x00000010,JOY_PAD_9},		//テンキーの9
			{1,0x04000000,JOY_PAD_UP},		//I
			{1,0x40000000,JOY_PAD_LEFT},	//J
			{1,0x20000000,JOY_PAD_RIGHT},	//L
			{1,0x00400000,JOY_PAD_DOWN},	//M
			{1,0x00000004,JOY_PAD_OPTION},	//M
#else
			{3,0x40000000,JOY_PAD_UP},		//UP
			{3,0x20000000,JOY_PAD_DOWN},	//DOWN
			{3,0x10000000,JOY_PAD_RIGHT},	//RIGHT
			{3,0x08000000,JOY_PAD_LEFT},	//LEFT
			{0,0x00000040,JOY_PAD_Z},		//Z
			{0,0x00000080,JOY_PAD_X},		//X
			{0,0x00000100,JOY_PAD_CONTROL},	//C
			{0,0x00000200,JOY_PAD_CONTROL},	//V
			{1,0x00020000,JOY_PAD_SPACE},	//SPACE
			{1,0x00200000,JOY_PAD_ESC},		//ESC
			{1,0x00000010,JOY_PAD_RETURN},	//RETURN
			{1,0x01000000,JOY_PAD_SHIFT},	//SHIFT
			{1,0x08000000,JOY_PAD_CONTROL},	//CONTROL

			{2,0x00000800,JOY_PAD_LEFT},	//テンキーの1
			{2,0x00000800,JOY_PAD_DOWN},	//テンキーの1
			{2,0x00000800,JOY_PAD_1},		//テンキーの1
			{2,0x00001000,JOY_PAD_DOWN},	//テンキーの2
			{2,0x00001000,JOY_PAD_2},		//テンキーの2
			{2,0x00002000,JOY_PAD_RIGHT},	//テンキーの3
			{2,0x00002000,JOY_PAD_DOWN},	//テンキーの3
			{2,0x00002000,JOY_PAD_3},		//テンキーの3
			{2,0x00004000,JOY_PAD_LEFT},	//テンキーの4
			{2,0x00004000,JOY_PAD_4},		//テンキーの4
			{2,0x00008000,JOY_PAD_5},		//テンキーの5
			{2,0x00008000,JOY_PAD_DOWN},	//テンキーの5
			{2,0x00000001,JOY_PAD_RIGHT},	//テンキーの6
			{2,0x00000001,JOY_PAD_6},		//テンキーの6
			{2,0x00000002,JOY_PAD_LEFT},	//テンキーの7
			{2,0x00000002,JOY_PAD_UP},		//テンキーの7
			{2,0x00000002,JOY_PAD_7},		//テンキーの7
			{2,0x00000008,JOY_PAD_UP},		//テンキーの8
			{2,0x00000008,JOY_PAD_8},		//テンキーの8
			{2,0x00000010,JOY_PAD_RIGHT},	//テンキーの9
			{2,0x00000010,JOY_PAD_UP},		//テンキーの9
			{2,0x00000010,JOY_PAD_9},		//テンキーの9
			{1,0x04000000,JOY_PAD_UP},		//I
			{1,0x40000000,JOY_PAD_LEFT},	//J
			{1,0x20000000,JOY_PAD_RIGHT},	//L
			{1,0x00400000,JOY_PAD_DOWN},	//M
			{1,0x00000004,JOY_PAD_OPTION},	//M
#endif
		};
		KeyMap theKeys;
		unsigned long *ptr;
		int i;

		GetKeys(theKeys);
		ptr = (unsigned long *)&theKeys;
		for (i=0;i<sizeof(PadTbl)/sizeof(struct TPADTBL);i++) {
			unsigned long k = *((unsigned long*)&theKeys[PadTbl[i].no]);
			if (k & PadTbl[i].keymask)
				retval |= PadTbl[i].padbit;
		}
	}
#endif
	return retval;
}

@implementation PPGameView

@synthesize g;
@synthesize game;
@synthesize touchLocation,touchScreen;

+ (NSOpenGLPixelFormat*)defaultPixelFormat
{
    NSOpenGLPixelFormatAttribute attributes [] = {
        NSOpenGLPFAWindow,
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFASampleBuffers, 1, 
        NSOpenGLPFASamples, 2,
		//NSOpenGLCPSwapInterval,1,
        NSOpenGLPFANoRecovery,
        (NSOpenGLPixelFormatAttribute)nil
    };
    return [[[NSOpenGLPixelFormat alloc] 
                        initWithAttributes:attributes] autorelease];
}

-(id) initWithFrame: (NSRect) frameRect
{
    NSOpenGLPixelFormat *pixelFormat = [PPGameView defaultPixelFormat];
    self = [super initWithFrame: frameRect pixelFormat: pixelFormat];
    return self;
}

/*
- (id)initWithFrame:(NSRect)frame {
	// Pixel Format Attributes for the View-based (non-FullScreen) NSOpenGLContext
	NSOpenGLPixelFormatAttribute attrs[] = {

		// Specifying "NoRecovery" gives us a context that cannot fall back to the software renderer.  This makes the View-based context a compatible with the fullscreen context, enabling us to use the "shareContext" feature to share textures, display lists, and other OpenGL objects between the two.
		NSOpenGLPFANoRecovery,

		// Attributes Common to FullScreen and non-FullScreen
		NSOpenGLPFAColorSize, (NSOpenGLPixelFormatAttribute)32,
		NSOpenGLPFADepthSize, (NSOpenGLPixelFormatAttribute)32,
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFAAccelerated,
		(NSOpenGLPixelFormatAttribute)0
	};
	long rendererID;

	// Create our non-FullScreen pixel format.
	NSOpenGLPixelFormat* pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];

	// Just as a diagnostic, report the renderer ID that this pixel format binds to.  CGLRenderers.h contains a list of known renderers and their corresponding RendererID codes.
	[pixelFormat getValues:&rendererID forAttribute:NSOpenGLPFARendererID forVirtualScreen:0];
	//NSLog(@"NSOpenGLView pixelFormat RendererID = %08x", (unsigned)rendererID);

	[self registerForDraggedTypes:[NSArray arrayWithObjects:NSFilenamesPboardType, nil]];

	return [super initWithFrame:frame pixelFormat:pixelFormat];
}
*/

- (void)drawRect:(NSRect)rect {
	[game setView:self];

	GLint vblSynch = 1;
	[[self openGLContext] setValues:&vblSynch forParameter: NSOpenGLCPSwapInterval];

	unsigned long key = H_GetImdKey();
	
	if (_initialize == false) {
		//sixaxis_init();

		//game = new QBRoboWar();

		if (g == NULL) {
			g = new PPGameSplite();
		}

		_initialize = true;
		//[self loadTexture];
		
		QBSound_Start(4);
		QBSound_Reset();
	}

#if 0
	//if (sixaxis_idle()) 
	{
		int f = 0;
		for (int i=0;i<256;i++) {
			joyf[i]=0;
			joy[i]=0;
		}
		if (!HIDHaveDeviceList ()) {
			if (0 != HIDBuildDeviceList (kHIDPage_GenericDesktop, 0)) {
				f = 1;
			}
		} else {
			f = 1;
		}
		 if (f) {
			{
				pRecDevice pDevice = HIDGetFirstDevice();
				while (pDevice) {
#if 0
					if (pDevice->vendorID == kOurVendorID && pDevice->productID == kOurProductID) {
						pRecElement pElement = HIDGetFirstDeviceElement(pDevice,kHIDElementTypeInput);
						while (pElement) {
							if (pElement) {
								int j = HIDGetElementValue(pDevice,pElement);
								switch (pElement->usage) {
								case JOY_AX1:
									adx = j-128;
									break;
								case JOY_AY1:
									ady = j-128;
									break;
								case JOY_AX2:
								case JOY_AY2:
									{
										int i = joy[pElement->usage] - (j-128);
										if (i < 0) i = -i;
										if (i > 100) {
											joy[pElement->usage] = 0;
										} else {
											joy[pElement->usage] = j-128;
										}
									}
									break;
								default:
									if (joyf[pElement->usage] == 0) {
										joyf[pElement->usage] = 1;
										joy[pElement->usage] = j;
									}
									break;
								}
							}
							pElement = HIDGetNextDeviceElement(pElement,kHIDElementTypeInput); 
						}
					} else
#endif
					if ((pDevice->usage == kHIDUsage_GD_Joystick || pDevice->usage == kHIDUsage_GD_GamePad) && pDevice->usagePage == 1) {
//printf("%x,%d,%d\n",(unsigned long)pDevice->locID,(unsigned long)pDevice->usage,(unsigned long)pDevice->usagePage);
						pRecElement pElement = HIDGetFirstDeviceElement(pDevice,kHIDElementTypeInput);
//printf("buttons=%d,hats=%d,",pDevice->buttons,pDevice->hats);
						while (pElement) {
							if (pElement) {
								int value = HIDGetElementValue(pDevice,pElement);
								int min = pElement->scaledMin;
								int max = pElement->scaledMax;
								switch (pElement->usage) {
								case JOY_AX1:
/*
printf("(%d,%d,(%d,%d),(%d,%d),(%d,%d),%d)"
	,pElement->usage,(unsigned char)value
	,(unsigned char)min,(unsigned char)max
	,pElement->min,pElement->max
	,pElement->calMin,pElement->calMax
	,pElement->size);
*/
									if (value > max - (max - min)/3) {
										//joy[JOY_RIGHT1] = 1;
										key |= JOY_PAD_RIGHT;
									} else
									if (value < min + (max - min)/3) {
										//joy[JOY_LEFT1] = 1;
										key |= JOY_PAD_LEFT;
									}
									break;
								case JOY_AY1:
//printf("(%d,%d,%d,%d,%d,%d,%d)",pElement->usage,value,min,pElement->min,max,pElement->max,pElement->size);
									if (value > max - (max - min)/3) {
										//joy[JOY_DOWN1] = 1;
										key |= JOY_PAD_DOWN;
									} else
									if (value < min + (max - min)/3) {
										//joy[JOY_UP1] = 1;
										key |= JOY_PAD_UP;
									}
									break;
								case JOY_AX2:
//printf("(%d,%d,%d,%d,%d,%d)",pElement->usage,value,min,pElement->min,max,pElement->max);
									if (value > max - (max - min)/3) {
										//joy[JOY_RIGHT2] = 1;
									} else
									if (value < min + (max - min)/3) {
										//joy[JOY_LEFT2] = 1;
									}
									break;
								case JOY_AY2:
//printf("(%d,%d,%d,%d,%d,%d)",pElement->usage,value,min,pElement->min,max,pElement->max);
									if (value > max - (max - min)/3) {
										//joy[JOY_DOWN2] = 1;
									} else
									if (value < min + (max - min)/3) {
										//joy[JOY_UP2] = 1;
									}
									break;
								case JOY_POV:
//printf("(%d,%d,%d,%d)",pElement->usage,value,min,max);
//printf("(%d,%d,%d,%d,%d,%d,%d)",pElement->usage,value,min,pElement->min,max,pElement->max,pElement->size);
									if (value >= pElement->min && value <= pElement->max) {
										value -= pElement->min;
										switch (value) {
										case 0:
											//joy[JOY_UP1] = 1;
											key |= JOY_PAD_UP;
											break;
										case 1:
											//joy[JOY_UP1] = 1;
											//joy[JOY_RIGHT1] = 1;
											key |= JOY_PAD_UP;
											key |= JOY_PAD_RIGHT;
											break;
										case 2:
											//joy[JOY_RIGHT1] = 1;
											key |= JOY_PAD_RIGHT;
											break;
										case 3:
											//joy[JOY_RIGHT1] = 1;
											//joy[JOY_DOWN1] = 1;
											key |= JOY_PAD_RIGHT;
											key |= JOY_PAD_DOWN;
											break;
										case 4:
											//joy[JOY_DOWN1] = 1;
											key |= JOY_PAD_DOWN;
											break;
										case 5:
											//joy[JOY_DOWN1] = 1;
											//joy[JOY_LEFT1] = 1;
											key |= JOY_PAD_DOWN;
											key |= JOY_PAD_LEFT;
											break;
										case 6:
											//joy[JOY_LEFT1] = 1;
											key |= JOY_PAD_LEFT;
											break;
										case 7:
											//joy[JOY_LEFT1] = 1;
											//joy[JOY_UP1] = 1;
											key |= JOY_PAD_LEFT;
											key |= JOY_PAD_UP;
											break;
										}
									}
									break;
								default:
//printf("(%d,%d)",pElement->usage,value);
									if (pDevice->vendorID == kOurVendorID && pDevice->productID == kOurProductID) {
										switch (pElement->usage) {
										case JOY_UP1:
											if (value) key |= JOY_PAD_UP;
											break;
										case JOY_DOWN1:
											if (value) key |= JOY_PAD_DOWN;
											break;
										case JOY_LEFT1:
											if (value) key |= JOY_PAD_LEFT;
											break;
										case JOY_RIGHT1:
											if (value) key |= JOY_PAD_RIGHT;
											break;
										default:
											if (joyf[pElement->usage] == 0) {
												joyf[pElement->usage] = 1;
												joy[pElement->usage] = value;
											}
											break;
										}
									} else {
										if (joyf[pElement->usage] == 0) {
											joyf[pElement->usage] = 1;
											joy[pElement->usage] = value;
										}
									}
/*
									if (joyf[pElement->usage] == 0) {
										joyf[pElement->usage] = 1;
										joy[pElement->usage] = j;
									}
*/
									break;
								}
//printf("%d(%x,%x,%x)",(unsigned long)pElement->usage,(unsigned long)j,(unsigned long)pElement->scaledMin,(unsigned long)pElement->scaledMax);
							}
							pElement = HIDGetNextDeviceElement(pElement,kHIDElementTypeInput); 
						}
//printf("\n");
					}
					pDevice = HIDGetNextDevice(pDevice);
				}
			}
		}
	}
#endif
	
	//for (int i=64;i<128;i++) {
	//	printf("%d,",joy[i]);
	//}
//printf("\n");

	if (joy[JOY_RIGHT2]) {
		key |= JOY_PAD_SPACE;
	}
	if (joy[JOY_DOWN2]) {
		key |= JOY_PAD_Z;
	}
#if 0
	if (joy[JOY_UP1]) {
		key |= JOY_PAD_UP;
	}
	if (joy[JOY_DOWN1]) {
		key |= JOY_PAD_DOWN;
	}
	if (joy[JOY_LEFT1]) {
		key |= JOY_PAD_LEFT;
	}
	if (joy[JOY_RIGHT1]) {
		key |= JOY_PAD_RIGHT;
	}
	if (joy[JOY_RIGHT2]) {
		key |= JOY_PAD_SPACE;
	}
	if (joy[JOY_LEFT2]) {
		key |= JOY_PAD_X;
	}
	if (joy[JOY_DOWN2]) {
		key |= JOY_PAD_Z;
	}
	if (joy[JOY_UP2]) {
		key |= JOY_PAD_SHIFT;
	}
	if (joy[JOY_SELECT]) {
//printf("select\n");
		key |= JOY_PAD_CONTROL;
	}
	if (joy[JOY_START]) {
		key |= JOY_PAD_RETURN;
	}
#endif
	
	if (![NSApp isActive]) key = 0;

	NSRect viewPort = [self bounds];

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glViewport(0,0,640,400);
	glViewport((long)viewPort.origin.x, (long)viewPort.origin.y, (long)viewPort.size.width, (long)viewPort.size.height);
	//glLoadIdentity();
	glLoadIdentity();

	glOrtho(-1.0f,1.0f,-1.0f,1.0f,-1.0f,1.0f);

	//gluPerspective(40.0, (double)viewPort.size.width / (double)viewPort.size.height, 0.1, 10000.0);
	//gluLookAt(0.0,0.0,0.0,0.0,0.0,-1.0,0.0,-1.0,0.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	
	//glEnable(GL_MULTISAMPLE_ARB);
	//glHint (GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
	
	staticKey = key;

//printf("0x%X\n",staticKey);

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
	
		g->screenWidth = viewPort.size.width;//self.width;//160;
		g->screenHeight = viewPort.size.height;//self.height;//416/2;
	
		g->ClearScreen2D(0.0f, 0.0f, 0.0f);

		if ([game draw] == 0) {
			[game drawOT];
		}
	}

#if 0	
	if (game == NULL) {
		game = QBGameMaster();
		game->InitGame();
		H_InitMovie();
	}
	if (game) {
		game->MainIdle(g,key);
	}
#endif

	//glDisable(GL_MULTISAMPLE_ARB);
#if 0
	H_MovieIdle();
#endif
	[[self openGLContext] flushBuffer];
}

- (void) reshape
{
}

//- (BOOL)resignFirstResponder
//{
//	return YES;
//}

- (BOOL)acceptsFirstResponder
{
	return YES;
}

- (void) keyDown:(NSEvent *)theEvent
{
    // Delegate to our controller object for handling key events.
    //[controller keyDown:theEvent];
}

- (void)dealloc {
	if (g) {
		g->Exit();
		delete g;
		g = NULL;
	}
#if 0
	QBSound_Exit();
#endif
	[super dealloc];
}

#if 0
- (void)exitApplication
{
	if (game) {
		game->ExitGame();
	}
}
#endif

- (int)arrowKey:(GLButton*)button count:(int)count
{
	int n=-1;

	{
		NSEnumerator* e = [touchesSet objectEnumerator];
		NSString* t;
		while (t = [e nextObject]) {
			NSPoint location = NSPointFromString(t);
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
	if (staticKey & JOY_PAD_UP) key |= PAD_UP;
	if (staticKey & JOY_PAD_DOWN) key |= PAD_DOWN;
	if (staticKey & JOY_PAD_LEFT) key |= PAD_LEFT;
	if (staticKey & JOY_PAD_RIGHT) key |= PAD_RIGHT;
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
	if (staticKey & JOY_PAD_UP) key |= PAD_UP;
	if (staticKey & JOY_PAD_DOWN) key |= PAD_DOWN;
	if (staticKey & JOY_PAD_LEFT) key |= PAD_LEFT;
	if (staticKey & JOY_PAD_RIGHT) key |= PAD_RIGHT;
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
	if (staticKey & JOY_PAD_UP) key |= PAD_UP;
	if (staticKey & JOY_PAD_DOWN) key |= PAD_DOWN;
	if (staticKey & JOY_PAD_LEFT) key |= PAD_LEFT;
	if (staticKey & JOY_PAD_RIGHT) key |= PAD_RIGHT;
	return key;
}

- (bool)touchCheck:(GLButton*)button
{
	NSEnumerator* e = [touchesSet objectEnumerator];
	NSString* t;
	while (t = [e nextObject]) {
		int hx,hy,hw,hh;
		int x,y;
		NSPoint location = NSPointFromString(t);
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

	if (touchScreen) 
	{
		QBRect r;
		int x,y;
		x = touchLocation.x;
		y = touchLocation.y;
		if (r.tri_hitcheck(keyPos,x-dx,y-dy)) {
			touch = true;
		}
	}
	
	return touch;
}

- (unsigned long)staticButton
{
	unsigned long key = 0;
	if (staticKey & JOY_PAD_Z) key |= PAD_A;
	if (staticKey & JOY_PAD_X) key |= PAD_B;
	if (staticKey & JOY_PAD_SPACE) key |= PAD_A;
	return key;
}

- (void)mouseDown:(NSEvent *)theEvent
{
	NSPoint pt;
	NSSize size = [self bounds].size;
	pt = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	touchLocation.x = pt.x;
	touchLocation.y = size.height-pt.y;
	touchScreen = true;

	NSPoint ptt;
	ptt.x = touchLocation.x;
	ptt.y = touchLocation.y;

	[touchesSet release];
	touchesSet = nil;
	if (touchesSet == nil) touchesSet = [[NSMutableSet set] retain];
	[touchesSet addObject:NSStringFromPoint(ptt)];
}

- (void)mouseDragged:(NSEvent *)theEvent
{
	NSPoint pt;
	NSSize size = [self bounds].size;
	pt = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	touchLocation.x = pt.x;
	touchLocation.y = size.height-pt.y;

	NSPoint ptt;
	ptt.x = touchLocation.x;
	ptt.y = touchLocation.y;

	[touchesSet release];
	touchesSet = nil;
	if (touchesSet == nil) touchesSet = [[NSMutableSet set] retain];
	[touchesSet addObject:NSStringFromPoint(ptt)];
}

- (void)mouseUp:(NSEvent *)theEvent
{
	touchScreen = false;

	[touchesSet release];
	touchesSet = nil;
}

@end

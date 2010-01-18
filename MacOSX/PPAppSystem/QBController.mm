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

#import "QBController.h"
#import "PPGame.h"
#define _PG_GLOBAL_
#include "key.h"
#include "font.h"
#include "font2.h"

extern "C" NSString* PPGameTargetClassName();

@implementation QBController

- (void)awakeFromNib
{
	if (_isInitialized == NO) {
		_isInitialized = YES;
		view.game = [[NSClassFromString(PPGameTargetClassName()) alloc] init];
		[view setNeedsDisplay:YES];
		[[NSTimer scheduledTimerWithTimeInterval:1.0/60.0 target:self selector:@selector(doIdle:) userInfo:self repeats:YES] retain];
	}
}

- (void)applicationDidFinishLaunching:(NSNotification*)aNotification
{
	NSNotificationCenter *notificationCenter = [NSNotificationCenter defaultCenter];
	[notificationCenter addObserver: self
						   selector: @selector (closeGame:)
							   name: @"closeGame"
							 object: nil];
	[notificationCenter addObserver: self
						   selector: @selector (windowHorizontal:)
							   name: @"windowHorizontal"
							 object: nil];
	[notificationCenter addObserver: self
						   selector: @selector (windowVertical:)
							   name: @"windowVertical"
							 object: nil];
	[window setFrameUsingName:@"mainWindow"];

	if ([view.game horizontalView]) {
		[[NSNotificationCenter defaultCenter] postNotificationName:@"windowHorizontal" object:nil];
	}

	[window orderFront:self];
}

- (void)applicationWillTerminate:(NSNotification*)aNotification
{
	[window saveFrameUsingName:@"mainWindow"];
}

- (void)doIdle:(NSTimer*)theTimer
{
	[view setNeedsDisplay:YES];
}

- (void)closeGame:(id)notification
{
	[NSApp terminate:self];
}

- (BOOL)windowShouldClose:(NSWindow *)sender
{
	[NSApp terminate:self];
	return NO;
}

- (void)windowHorizontal:(id)notification
{
	NSRect frameRect = [window frame];
	frameRect.size.width = 480;
	frameRect.size.height = 320;
	frameRect = [window frameRectForContentRect:frameRect];
	[window setFrame:frameRect display:YES];
}

- (void)windowVertical:(id)notification
{
	NSRect frameRect = [window frame];
	frameRect.size.width = 320;
	frameRect.size.height = 480;
	frameRect = [window frameRectForContentRect:frameRect];
	[window setFrame:frameRect display:YES];
}

@end

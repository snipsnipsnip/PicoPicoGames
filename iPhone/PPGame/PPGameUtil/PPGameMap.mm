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

#include "PPGameMap.h"

PPGameMap::PPGameMap(const char* path)
{
	NSDictionary* d = [[NSDictionary dictionaryWithContentsOfFile:[NSString stringWithCString:path]] retain];
	map[0].init();
	map[1].init();
	map[2].init();
	map[0].decompress([[d objectForKey:@"bg0"] bytes]);
	map[1].decompress([[d objectForKey:@"bg1"] bytes]);
	map[2].decompress([[d objectForKey:@"bg2"] bytes]);
	[d release];
}

PPGameMap::~PPGameMap()
{
	map[0].freedata();
	map[1].freedata();
	map[2].freedata();
}

/*
#import "PPGameMap.h"

@implementation PPGameMap

- (id)initWithContentsOfFile:(NSString*)path
{
	if (self = [super init]) {
		NSDictionary* d = [[NSDictionary dictionaryWithContentsOfFile:path] retain];
		map[0].init();
		map[1].init();
		map[2].init();
		map[0].decompress([[d objectForKey:@"bg0"] bytes]);
		map[1].decompress([[d objectForKey:@"bg1"] bytes]);
		map[2].decompress([[d objectForKey:@"bg2"] bytes]);
		event = [[d objectForKey:@"event"] retain];
		[d release];
	}
	return self;
}

- (void)dealloc
{
	map[0].freedata();
	map[1].freedata();
	map[2].freedata();
	[event release];
	[super dealloc];
}

- (NSArray*)events
{
	return event;
}

@end
*/

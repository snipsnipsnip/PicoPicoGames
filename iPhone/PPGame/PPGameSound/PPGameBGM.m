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

#import "PPGameBGM.h"

static MPMusicPlayerController *musicPlayer = nil;

@implementation PPGameBGM

@synthesize controller;
@synthesize music;
@synthesize key;
@synthesize selectedPlay;

+ (MPMusicPlayerController*)musicPlayer
{
#if !TARGET_IPHONE_SIMULATOR
	if (musicPlayer == nil) {
		//if (useiPodPlayer()) {
		//	musicPlayer = [[MPMusicPlayerController iPodMusicPlayer] retain];
		//} else {
			musicPlayer = [[MPMusicPlayerController applicationMusicPlayer] retain];
		//}
		[musicPlayer setShuffleMode: MPMusicShuffleModeOff];
		[musicPlayer setRepeatMode: MPMusicRepeatModeAll];
	}
	return musicPlayer;
#else
	return nil;
#endif
}

+ (void)stop
{
	[[PPGameBGM musicPlayer] stop];
}

- (id)init
{
	if (self = [super init]) {
	}
	return self;
}

- (void)dealloc
{
	[key release];
	[music release];
	[super dealloc];
}

- (void)mediaPicker: (MPMediaPickerController *) mediaPicker didPickMediaItems: (MPMediaItemCollection *)mediaItemCollection
{
//NSLog(@"mediaPicker %@",key);
	[controller dismissModalViewControllerAnimated: YES];
	self.music = mediaItemCollection;
	[controller release];
	controller = nil;
	[self saveForKey:self.key];
	if (selectedPlay) [self play];
}

- (void)mediaPickerDidCancel:(MPMediaPickerController *)mediaPicker
{
	[controller dismissModalViewControllerAnimated: YES];
	[controller release];
	controller = nil;
}

- (void)selectBGM:(UIViewController*)_controller
{
//NSLog(@"selectBGM %@",key);
	MPMediaPickerController *picker = [[MPMediaPickerController alloc] initWithMediaTypes: MPMediaTypeAnyAudio];
	picker.delegate						= self;
	picker.allowsPickingMultipleItems	= YES;
	picker.prompt						= nil;//NSLocalizedString (@"Add songs to play", "Prompt in media item picker");
	self.controller = _controller;
	[_controller presentModalViewController: picker animated: YES];
	[picker release];
}

- (MPMusicPlayerController*)musicPlayer
{
	return [PPGameBGM musicPlayer];
}

- (void)play
{
	if (self.music) {
		[self.musicPlayer setQueueWithItemCollection:music];
		self.musicPlayer.currentPlaybackTime = 0;
		[self.musicPlayer setRepeatMode:MPMusicRepeatModeAll];
		[self.musicPlayer play];
	} else {
		[self stop];
	}
}

- (void)playOneTime
{
	if (self.music) {
		[self.musicPlayer setQueueWithItemCollection:music];
		self.musicPlayer.currentPlaybackTime = 0;
		[self.musicPlayer setRepeatMode:MPMusicRepeatModeNone];
		[self.musicPlayer play];
	} else {
		[self stop];
	}
}

- (void)stop
{
	[self.musicPlayer stop];
}

- (void)reset
{
//NSLog(@"reset");
	self.music = nil;
//	[self saveForKey:self.key];
}

- (bool)saveForKey:(NSString*)keyName
{
	if (keyName) {
		if (self.music) {
			NSMutableArray* a = [[NSMutableArray array] retain];
			for (MPMediaItem* item in self.music.items) {
				[a addObject:[item valueForProperty:MPMediaItemPropertyPersistentID]];
			}
			[[NSUserDefaults standardUserDefaults] setObject:a forKey:keyName];
//NSLog(@"save %@",keyName);
//NSLog([a description]);
			[a release];
			return true;
		} else {
			[[NSUserDefaults standardUserDefaults] removeObjectForKey:keyName];
		}
	}
	return false;
}

- (bool)loadForKey:(NSString*)keyName
{
	if (keyName) {
//NSLog(@"load %@",keyName);
		NSArray* a = [[NSUserDefaults standardUserDefaults] objectForKey:keyName];
//NSLog([a description]);
		if ([a isKindOfClass:[NSArray class]]) {
			NSMutableArray* items = [[NSMutableArray array] retain];
			for (NSNumber* persistentId in a) {
				MPMediaQuery* query = [[MPMediaQuery alloc] init];
				[query addFilterPredicate:[MPMediaPropertyPredicate predicateWithValue:persistentId forProperty:MPMediaItemPropertyPersistentID]];
//NSLog([persistentId description]);
				if (query.items != nil) {
					if (query.items.count > 0) {
						[items addObjectsFromArray:query.items];
					}
				}
				[query release];
			}
			if (items.count > 0) {
				self.music = [MPMediaItemCollection collectionWithItems:items];
			} else {
				self.music = nil;
			}
//NSLog([self.music.items description]);
			[items release];
			return true;
		}
	}
	return false;
}

@end

//
//  ShadowBusterView.m
//  PicoPicoGamePack
//
//  Created by 山口 博光 on 09/08/07.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "PPPicoQuest.h"
#import "QBPicoQuest.h"
#import "shadowChar.h"
#import "key.h"
#import "font.h"

static PPGameTextureInfo texList[] = {
	{"shadowChar.png",shadowChar_group,false},
	{"font.png",font_group,false},
	{"key.png",key_group,false},
	{NULL,NULL,false}
};

@implementation PPPicoQuest

- (PPGameTextureInfo*)textures
{
	return texList;
}

- (bool)start
{
	if (game == NULL) {
		game = new QBPicoQuest();
	}
	return true;
}

- (bool)exit
{
//NSLog(@"carrace exit");
	if (game) {
		delete game;
	}
	game = NULL;
	return true;
}

- (int)idle
{
	[self doGameIdle:game];
	[self drawKey:QBCHAR_KEY];
	return 0;
}

- (int)controllerLayout
{
	return QBGAME_CONTROLLER_TYPE_H_LEFT;
}

@end

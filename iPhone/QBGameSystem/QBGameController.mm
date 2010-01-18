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

#import "QBGameController.h"

@implementation QBGameController

- (PPGameTextureInfo*)textures
{
	[self game];
	return game->TextureInfo();
}

- (bool)start
{
	[self game];
	return true;
}

- (bool)exit
{
	if (game) {
		delete game;
	}
	game = NULL;
	return true;
}

- (void)touchMenu:(QBMenu*)menu
{
	if (menu->disp && menu->touchOK) {
		//int touchCur = -1;
		menu->select = false;
		menu->touch = false;
		for (int i=0;menu->string[i]!=NULL;i++) {
			float pos[8][2] = {
				{0,0},
				{32,0},
				{32,32},
				{0,0},

				{0,0},
				{32,32},
				{0,32},
				{0,0},
			};
			menu->touchRect(&pos[0][0],i);
			if ([view touchCheckWithPoly:&pos[0][0] x:0 y:0]) {
				menu->touchCur = i;
				menu->touch = true;
			}
			if ([view touchCheckWithPoly:&pos[4][0] x:0 y:0]) {
				menu->touchCur = i;
				menu->touch = true;
			}
		}
		if (menu->touch == false && menu->pretouch == true) {
			if (menu->cur == menu->touchCur) {
				menu->select = true;
			} else {
				menu->play();
			}
			menu->cur = menu->touchCur;
		}
		menu->pretouch = menu->touch;
	} else {
		menu->select = false;
	}
	menu->disp = false;
}

- (void)setupKey
{
	int layout = [self controllerLayout];
	switch (layout) {
	case QBGAME_CONTROLLER_TYPE_VERTICAL:
		if (PPGame_GetDefault(PPGAME_KEY_SIDE_RIGHT)) {
			layout = QBGAME_CONTROLLER_TYPE_V_RIGHT;
		} else {
			layout = QBGAME_CONTROLLER_TYPE_V_LEFT;
		}
		break;
	case QBGAME_CONTROLLER_TYPE_HORIZONTAL:
		if (PPGame_GetDefault(PPGAME_KEY_SIDE_RIGHT)) {
			layout = QBGAME_CONTROLLER_TYPE_H_RIGHT;
		} else {
			layout = QBGAME_CONTROLLER_TYPE_H_LEFT;
		}
		break;
	default:
		break;
	}
	switch (layout) {
	case QBGAME_CONTROLLER_TYPE_V_LEFT:
		arrowKeyNo = PG_BUTTON_VLC;
		buttonANo = PG_BUTTON_VL1;
		buttonBNo = PG_BUTTON_VL2;
		break;
	case QBGAME_CONTROLLER_TYPE_V_RIGHT:
		arrowKeyNo = PG_BUTTON_VRC;
		buttonANo = PG_BUTTON_VR1;
		buttonBNo = PG_BUTTON_VR2;
		break;
	case QBGAME_CONTROLLER_TYPE_H_LEFT:
		arrowKeyNo = PG_BUTTON_HLC;
		buttonANo = PG_BUTTON_HL1;
		buttonBNo = PG_BUTTON_HL2;
		break;
	case QBGAME_CONTROLLER_TYPE_H_RIGHT:
		arrowKeyNo = PG_BUTTON_HRC;
		buttonANo = PG_BUTTON_HR1;
		buttonBNo = PG_BUTTON_HR2;
		break;
	case QBGAME_CONTROLLER_TYPE_VERTICAL_CENTER:
	default:
		arrowKeyNo = PG_BUTTON_C;
		buttonANo = PG_BUTTON_1;
		buttonBNo = PG_BUTTON_2;
		break;
	}
}

- (unsigned long)getKey
{
	unsigned long key = 0;
	
	[self setupKey];

	if ([self buttonVisible] & QBGAME_CONTROLLER_ENABLE_ARROW) {
		GLButton b(key_group,arrowKeyNo,0);
		if (game->KeyCount() == 4) {
			key |= [view arrowKey4:&b];
		} else {
			key |= [view arrowKey8:&b];
		}
	}
	if ([self buttonVisible] & QBGAME_CONTROLLER_ENABLE_A) {
		GLButton b(key_group,buttonANo,0);
		if ([view touchCheck:&b]) {
			key |= PAD_A;
		}
	}
	if ([self buttonVisible] & QBGAME_CONTROLLER_ENABLE_B) {
		GLButton b(key_group,buttonBNo,0);
		if ([view touchCheck:&b]) {
			key |= PAD_B;
		}
	}
	
	key |= [view staticButton]; 

	return key;
}

- (void)drawKey:(int)textureId
{
	[self setupKey];
	int i = polyCount;
	if ([self buttonVisible] & QBGAME_CONTROLLER_ENABLE_ARROW) {
		poly[i].color(255, 255, 255);
		poly[i].alpha = 128;
		poly[i++].splite(0,0,0,arrowKeyNo,textureId);
	}
	if ([self buttonVisible] & QBGAME_CONTROLLER_ENABLE_A) {
		poly[i].color(255, 255, 255);
		poly[i].alpha = 128;
		poly[i++].splite(0,0,0,buttonANo,textureId);
	}
	if ([self buttonVisible] & QBGAME_CONTROLLER_ENABLE_B) {
		poly[i].color(255, 255, 255);
		poly[i].alpha = 128;
		poly[i++].splite(0,0,0,buttonBNo,textureId);
	}
	polyCount = i;
}

- (void)doGameIdle
{
	if (game) {
		game->setTouchCount(0);
#if TARGET_OS_IPHONE
		NSMutableSet* touchesSet = [view touchesSet];
		NSEnumerator* e = [touchesSet objectEnumerator];
		UITouch* t;
		while (t = [e nextObject]) {
			CGPoint location = [t locationInView:view];
			game->setTouchPosition(location.x,location.y);
		}
#else
		if (view.touchScreen) {
			game->setTouchPosition(view.touchLocation.x,view.touchLocation.y);
		}
#endif
		[self touchMenu:&game->menu[game->curMenu]];
	}
	unsigned long key = [self getKey];
	if (game) {
		//game->poly = poly;
		//game->key = key;
		//game->text->idle(view.g,game->stringID());
		polyCount = game->gameIdle(key,poly,view.g,self.controller);
		//[self bgmIdle];
		//game->bgmSettingIdle(self.controller);
		//game->bgmSetting();
		//game->prekey = key;
		//polyCount = game->polyCount;
		if (game->KeyTextureID() >= 0) [self drawKey:game->KeyTextureID()];
		if (game->exitGame) {
			game->exitGame = false;
			[self closeGame];
		}
	}
}

/*
- (void)bgmIdle
{
}
*/

- (BOOL)horizontalView
{
	switch ([self controllerLayout]) {
	case QBGAME_CONTROLLER_TYPE_HORIZONTAL:
	case QBGAME_CONTROLLER_TYPE_H_LEFT:
	case QBGAME_CONTROLLER_TYPE_H_RIGHT:
		break;
	case QBGAME_CONTROLLER_TYPE_VERTICAL:
	case QBGAME_CONTROLLER_TYPE_V_LEFT:
	case QBGAME_CONTROLLER_TYPE_V_RIGHT:
	case QBGAME_CONTROLLER_TYPE_VERTICAL_CENTER:
	default:
		return NO;
	}
	return YES;
}

- (int)controllerLayout
{
	[self game];
	if (game) {
		return game->WindowLayout();
	}
	return 0;
}

- (unsigned long)buttonVisible
{
	[self game];
	if (game) {
		return game->VisibleButton();
	}
	return 0;
}

- (int)crosskeyAreaSize
{
	[self game];
	if (game) {
		return game->KeySize();
	}
	return 64*64;
}

- (QBGame*)game
{
	return nil;
}

- (int)idle
{
	[self doGameIdle];
	return 0;
}

@end

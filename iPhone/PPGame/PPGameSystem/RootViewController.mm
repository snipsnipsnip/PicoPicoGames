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

#import "RootViewController.h"
#import "PPGameViewController.h"
#import "PPGameSound.h"
#import "PPGameUtil.h"
#import "DescViewController.h"
#import "KeyConfigViewController.h"

#define kSwitchButtonWidth		94.0
#define kSwitchButtonHeight		27.0
#define kSegmentControllerWidth		120.0
#define kSegmentControllerHeight		32.0

@implementation RootViewController

- (void)create_UISwitch
{
	if (soundSwitch == nil) {
		CGRect frame = CGRectMake(0.0, 0.0, kSwitchButtonWidth, kSwitchButtonHeight);
		soundSwitch = [[UISwitch alloc] initWithFrame:frame];
		[soundSwitch addTarget:self action:@selector(soundSwitchAction:) forControlEvents:UIControlEventValueChanged];
		soundSwitch.backgroundColor = [UIColor clearColor];
	}
	if (manualSwitch == nil) {
		CGRect frame = CGRectMake(0.0, 0.0, kSwitchButtonWidth, kSwitchButtonHeight);
		manualSwitch = [[UISwitch alloc] initWithFrame:frame];
		[manualSwitch addTarget:self action:@selector(manualSwitchAction:) forControlEvents:UIControlEventValueChanged];
		manualSwitch.backgroundColor = [UIColor clearColor];
	}
/*
	if (controllerPositionSegment == nil) {
		CGRect frame = CGRectMake(0.0, 0.0, kSwitchButtonWidth, kSwitchButtonHeight);
		controllerPositionSegment = [[UISegmentedControl alloc] initWithFrame:frame];
		[controllerPositionSegment insertSegmentWithTitle:@"LEFT" atIndex:0 animated:NO];
		[controllerPositionSegment insertSegmentWithTitle:@"RIGHT" atIndex:1 animated:NO];
		controllerPositionSegment.selectedSegmentIndex = 0;
		//[controllerPositionSegment addTarget:self action:@selector(manualSwitchAction:) forControlEvents:UIControlEventValueChanged];
		controllerPositionSegment.backgroundColor = [UIColor clearColor];
	}
*/
}

- (IBAction)soundSwitchAction:(id)sender
{
	NSUserDefaults* def = [NSUserDefaults standardUserDefaults];
	if (soundSwitch.on) {
		[def setObject:@"on" forKey:@"sound"];
NSLog(@"soundSwitchAction on");
	} else {
		[def setObject:@"off" forKey:@"sound"];
NSLog(@"soundSwitchAction off");
	}
	[def synchronize];
}

- (IBAction)manualSwitchAction:(id)sender
{
	NSUserDefaults* def = [NSUserDefaults standardUserDefaults];
	if (manualSwitch.on) {
		[def setObject:@"on" forKey:@"manual"];
NSLog(@"manualSwitchAction on");
	} else {
		[def setObject:@"off" forKey:@"manual"];
NSLog(@"manualSwitchAction off");
	}
	[def synchronize];
}

- (void)checkUserDefault
{
	NSUserDefaults* def = [NSUserDefaults standardUserDefaults];
	if ([[def objectForKey:@"sound"] isEqualToString:@"off"]) {
		soundSwitch.on = NO;
	} else {
		soundSwitch.on = YES;
	}
	if ([[def objectForKey:@"manual"] isEqualToString:@"off"]) {
		manualSwitch.on = NO;
	} else {
		manualSwitch.on = YES;
	}
	if ([[def objectForKey:@"keySide"] isEqualToString:@"right"]) {
		controllerPositionCell.detail.text = @"RIGHT";
	} else {
		controllerPositionCell.detail.text = @"LEFT";
	}
}

static int Locale()
{
//	return 1;
	//NSLog([[NSLocale currentLocale] objectForKey:NSLocaleLanguageCode]);
	NSString* code = NSLocalizedString(@"language",@"");
	if ([code isEqualToString:@"jp"]) {
		return 1;
	}
	return 0;
}

- (void)viewDidLoad {

//	CGAffineTransform transform = self.view.transform;

//	transform = CGAffineTransformRotate(transform, (M_PI / 2.0));
//	self.view.transform = transform;

	NSDictionary* optionList = 
				[NSDictionary dictionaryWithObjectsAndKeys:@"Option",@"title",
						[NSMutableArray arrayWithObjects:
	//						[NSMutableDictionary dictionaryWithObjectsAndKeys:@"Controller",@"title",@"",@"keyPosition",@"5.png",@"icon",nil],
							[NSMutableDictionary dictionaryWithObjectsAndKeys:@"How to play",@"title",@"",@"manualSwitch",@"m.png",@"icon",nil],
							[NSMutableDictionary dictionaryWithObjectsAndKeys:@"Sound",@"title",@"GameSysSound",@"switch",@"5.png",@"icon",nil],
							[NSMutableDictionary dictionaryWithObjectsAndKeys:@"Yamagame's Homepage",@"title",@"y.png",@"icon",@"",@"gopage",nil],
							nil],@"row",nil];

	if (Locale()) {
		gameList = [[NSMutableArray arrayWithObjects:
				[NSDictionary dictionaryWithObjectsAndKeys:@"Information",@"title",
						[NSMutableArray arrayWithObjects:
							[NSMutableDictionary dictionaryWithObjectsAndKeys:@"OREBSUTER2",@"title",@"ore2.png",@"icon",@"",@"goinfo",@"orebuster2.html",@"url2",@"itouchad",@"manual",nil],
							nil],@"row",nil],
				[NSDictionary dictionaryWithObjectsAndKeys:@"Games",@"title",
						[NSArray arrayWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"QBGameList" ofType:@"plist"]],
							@"row",nil],
					optionList,
					nil] retain];
	} else {
		gameList = [[NSMutableArray arrayWithObjects:
				[NSDictionary dictionaryWithObjectsAndKeys:@"Games",@"title",
						[NSArray arrayWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"QBGameList" ofType:@"plist"]],
							@"row",nil],
					optionList,
					nil] retain];
	}
	
    [super viewDidLoad];
	
	
	self.navigationItem.title = NSLocalizedString(@"PicoPicoGames",@"");

    // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    // self.navigationItem.rightBarButtonItem = self.editButtonItem;
}


- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
	[self create_UISwitch];
	[self checkUserDefault];
//	[tableView reloadData];
}

/*
- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
}
*/
/*
- (void)viewWillDisappear:(BOOL)animated {
	[super viewWillDisappear:animated];
}
*/

- (void)viewDidDisappear:(BOOL)animated {
	[super viewDidDisappear:animated];
//	[soundSwitch release];
//	soundSwitch = nil;
//	[manualSwitch release];
//	manualSwitch = nil;
	//[controllerPositionSegment release];
	//controllerPositionSegment = nil;
}

#if 0
 // Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
NSLog(@"interfaceOrientation %d",interfaceOrientation);
	// Return YES for supported orientations.
	return NO;//(interfaceOrientation == UIInterfaceOrientationPortrait);
}
#endif


- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release anything that can be recreated in viewDidLoad or on demand.
	// e.g. self.myOutlet = nil;
//NSLog(@"viewDidUnload");
//	[soundSwitch release];
//	soundSwitch = nil;
}


#pragma mark Table view methods

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	return gameList.count;
//	return 1;
}


// Customize the number of rows in the table view.
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return [[[gameList objectAtIndex:section] objectForKey:@"row"] count];
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section {
    return NSLocalizedString([[gameList objectAtIndex:section] objectForKey:@"title"],@"");
}

// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tb cellForRowAtIndexPath:(NSIndexPath *)indexPath {
#if 1
    static NSString *CellIdentifier = @"Cell";

	if ([[[[gameList objectAtIndex:indexPath.section] objectForKey:@"row"] objectAtIndex:indexPath.row] objectForKey:@"keyPosition"]) {
		NSUserDefaults* def = [NSUserDefaults standardUserDefaults];
		KeyConfigCell *cell = controllerPositionCell;
		{
			NSString* c = [[[[gameList objectAtIndex:indexPath.section] objectForKey:@"row"] objectAtIndex:indexPath.row] objectForKey:@"icon"];
			if (c) {
				cell.imageView.image = [UIImage imageNamed:c];
			} else {
				cell.imageView.image = nil;
			}
		}
		
		cell.title.text = NSLocalizedString([[[[gameList objectAtIndex:indexPath.section] objectForKey:@"row"] objectAtIndex:indexPath.row] objectForKey:@"title"],@"");
		if ([[def objectForKey:@"keySide"] isEqualToString:@"right"]) {
			cell.detail.text = NSLocalizedString(@"Right",@"");
		} else {
			cell.detail.text = NSLocalizedString(@"Left",@"");
		}
		cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;

		return cell;
	}
    
    UITableViewCell *cell = (UITableViewCell*)[tb dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
		cell.selectionStyle=UITableViewCellSelectionStyleNone; 
    }
    
	// Configure the cell.
	
	NSString* title = [[[[gameList objectAtIndex:indexPath.section] objectForKey:@"row"] objectAtIndex:indexPath.row] objectForKey:@"title"];
	if (title) {
		cell.textLabel.text = NSLocalizedString(title,@"");
	} else {
		NSString* title = [[[[gameList objectAtIndex:indexPath.section] objectForKey:@"row"] objectAtIndex:indexPath.row] objectForKey:@"class"];
		cell.textLabel.text = title;
	}
//	NSLog(cell.textLabel.font.fontName);
//	NSLog(@"%f",cell.textLabel.font.pointSize);
	cell.accessoryType = UITableViewCellAccessoryNone;
	cell.detailTextLabel.text = nil;

	{
		NSString* c = [[[[gameList objectAtIndex:indexPath.section] objectForKey:@"row"] objectAtIndex:indexPath.row] objectForKey:@"icon"];
		if (c) {
			cell.imageView.image = [UIImage imageNamed:c];
		} else {
			cell.imageView.image = nil;
		}
	}
	
	if ([[[[gameList objectAtIndex:indexPath.section] objectForKey:@"row"] objectAtIndex:indexPath.row] objectForKey:@"switch"]) {
		cell.accessoryView = soundSwitch;
	} else
	if ([[[[gameList objectAtIndex:indexPath.section] objectForKey:@"row"] objectAtIndex:indexPath.row] objectForKey:@"manualSwitch"]) {
		cell.accessoryView = manualSwitch;
	} else
	if ([[[[gameList objectAtIndex:indexPath.section] objectForKey:@"row"] objectAtIndex:indexPath.row] objectForKey:@"keyPosition"]) {
	} else {
		cell.accessoryView = nil;
		cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
	}
#else    
    static NSString *CellIdentifier = @"Cell";
    
    DisplayCell *cell = (DisplayCell*)[tb dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[DisplayCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
    }
    
	// Configure the cell.
	
	cell.textLabel.text = NSLocalizedString([[[[gameList objectAtIndex:indexPath.section] objectForKey:@"row"] objectAtIndex:indexPath.row] objectForKey:@"title"],@"");
	cell.editingAccessoryType = UITableViewCellAccessoryNone;

	{
		NSString* c = [[[[gameList objectAtIndex:indexPath.section] objectForKey:@"row"] objectAtIndex:indexPath.row] objectForKey:@"icon"];
		if (c) {
			cell.imageView.image = [UIImage imageNamed:c];
		} else {
			cell.imageView.image = nil;
		}
	}
	
	if ([[[[gameList objectAtIndex:indexPath.section] objectForKey:@"row"] objectAtIndex:indexPath.row] objectForKey:@"switch"]) {
		cell.switchView = soundSwitch;
	} else
	if ([[[[gameList objectAtIndex:indexPath.section] objectForKey:@"row"] objectAtIndex:indexPath.row] objectForKey:@"manualSwitch"]) {
		cell.switchView = manualSwitch;
	} else {
		cell.switchView = nil;
	}
#endif
    return cell;
}


- (NSIndexPath *)tableView:(UITableView *)tv willSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	if ([[[gameList objectAtIndex:indexPath.section] objectForKey:@"row"] count] > indexPath.row) {
		NSDictionary* d = [[[gameList objectAtIndex:indexPath.section] objectForKey:@"row"] objectAtIndex:indexPath.row];
		if ([d objectForKey:@"class"]) {
			return indexPath;
		}
		if ([d objectForKey:@"gopage"]) {
			return indexPath;
		}
		if ([d objectForKey:@"goinfo"]) {
			return indexPath;
		}
	}
	if ([[[[gameList objectAtIndex:indexPath.section] objectForKey:@"row"] objectAtIndex:indexPath.row] objectForKey:@"keyPosition"]) {
		return indexPath;
	}
	return nil;
}

// Override to support row selection in the table view.
- (void)tableView:(UITableView *)tb didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	if ([[[gameList objectAtIndex:indexPath.section] objectForKey:@"row"] count] > indexPath.row) {
		NSMutableDictionary* gameInfo = [[[gameList objectAtIndex:indexPath.section] objectForKey:@"row"] objectAtIndex:indexPath.row];
		if ([gameInfo objectForKey:@"class"]) {
			NSUserDefaults* def = [NSUserDefaults standardUserDefaults];
			
			PPGame_SetSoundEnable(soundSwitch.on == YES);
			
			if ([[def objectForKey:@"keySide"] isEqualToString:@"right"]) {
				PPGame_SetDefault(PPGAME_KEY_SIDE_RIGHT);
			} else {
				PPGame_SetDefault(PPGAME_KEY_SIDE_LEFT);
			}
			
			if ([gameInfo objectForKey:@"manual"] == nil || manualSwitch.on == NO) {
				NSString* xibName = [gameInfo objectForKey:@"xib"];
				if (xibName == nil) {
					xibName = @"PPGameViewController";
				}
				PPGameViewController *controller = [[PPGameViewController alloc] initWithNibName:xibName bundle:nil];
				controller.gameClassName = [gameInfo objectForKey:@"class"];
				[self.navigationController pushViewController:controller animated:YES];
				[controller release];
			} else {
				DescViewController *controller = [[DescViewController alloc] initWithNibName:@"DescViewController" bundle:nil];
				controller.gameInfo = gameInfo;
				[self.navigationController pushViewController:controller animated:YES];
				[controller release];
			}

		}
		if ([gameInfo objectForKey:@"gopage"]) {
			[self goYamagameHP];
			[tb deselectRowAtIndexPath:indexPath animated:YES];
		}
		if ([gameInfo objectForKey:@"goinfo"]) {
			DescViewController *controller = [[DescViewController alloc] initWithNibName:@"DescViewController" bundle:nil];
			controller.gameInfo = gameInfo;
			[self.navigationController pushViewController:controller animated:YES];
			[controller release];
		}
	}
	if ([[[[gameList objectAtIndex:indexPath.section] objectForKey:@"row"] objectAtIndex:indexPath.row] objectForKey:@"keyPosition"]) {
		KeyConfigViewController *controller = [[KeyConfigViewController alloc] initWithNibName:@"KeyConfigViewController" bundle:nil];
		[self.navigationController pushViewController:controller animated:YES];
		[controller release];
	}
}

- (void)dealloc {
	[soundSwitch release];
	soundSwitch = nil;
	[manualSwitch release];
	manualSwitch = nil;
	//[controllerPositionSegment release];
	//controllerPositionSegment = nil;
	[gameList release];
	gameList = nil;
    [super dealloc];
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
	switch (alertView.tag) {
	case 0:		//アドレスブック
		break;
	case 1:		//サーバー
		break;
	case 2:		//カメラ
		break;
	case 3:		//ホームページ
		if (buttonIndex) {
			[[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"http://yamagame.secret.jp/"]];
		}
		break;
	}
}

- (void)goYamagameHP
{
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"" message:NSLocalizedString(@"The application will be ended.Proceed?",@"")
							delegate:self cancelButtonTitle:NSLocalizedString(@"Cancel",@"") otherButtonTitles:NSLocalizedString(@"OK",@""), nil];
	alert.tag = 3;
	[alert show];
	[alert release];
}

@end


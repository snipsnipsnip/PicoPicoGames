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

#import "PPGameViewController.h"
#define kAccelerometerFrequency     10

enum {
	ORIENTATION_DOWN,
	ORIENTATION_LEFT,
	ORIENTATION_RIGHT,
	ORIENTATION_TOP,
};

@implementation PPGameViewController

@synthesize game;
@synthesize gameClassName;

- (PPGame*)game
{
	if (_game == nil) {
		_game = [[NSClassFromString(gameClassName) alloc] init];
	}
	return _game;
}

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
        // Custom initialization
    }
    return self;
}
*/

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	if (doneButton) self.navigationItem.rightBarButtonItem = doneButton;
	if ([self horizontalView]) {
		[self rotate:[[UIDevice currentDevice] orientation]];
	}
	((PPGameView*)self.view).game = self.game;
}

#define GROW_ANIMATION_DURATION_SECONDS 0.5

- (void)calcOrientation:(UIAcceleration *)acceleration
{
	UIAccelerationValue x = acceleration.x;
	UIAccelerationValue y = acceleration.y;
	int t=0;
	
	if (x < -0.5) {
		t = ORIENTATION_LEFT;
	} else
	if (x >  0.5) {
		t = ORIENTATION_RIGHT;
	}
	if (y < -0.5) {
		//t = ORIENTATION_DOWN;
		return;
	} else
	if (y >  0.5) {
		//t = ORIENTATION_TOP;
		return;
	}
	
	if (nowOrientation != t) {
		orientationTimer ++;
		if (orientationTimer > 10) {
			orientationTimer = 0;
			nowOrientation = t;

NSLog(@"nowOrientation %d",nowOrientation);
#if 1
			[UIView beginAnimations:nil context:NULL];
			[UIView setAnimationDuration:GROW_ANIMATION_DURATION_SECONDS];
			[UIView setAnimationDelegate:self];
			[UIView setAnimationDidStopSelector:@selector(growAnimationDidStop:finished:context:)];
			CGAffineTransform transform;
			switch (nowOrientation) {
			case ORIENTATION_DOWN:
				transform = CGAffineTransformMakeRotation(0);
				break;
			case ORIENTATION_TOP:
				transform = CGAffineTransformMakeRotation(M_PI);
				break;
			case ORIENTATION_LEFT:
				transform = CGAffineTransformMakeRotation(M_PI/2.0);
				break;
			case ORIENTATION_RIGHT:
				transform = CGAffineTransformMakeRotation(M_PI*1.5);
				break;
			}
			self.view.transform = transform;
			[UIView commitAnimations];
#endif
		}
	} else {
		orientationTimer = 0;
	}
}

- (void)accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)acceleration {

	//NSLog(@"x:%f y:%f z:%f",acceleration.x,acceleration.y,acceleration.z);

	//y < -0.5
	//y >  0.5
	//x < -0.5
	//x >  0.5
	
	[self calcOrientation:acceleration];
/*
	if (preAcc) {
		UIAccelerationValue dx = (acceleration.x-preAcc.x);
		UIAccelerationValue dy = (acceleration.y-preAcc.y);
		UIAccelerationValue dz = (acceleration.z-preAcc.z);
		UIAccelerationValue len = sqrt(dx*dx+dy*dy+dz*dz);
	}
*/
	[preAcc release];
	preAcc = [acceleration retain];
}

-(void)rotate:(UIDeviceOrientation)interfaceOrientation
{
	if ([self horizontalView]) {
		if(interfaceOrientation == UIDeviceOrientationLandscapeRight) {
			CGAffineTransform transform;
			transform = CGAffineTransformMakeRotation(M_PI*1.5);
			self.view.transform = transform;
		} else
		if(interfaceOrientation == UIDeviceOrientationLandscapeLeft) {
			CGAffineTransform transform;
			transform = CGAffineTransformMakeRotation(M_PI/2.0);
			self.view.transform = transform;
		}
	}
}

-(void)receivedRotate:(NSNotification*)notification
{
	if ([self horizontalView]) {
		UIDeviceOrientation interfaceOrientation = [[UIDevice currentDevice] orientation];
		if(interfaceOrientation == UIDeviceOrientationLandscapeRight) {
			[UIView beginAnimations:nil context:NULL];
			[UIView setAnimationDuration:GROW_ANIMATION_DURATION_SECONDS];
			[UIView setAnimationDelegate:self];
			[UIView setAnimationDidStopSelector:@selector(growAnimationDidStop:finished:context:)];
			CGAffineTransform transform;
			transform = CGAffineTransformMakeRotation(M_PI*1.5);
			self.view.transform = transform;
			[UIView commitAnimations];
		} else
		if(interfaceOrientation == UIDeviceOrientationLandscapeLeft) {
			[UIView beginAnimations:nil context:NULL];
			[UIView setAnimationDuration:GROW_ANIMATION_DURATION_SECONDS];
			[UIView setAnimationDelegate:self];
			[UIView setAnimationDidStopSelector:@selector(growAnimationDidStop:finished:context:)];
			CGAffineTransform transform;
			transform = CGAffineTransformMakeRotation(M_PI/2.0);
			self.view.transform = transform;
			[UIView commitAnimations];
		}
	}
}

- (void)viewWillAppear:(BOOL)animated {
	NSNotificationCenter *notificationCenter = [NSNotificationCenter defaultCenter];
	[notificationCenter addObserver: self
						   selector: @selector (closeGame:)
							   name: @"closeGame"
							 object: nil];
	nowOrientation = ORIENTATION_LEFT;
    [super viewWillAppear:animated];
	[self.navigationController setNavigationBarHidden:YES animated:YES];
	[((PPGameView*)self.view) startAnimation];

	//[[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1.0 / kAccelerometerFrequency)];
	//[[UIAccelerometer sharedAccelerometer] setDelegate:self];
	
	//[self willRotateToInterfaceOrientation:UIInterfaceOrientationLandscapeLeft duration:0];

	if (initialOrientation == NO) {
		if ([self horizontalView]) {
			CGAffineTransform transform = self.view.transform;
			transform = CGAffineTransformRotate(transform, (M_PI / 2.0));
			self.view.transform = transform;
			//[self rotate:[[UIDevice currentDevice] orientation]];
		}
		initialOrientation = YES;
	}

	if (self.game.initFlag == NO) {
		[self.game setView:((PPGameView*)self.view)];
		[self.game start];
		self.game.initFlag = YES;
	}
	self.game.controller = self;
//	((PPGameView*)self.view).game = self.game;

	[self rotate:[[UIDevice currentDevice] orientation]];
}

- (void)viewWillDisappear:(BOOL)animated {
	[[NSNotificationCenter defaultCenter] removeObserver: self
													name: @"closeGame"
												  object: nil];
    [super viewWillDisappear:animated];
	[self.navigationController setNavigationBarHidden:NO animated:YES];
	[((PPGameView*)self.view) stopAnimation];
	//[game exit];
	
	//[[UIAccelerometer sharedAccelerometer] setDelegate:nil];
	//[self.view exitGame];

	//if ([self horizontalView]) {
	//}
}

#if 0
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	switch (interfaceOrientation) {
    case UIInterfaceOrientationLandscapeLeft:
    case UIInterfaceOrientationLandscapeRight:
		self.view.transform = CGAffineTransformIdentity;
		return YES;
	}
	return NO;
    // Return YES for supported orientations
    //return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
#endif

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
//NSLog(@"PPGameViewController viewDidUnload");
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


- (void)dealloc {
//NSLog(@"PPGameViewController dealloc");
	//[_game exit];
	[self.game release];
    [super dealloc];
}

- (IBAction)doneGame:(id)sender
{
	[self.navigationController popViewControllerAnimated:YES];
}

- (void)exitGame
{
	[self.game exit];
}

- (void)resignActive
{
	[self.game resignActive];
}

- (void)becomeActive
{
	[self.game becomeActive];
}

/*
- (GLGame*)game
{
	return ((PPGameView*)self.view).game;
}

- (void)setGame:(GLGame*)_game
{
	if (game != _game) {
		((PPGameView*)self.view).game = _game;
		game = [_game retain];
	}
}
*/

- (void)closeGame:(id)notification
{
//	((PPGameView*)self.view).game = nil;
//	[_game release];;
//	_game = nil;
	self.game.controller = nil;
	[self.navigationController popViewControllerAnimated:YES];
}

/*
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return UIInterfaceOrientationIsLandscape(interfaceOrientation);
}
*/

- (BOOL)horizontalView
{
	return [self.game horizontalView];
}

@end

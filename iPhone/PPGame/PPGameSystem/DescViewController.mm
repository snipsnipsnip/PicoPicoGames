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

#import "DescViewController.h"
#import "PPGameViewController.h"


@implementation DescViewController

@synthesize gameInfo;
@synthesize startButton;

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
	step = 0;
}

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
	self.navigationController.title = [gameInfo objectForKey:@"title"];
	if (step == 0) {
		loadingView.hidden = NO;
		gameDesc.delegate = self;
		NSURL* url = nil;
		if ([gameInfo objectForKey:@"url2"] != nil) {
			NSMutableString* urls = nil;
			urls = [NSMutableString stringWithString:NSLocalizedString(@"url2",@"")];
			[urls appendString:[gameInfo objectForKey:@"url2"]];
			url = [NSURL URLWithString:urls];
		} else
		if ([gameInfo objectForKey:@"url"] == nil) {
			NSString* path = [[NSBundle mainBundle] pathForResource:[gameInfo objectForKey:@"manual"] ofType:@"html"];
			url = [NSURL fileURLWithPath:path isDirectory:NO];
		} else {
			NSMutableString* urls = nil;
			urls = [NSMutableString stringWithString:NSLocalizedString(@"url",@"")];
			[urls appendString:[gameInfo objectForKey:@"url"]];
			url = [NSURL URLWithString:urls];
		}
		[gameDesc loadRequest:[NSURLRequest requestWithURL:url]];
//NSLog(@"load");
	} else {
		loadingView.hidden = YES;
//NSLog(@"no load");
	}
	if ([gameInfo objectForKey:@"goinfo"] != nil) {
	} else {
		self.startButton.title = NSLocalizedString(@"Start",@"");
		self.navigationItem.rightBarButtonItem = self.startButton;
	}
//	[gameDesc loadRequest:[NSURLRequest requestWithURL:[NSURL fileURLWithPath:[[NSBundle mainBundle] pathForResource:[gameInfo objectForKey:@"manual"] ofType:@"html"] isDirectory:NO]]];
}

- (void)viewDidDisappear:(BOOL)animated {
	gameDesc.delegate = nil;
	[super viewDidDisappear:animated];
}

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	step = 0;
//NSLog(@"viewDidUnload");
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}

- (IBAction)startGame:(id)sender
{
	NSString* xibName = [gameInfo objectForKey:@"xib"];
	if (xibName == nil) {
		xibName = @"PPGameViewController";
	}
	PPGameViewController *controller = [[PPGameViewController alloc] initWithNibName:xibName bundle:nil];
	controller.gameClassName = [gameInfo objectForKey:@"class"];
	[self.navigationController pushViewController:controller animated:YES];
	[controller release];
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
//	loadingView.hidden = YES;
	NSString* path = [[NSBundle mainBundle] pathForResource:[gameInfo objectForKey:@"manual"] ofType:@"html"];
	[gameDesc loadRequest:[NSURLRequest requestWithURL:[NSURL fileURLWithPath:path isDirectory:NO]]];
}

- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
	switch (step) {
	case 0:
#if 0
		{
			UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"" message:[error localizedDescription]
									delegate:self cancelButtonTitle:NSLocalizedString(@"Close",@"") otherButtonTitles:nil];
			[alert show];
			[alert release];
		}
#else
		{
			NSString* path = [[NSBundle mainBundle] pathForResource:[gameInfo objectForKey:@"manual"] ofType:@"html"];
			[gameDesc loadRequest:[NSURLRequest requestWithURL:[NSURL fileURLWithPath:path isDirectory:NO]]];
		}
#endif
		step ++;
		break;
	}
	loadingView.hidden = YES;
}

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType
{
	NSString *compURL = @"http://phobos.apple.com/WebObjects/";
	if(NSOrderedSame == [[[request URL] absoluteString] compare:compURL options:NSCaseInsensitiveSearch range:NSMakeRange(0,[compURL length])]) {
		[[UIApplication sharedApplication] openURL:[request URL]];
	}
	//	NSLog(@"shouldStartLoadWithRequest");
//NSLog(@"%@,%d",[request description],navigationType);
	return YES;
}

- (void)webViewDidFinishLoad:(UIWebView *)webView
{
	loadingView.hidden = YES;
	step ++;
}

- (void)webViewDidStartLoad:(UIWebView *)webView
{
	loadingView.hidden = NO;
}

@end

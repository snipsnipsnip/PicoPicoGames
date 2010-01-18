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

#import "UIImage-FontTexture.h"
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

static CGFloat fontSize = 15;
static UIColor* fontBGColor = nil;
static UIColor* fontColor = nil;

static UIImage* fontImage(char* string)
{
	return [UIImage fontImage:[NSString stringWithUTF8String:string]];
}

@implementation UIImage (FontTexture)

+ (void)setFontSize:(CGFloat)size
{
	fontSize = size;
}

+ (void)setFontImageBGColor:(UIColor*)col
{
	if (fontBGColor != col) {
		[fontBGColor release];
		fontBGColor = [col retain];
	}
}

+ (void)setFontColor:(UIColor*)col
{
	if (fontColor != col) {
		[fontColor release];
		fontColor = [col retain];
	}
}

+ (UIImage*)fontImage:(NSString*)string
{
	CGSize textSize;
	UIImage* image = self;
	UIFont *font = [UIFont boldSystemFontOfSize:fontSize];

	textSize = [string sizeWithFont:font minFontSize:fontSize actualFontSize:&fontSize forWidth:320 lineBreakMode:UILineBreakModeMiddleTruncation];
	textSize.width = fontSize+1;
	textSize.height = fontSize+1;

	UIGraphicsBeginImageContext(textSize);
	CGContextRef context = UIGraphicsGetCurrentContext();
	if (fontBGColor == nil) {
		CGContextSetFillColorWithColor(context,[UIColor blackColor].CGColor);
	} else {
		CGContextSetFillColorWithColor(context,fontBGColor.CGColor);
	}
	CGContextFillRect(context, CGRectMake(0,0,textSize.width,textSize.height));

	CGPoint point;
	if (fontColor == nil) {
		[[UIColor whiteColor] set];
	} else {
		[fontColor set];
	}
	point = CGPointMake(0,0);
	[string drawAtPoint:point forWidth:textSize.width withFont:font fontSize:fontSize lineBreakMode:UILineBreakModeMiddleTruncation baselineAdjustment:UIBaselineAdjustmentAlignBaselines];

	image = UIGraphicsGetImageFromCurrentImageContext();
	
	UIGraphicsEndImageContext();

	return image;
}

@end

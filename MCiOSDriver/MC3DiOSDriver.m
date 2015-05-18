//
//  MC3DiOSDriver.c
//  monkcGame
//
//  Created by SunYuLi on 5/18/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import "MC3DiOSDriver.h"

MCMatrix4 MCMatrix4Multiply(MCMatrix4 matrixLeft, MCMatrix4 matrixRight)
{
    return MCMatrix4FromGLKMatrix4(GLKMatrix4Multiply(MCMatrix4ToGLKMatrix4(matrixLeft),
                                                      MCMatrix4ToGLKMatrix4(matrixRight)));
}

static UIView* _rootUIView = nil;
static UIEventHandler* _handler = nil;

void MCUIRegisterRootUIView(void* rootview)
{
    _rootUIView = (__bridge UIView*)rootview;
    _handler = [UIEventHandler new];
}

void MCUIAddLabelButton(const char* bgname, const char* labelname, MCFloat x, MCFloat y, MCInt tag)
{
    if (_rootUIView) {
        UIButton* btn = [UIButton buttonWithType:UIButtonTypeCustom];
        [btn setTitle:[NSString stringWithCString:labelname encoding:NSUTF8StringEncoding] forState:UIControlStateNormal];
        [btn setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
        [btn sizeToFit];
        btn.center = CGPointMake(x, y);
        btn.tag = tag;
        [btn addTarget:_handler action:@selector(onButtonClicked:) forControlEvents:UIControlEventTouchUpInside];
        [_rootUIView addSubview:btn];
    }
}

#ifdef __OBJC__
@implementation UIEventHandler

- (void) onButtonClicked:(id)sender
{
    UIButton* btn = (UIButton*)sender;
    btn.tag;
    
}

@end
#endif /* __OBJC__ */



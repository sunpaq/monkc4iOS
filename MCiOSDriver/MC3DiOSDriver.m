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
void MCUIRegisterRootUIView(void* rootview)
{
    _rootUIView = (__bridge UIView*)rootview;
}

void MCUIAddLabelButton(const char* bgname, const char* labelname)
{
    if (_rootUIView) {
        UIButton* btn = [UIButton buttonWithType:UIButtonTypeRoundedRect];
        [btn setTitle:[NSString stringWithCString:labelname encoding:NSUTF8StringEncoding] forState:UIControlStateNormal];
        [btn sizeToFit];
        btn.center = CGPointMake(100, 50);
        
        [_rootUIView addSubview:btn];
    }
}

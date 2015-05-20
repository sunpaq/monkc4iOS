//
//  MC3DiOSDriver.h
//  monkcGame
//
//  Created by SunYuLi on 5/18/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#ifndef __monkcGame__MC3DiOSDriver__
#define __monkcGame__MC3DiOSDriver__

#ifdef __OBJC__
#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

@interface UIEventHandler : NSObject
- (void) onButtonClicked:(id)sender;
@end
#endif

#include <stdio.h>
#include "monkc.h"
#include "MC3DType.h"

#define MCMatrix4ToGLKMatrix4(mat) (GLKMatrix4){(mat).m00, (mat).m01, (mat).m02, (mat).m03, \
                                                (mat).m10, (mat).m11, (mat).m12, (mat).m13, \
                                                (mat).m20, (mat).m21, (mat).m22, (mat).m23, \
                                                (mat).m30, (mat).m31, (mat).m32, (mat).m33}

#define MCMatrix4FromGLKMatrix4(mat) (MCMatrix4){(mat).m00, (mat).m01, (mat).m02, (mat).m03, \
                                                 (mat).m10, (mat).m11, (mat).m12, (mat).m13, \
                                                 (mat).m20, (mat).m21, (mat).m22, (mat).m23, \
                                                 (mat).m30, (mat).m31, (mat).m32, (mat).m33}

MCMatrix4 MCMatrix4Multiply(MCMatrix4 matrixLeft, MCMatrix4 matrixRight);
MCMatrix4 MCMatrix4MakeLookAt(MCFloat eyeX, MCFloat eyeY, MCFloat eyeZ,
                              MCFloat centerX, MCFloat centerY, MCFloat centerZ,
                              MCFloat upX, MCFloat upY, MCFloat upZ);

//UI parts use UIKit
void MCUIRegisterRootUIView(void* rootview);
void MCUIAddLabelButton(const char* bgname, const char* labelname, MCFloat x, MCFloat y, MCInt tag);

#endif /* defined(__monkcGame__MC3DiOSDriver__) */




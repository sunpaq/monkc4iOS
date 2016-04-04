//
//  MC3DiOSDriver.h
//  monkcGame
//
//  Created by SunYuLi on 5/18/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#ifndef __monkcGame__MC3DiOSDriver__
#define __monkcGame__MC3DiOSDriver__

//gesture callback
void onGestureSwip();
void onGesturePan(double x, double y);
void onGesturePinch(double scale);

//GL callback
void onRootViewLoad(void* rootview);
void onOpenExternalFile(const char* filepath);
void onReceiveMemoryWarning();
void onSetupGL(int windowWidth, int windowHeight);
void onTearDownGL();
void onUpdate();
void onDraw();

#ifdef __OBJC__
#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

@interface UIEventHandler : NSObject <UIGestureRecognizerDelegate>
@property (nonatomic) UIView* targetView;
@property UISwipeGestureRecognizer* swip;
@property UIPinchGestureRecognizer* pinch;
@property UIPanGestureRecognizer* pan;

- (void) onButtonClicked:(id)sender;
@end
#endif

#include <stdio.h>
#include "monkc.h"
#include "MC3DType.h"
#include "MCUIBase.h"
#include "MCBuffer.h"

#define MCMatrix4ToGLKMatrix4(mat) (GLKMatrix4){(mat).m00, (mat).m01, (mat).m02, (mat).m03, \
                                                (mat).m10, (mat).m11, (mat).m12, (mat).m13, \
                                                (mat).m20, (mat).m21, (mat).m22, (mat).m23, \
                                                (mat).m30, (mat).m31, (mat).m32, (mat).m33}

#define MCMatrix4FromGLKMatrix4(mat) (MCMatrix4){(mat).m00, (mat).m01, (mat).m02, (mat).m03, \
                                                 (mat).m10, (mat).m11, (mat).m12, (mat).m13, \
                                                 (mat).m20, (mat).m21, (mat).m22, (mat).m23, \
                                                 (mat).m30, (mat).m31, (mat).m32, (mat).m33}

MCMatrix4 MCMatrix4Multiply(MCMatrix4 matrixLeft, MCMatrix4 matrixRight);
MCMatrix4 MCMatrix4MakeLookAt(double eyeX, double eyeY, double eyeZ,
                              double centerX, double centerY, double centerZ,
                              double upX, double upY, double upZ);

MCUInt MCLoadSpriteTexture(const char* name, const char* suffix);

//UI parts use UIKit
void MCUIRegisterRootUIView(void* rootview);
void MCUIAddLabelButton(const char* bgname, const char* labelname, MCColor color, double x, double y, MCInt tag, MCBool isContinous);
void MCUIAddLabel(const char* labelname, MCColor color, double x, double y, MCInt tag);
void MCUILabelTextUpdate(const char* newtext, MCInt tag);
void MCUIButtonRegisterCallback(mc_message msg);

//File
void MCFileGetPath(const char* filename, const char* extention, char* buffer);
const char* MCFileCopyContent(const char* filename, const char* extention);

#endif /* defined(__monkcGame__MC3DiOSDriver__) */




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

static UIView* _rootUIView = nil;
static UIEventHandler* _handler = nil;
static mc_message onButtonClickMsg = {nil, nil};

#ifdef __OBJC__
@implementation UIEventHandler

- (void) onButtonClicked:(id)sender
{
    UIButton* btn = (UIButton*)sender;
    if (onButtonClickMsg.address) {
        _push_jump(onButtonClickMsg, (MCInt)btn.tag);
    }
}

@end
#endif /* __OBJC__ */

MCMatrix4 MCMatrix4Multiply(MCMatrix4 matrixLeft, MCMatrix4 matrixRight)
{
    return MCMatrix4FromGLKMatrix4(GLKMatrix4Multiply(MCMatrix4ToGLKMatrix4(matrixLeft),
                                                      MCMatrix4ToGLKMatrix4(matrixRight)));
}

MCMatrix4 MCMatrix4MakeLookAt(MCFloat eyeX, MCFloat eyeY, MCFloat eyeZ,
                              MCFloat centerX, MCFloat centerY, MCFloat centerZ,
                              MCFloat upX, MCFloat upY, MCFloat upZ)
{
    return MCMatrix4FromGLKMatrix4(GLKMatrix4MakeLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ));
}

void MCUIRegisterRootUIView(void* rootview)
{
    _rootUIView = (__bridge UIView*)rootview;
    _handler = [UIEventHandler new];
}

void MCUIAddLabelButton(const char* bgname, const char* labelname, MCColor color, MCFloat x, MCFloat y, MCInt tag)
{
    if (_rootUIView) {
        UIButton* btn = [UIButton buttonWithType:UIButtonTypeCustom];
        [btn setTitle:[NSString stringWithCString:labelname encoding:NSUTF8StringEncoding] forState:UIControlStateNormal];
        [btn setTitleColor:[UIColor colorWithRed:color.R green:color.G blue:color.B alpha:1.0] forState:UIControlStateNormal];
        [btn sizeToFit];
        btn.center = CGPointMake(x, y);
        btn.tag = tag;
        [btn addTarget:_handler action:@selector(onButtonClicked:) forControlEvents:UIControlEventTouchUpInside];
        [_rootUIView addSubview:btn];
    }
}

void MCUIButtonRegisterCallback(mc_message msg)
{
    onButtonClickMsg = msg;
}

//GLKTextureLoader
MCUInt MCLoadSpriteTexture(const char* name, const char* suffix)
{
    NSString* spname = [NSString stringWithUTF8String:name];
    NSString* spsuffix = [NSString stringWithUTF8String:suffix];
    GLKTextureInfo *spriteTexture;
    NSError *theError;
    NSString *filePath = [[NSBundle mainBundle] pathForResource:spname ofType:spsuffix]; // 1
    spriteTexture = [GLKTextureLoader textureWithContentsOfFile:filePath options:nil error:&theError]; // 2
    
    if (theError == nil) {
        glBindTexture(spriteTexture.target, spriteTexture.name); // 3
        glEnable(spriteTexture.target); // 4
        return (MCUInt)spriteTexture.name;
    }else{
        return 0;
    }
}

void MCFileGetPath(const char* filename, const char* extention, char* buffer)
{
    NSString* path = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:filename]
                                                     ofType:[NSString stringWithUTF8String:extention]];
    const char * cstr = [path cStringUsingEncoding:NSUTF8StringEncoding];
    strcpy(buffer, cstr);
}







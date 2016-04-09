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
#import "MC3DiOS.h"

static UIView* _rootUIView = nil;
static UIEventHandler* _handler = nil;
static mc_message onButtonClickMsg = {nil, nil};

MCMatrix4 MCMatrix4Multiply(MCMatrix4 matrixLeft, MCMatrix4 matrixRight)
{
    return MCMatrix4FromGLKMatrix4(GLKMatrix4Multiply(MCMatrix4ToGLKMatrix4(matrixLeft),
                                                      MCMatrix4ToGLKMatrix4(matrixRight)));
}

MCMatrix4 MCMatrix4MakeLookAt(double eyeX, double eyeY, double eyeZ,
                              double centerX, double centerY, double centerZ,
                              double upX, double upY, double upZ)
{
    return MCMatrix4FromGLKMatrix4(GLKMatrix4MakeLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ));
}

void MCUIRegisterRootUIView(void* rootview)
{
    _rootUIView = (__bridge UIView*)rootview;
    _handler = [UIEventHandler new];
    _handler.targetView = _rootUIView;
    
    [_handler.targetView addGestureRecognizer:_handler.pan];
    [_handler.targetView addGestureRecognizer:_handler.pinch];
    [_handler.targetView addGestureRecognizer:_handler.swip];
}

void MCUIAddLabelButton(const char* bgname, const char* labelname, MCColor color, double x, double y, MCInt tag, MCBool isContinuous)
{
    if (_rootUIView) {
        UIButton* btn = [UIButton buttonWithType:UIButtonTypeCustom];
        [btn setTitle:[NSString stringWithCString:labelname encoding:NSUTF8StringEncoding] forState:UIControlStateNormal];
        [btn setTitleColor:[UIColor colorWithRed:color.R green:color.G blue:color.B alpha:1.0] forState:UIControlStateNormal];
        [btn sizeToFit];
        btn.center = CGPointMake(x, y);
        btn.tag = tag;
        if (isContinuous == MCTrue) {
            [btn addTarget:_handler action:@selector(onButtonClicked:) forControlEvents:UIControlEventAllTouchEvents];
        }else{
            [btn addTarget:_handler action:@selector(onButtonClicked:) forControlEvents:UIControlEventTouchDown];
        }
        [_rootUIView addSubview:btn];
    }
}

void MCUIAddLabel(const char* labelname, MCColor color, double x, double y, MCInt tag)
{
    if (_rootUIView) {
        UILabel* label = [UILabel new];
        label.text = [NSString stringWithCString:labelname encoding:NSUTF8StringEncoding];
        label.textColor = [UIColor colorWithRed:color.R/255 green:color.G/255 blue:color.B/255 alpha:1.0];
        [label sizeToFit];
        label.center = CGPointMake(x, y);
        label.tag = tag;
        [_rootUIView addSubview:label];
    }
}

void MCUILabelTextUpdate(const char* newtext, MCInt tag)
{
    UILabel* label = [_rootUIView viewWithTag:tag];
    label.text = [NSString stringWithCString:newtext encoding:NSUTF8StringEncoding];
    [label sizeToFit];
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
    CFStringRef fname = CFStringCreateWithCString(NULL, filename, kCFStringEncodingUTF8);
    CFStringRef  fext = CFStringCreateWithCString(NULL, extention, kCFStringEncodingUTF8);
    CFURLRef      url = CFBundleCopyResourceURL(CFBundleGetMainBundle(), fname, fext, NULL);
    CFStringRef  path = CFURLCopyPath(url);
    
    CFStringGetCString(path, buffer, PATH_MAX, kCFStringEncodingUTF8);
    
    CFRelease(fname);
    CFRelease(fext);
    CFRelease(path);
    CFRelease(url);
}

const char* MCFileCopyContent(const char* filename, const char* extention)
{
    char path[PATH_MAX];
    MCFileGetPath(filename, extention, path);
    
    FILE* f = fopen(path, "r");
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* const buffer = (char*)malloc(size * sizeof(char));
    char* iter = buffer;
    
    if (f != NULL) {
        char c;
        while ((c = fgetc(f)) != EOF) {
            *iter++ = c;
        }
        *iter = '\0';
    }
    
    return buffer;
}

void MCGLError(const char* errmsg)
{
    if (_handler) {
        [_handler onMCGLError:errmsg];
    }
}

#ifdef __OBJC__
@implementation UIEventHandler

//using ARC no need to release members
-(instancetype)init
{
    if ([super init]) {
        //gesture
        self.swip = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(onSwip:)];
        self.swip.direction = UISwipeGestureRecognizerDirectionRight | UISwipeGestureRecognizerDirectionLeft;
        self.swip.delegate = self;
        
        self.pinch = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(onPinch:)];
        self.pinch.delegate = self;
        
        self.pan = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(onPan:)];
        self.pan.delegate = self;
        
        return self;
    }else{
        return nil;
    }
}

- (void) onButtonClicked:(id)sender
{
    UIButton* btn = (UIButton*)sender;
    if (onButtonClickMsg.address) {
        _push_jump(onButtonClickMsg, (MCInt)btn.tag);
    }
}

- (void) onMCGLError:(const char*)errmsg
{
    [[[UIAlertView alloc] initWithTitle:@"ERROR"
                                message:[NSString stringWithUTF8String:errmsg]
                               delegate:self
                      cancelButtonTitle:@"OK, got it."
                      otherButtonTitles:nil, nil] show];
}

- (void)onSwip:(id)sender
{
    if (sender == self.swip) {
        onGestureSwip();
    }
}

- (void)onPinch:(id)sender
{
    if (sender == self.pinch) {
        if (self.pinch.velocity > 0) {
            onGesturePinch(-self.pinch.scale);
        }else{
            onGesturePinch(self.pinch.scale);
        }
    }
}

- (void)onPan:(id)sender
{
    if (sender == self.pan) {
        CGPoint trans = [self.pan translationInView:self.targetView];
        onGesturePan(trans.x, trans.y);
    }
}

- (BOOL)gestureRecognizerShouldBegin:(UIGestureRecognizer *)gestureRecognizer
{
    if (gestureRecognizer == self.swip) {
        return true;
    }
    else if (gestureRecognizer == self.pinch) {
        return true;
    }
    else if (gestureRecognizer == self.pan) {
        return true;
    }
    
    return false;
}

@end
#endif /* __OBJC__ */






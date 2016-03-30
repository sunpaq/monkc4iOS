//
//  GameViewController.m
//  monkcGame
//
//  Created by SunYuLi on 5/15/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#import "GameViewController.h"
#import "MainLoop.h"

@interface GameViewController () {

}
@property (strong, nonatomic) EAGLContext *context;

- (void)setupGL;
- (void)tearDownGL;
@end

@implementation GameViewController

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
        CGPoint trans = [self.pan translationInView:self.view];
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

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];

    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    //gesture
    self.swip = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(onSwip:)];
    self.swip.direction = UISwipeGestureRecognizerDirectionRight | UISwipeGestureRecognizerDirectionLeft;
    self.swip.delegate = self;
    
    self.pinch = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(onPinch:)];
    self.pinch.delegate = self;
    
    self.pan = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(onPan:)];
    self.pan.delegate = self;
    
    [view addGestureRecognizer:self.swip];
    [view addGestureRecognizer:self.pinch];
    [view addGestureRecognizer:self.pan];

    //register rootview
    onRootViewLoad((__bridge void *)(view));
    [self setupGL];
}

- (void)dealloc
{    
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];

    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;
        
        [self tearDownGL];
        
        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }

    // Dispose of any resources that can be recreated.
}

- (BOOL)prefersStatusBarHidden {
    return YES;
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
    
    //setup monkc
    double width = self.view.bounds.size.width;
    double height = self.view.bounds.size.height;
    
    NSString* vertShaderPathname = [[NSBundle mainBundle] pathForResource:@"MCGLRenderer" ofType:@"vsh"];
    GLchar* vsource = (GLchar*)[[NSString stringWithContentsOfFile:vertShaderPathname
                                                          encoding:NSUTF8StringEncoding error:nil] UTF8String];
    
    NSString* fragShaderPathname = [[NSBundle mainBundle] pathForResource:@"MCGLRenderer" ofType:@"fsh"];
    GLchar* fsource = (GLchar*)[[NSString stringWithContentsOfFile:fragShaderPathname
                                                          encoding:NSUTF8StringEncoding error:nil] UTF8String];
    
    NSString* texturePath = [[NSBundle mainBundle] pathForResource:@"tex8" ofType:@"bmp"];
    GLchar* texture = (GLchar*)[texturePath UTF8String];
    
    NSString* obj3dPath = [[NSBundle mainBundle] pathForResource:@"monkey2" ofType:@"obj"];
    GLchar* obj3d = (GLchar*)[obj3dPath UTF8String];
    
    NSString* cubePath = [[NSBundle mainBundle] pathForResource:@"cube" ofType:@"obj"];//teapot
    GLchar* cube = (GLchar*)[cubePath UTF8String];
    
    const char* filePathArray[] = {vsource, fsource, texture, obj3d, cube};
    onSetupGL(width, height, filePathArray);
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
    
    //clean monkc
    onTearDownGL();
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
    // monkc update
    onUpdate();
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    // monkc draw
    onDraw();
}

@end

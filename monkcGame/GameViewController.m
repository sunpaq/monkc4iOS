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
    GLKMatrix4 _modelViewProjectionMatrix;
    GLKMatrix3 _normalMatrix;
    float _rotation;
    
    GLuint _vertexArray;
    GLuint _vertexBuffer;
}
@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

- (void)setupGL;
- (void)tearDownGL;
@end

@implementation GameViewController

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
    
    self.effect = [[GLKBaseEffect alloc] init];
    self.effect.light0.enabled = GL_TRUE;
    self.effect.light0.ambientColor = GLKVector4Make(1.0f, 1.0f, 1.0f, 1.0f);
    self.effect.light0.diffuseColor = GLKVector4Make(1.0f, 1.0f, 1.0f, 1.0f);
    self.effect.light0.specularColor = GLKVector4Make(1.0f, 1.0f, 1.0f, 1.0f);
    
    //setup monkc
    double width = self.view.bounds.size.width;
    double height = self.view.bounds.size.height;
    onSetupGL(width, height);
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
    
    //clean monkc
    onTearDownGL();

    self.effect = nil;
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
    // monkc update
    onUpdate(self.timeSinceLastUpdate);
    
    MCMatrix4 pm = onUpdateProjectionMatrix();
    MCMatrix4 vm = onUpdateModelViewMatrix();
    
    self.effect.transform.projectionMatrix = MCMatrix4ToGLKMatrix4(pm);
    self.effect.transform.modelviewMatrix = MCMatrix4ToGLKMatrix4(vm);
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    // Render the object with GLKit
    [self.effect prepareToDraw];
    // monkc draw
    onDraw();
}

@end

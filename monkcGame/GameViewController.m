//
//  GameViewController.m
//  monkcGame
//
//  Created by SunYuLi on 5/15/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#import "GameViewController.h"
#import "MC3DiOS.h"

@interface GameViewController () {

}

@property (strong, nonatomic) EAGLContext *context;

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
    
    //split view
    if (self.splitViewController != nil) {
        self.splitViewController.delegate = self;
    }
}

-(void)viewWillAppear:(BOOL)animated
{
	[super viewWillAppear:animated];
	//register rootview
	if (self.uiview) {
		onRootViewLoad((__bridge void *)(self.uiview));
	}
	
	//OpenGL
	[self setupGL:(GLKView *)self.view];
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

- (void)setupGL:(GLKView*)glv
{
    [EAGLContext setCurrentContext:self.context];
    
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
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
    //monkc update
    onUpdate();
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    //monkc draw
    onDraw();
}

//UISplitViewControllerDelegate
//-(BOOL)splitViewController:(UISplitViewController *)splitViewController
//collapseSecondaryViewController:(UIViewController *)secondaryViewController
// ontoPrimaryViewController:(UIViewController *)primaryViewController
//{
//    return YES;
//}

-(UIViewController *)primaryViewControllerForCollapsingSplitViewController:(UISplitViewController *)splitViewController
{
    return nil;
}

-(UIViewController *)primaryViewControllerForExpandingSplitViewController:(UISplitViewController *)splitViewController
{
    return nil;//default
}

-(BOOL)splitViewController:(UISplitViewController *)splitViewController showViewController:(UIViewController *)vc sender:(id)sender
{

	if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad) {
		return YES;
	}else{
		return NO;
	}
}

//-(BOOL)splitViewController:(UISplitViewController *)splitViewController showDetailViewController:(UIViewController *)vc sender:(id)sender
//{
//    return YES;
//}

-(void)viewDidLayoutSubviews
{
    CGSize size = self.view.bounds.size;
    onResizeScreen(size.width, size.height);

}

-(IBAction)onFullScreenClicked:(id)sender
{
	//[self.parentViewController dismissViewControllerAnimated:self.parentViewController completion:nil];
}

-(IBAction)hideUnhideMaster:(id)sender
{
}

@end

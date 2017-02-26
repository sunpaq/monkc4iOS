//
//  GameViewController.m
//  monkcGame
//
//  Created by SunYuLi on 5/15/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#import "GameViewController.h"
#import <CoreMotion/CoreMotion.h>
#import "MC3DiOS.h"

@interface GameViewController () {
	double _savedCameraDistance;
	double _cameraHeightRatio; // height/100(stepper.value)
}

@property (strong, nonatomic) EAGLContext *context;
@property CMMotionManager* motionManager;

- (void)tearDownGL;
@end

@implementation GameViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    if (!self.context) {
        NSLog(@"Failed to create ES context");
        exit(-1);
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    view.drawableStencilFormat = GLKViewDrawableStencilFormat8;
    if([[NSUserDefaults standardUserDefaults] boolForKey:@"MAAOn"]) {
        view.drawableMultisample = GLKViewDrawableMultisample4X;
        self.naviItem.prompt = @"Multisampling Anti-Alias On";
    }
    else {
        view.drawableMultisample = GLKViewDrawableMultisampleNone;
        self.naviItem.prompt = nil;
    }

    //split view
    if (self.splitViewController != nil) {
        self.splitViewController.delegate = self;
    }
	
    self.naviItem.title = self.filename;
}

-(BOOL)isRotateOrPan
{
	if ([self.rotatePan.titleLabel.text isEqualToString:@"Rotate"]) {
		return YES;
	}
	else if ([self.rotatePan.titleLabel.text isEqualToString:@"Pan"]) {
		return NO;
	}
	return YES;
}

-(void)setIsRotateOrPan:(BOOL)b
{
	if (b) {
		[self.stepper setTintColor:[UIColor blueColor]];
		[self.rotatePan setTitleColor:[UIColor blueColor] forState:UIControlStateNormal];
		[self.rotatePan setTitle:@"Rotate" forState:UIControlStateNormal];

	}else{
		[self.stepper setTintColor:[UIColor redColor]];
		[self.rotatePan setTitleColor:[UIColor redColor] forState:UIControlStateNormal];
		[self.rotatePan setTitle:@"Pan" forState:UIControlStateNormal];
	}
}

-(BOOL)isTriangleOrWire
{
    if ([self.triangleWire.titleLabel.text isEqualToString:@"T"]) {
        return YES;
    }
    else if ([self.triangleWire.titleLabel.text isEqualToString:@"W"]) {
        return NO;
    }
    return YES;
}

-(void)setIsTriangleOrWire:(BOOL)b
{
    if (b) {
        onDrawModeChange(1);
        [self.triangleWire setTitleColor:[UIColor blueColor] forState:UIControlStateNormal];
        [self.triangleWire setTitle:@"T" forState:UIControlStateNormal];
    }else{
        onDrawModeChange(0);
        [self.triangleWire setTitleColor:[UIColor redColor] forState:UIControlStateNormal];
        [self.triangleWire setTitle:@"W" forState:UIControlStateNormal];
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

-(void)viewDidAppear:(BOOL)animated
{
    const char* cfile = [self.filename cStringUsingEncoding:NSUTF8StringEncoding];
    onOpenFile(cfile);
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
    
    //Core Motion
//    if([[NSUserDefaults standardUserDefaults] valueForKey:@"SkyboxOn"]) {
//        [self startDeviceMotion];
//    }
    
    [self startDeviceMotion];
}

- (void)tearDownGL
{
    //Core Motion
//    if([self.motionManager isDeviceMotionActive]) {
//        [self stopDeviceMotion];
//    }

    [EAGLContext setCurrentContext:self.context];
    
    //clean monkc
    onTearDownGL();
}

- (void)startDeviceMotion
{
    self.motionManager = [[CMMotionManager alloc] init];

    if ([self.motionManager isAccelerometerAvailable]) {
        self.motionManager.deviceMotionUpdateInterval = 1.0 / 60.0;
        [self.motionManager startDeviceMotionUpdatesUsingReferenceFrame:CMAttitudeReferenceFrameXArbitraryZVertical];
    }
}

- (void)stopDeviceMotion
{
    if ([self.motionManager isAccelerometerAvailable]) {
        [self.motionManager stopDeviceMotionUpdates];
        self.motionManager = nil;
    }
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
    CMAttitude* att = nil;
    
    //Core Motion
    att = self.motionManager.deviceMotion.attitude;
    
    //monkc update
    if (att) {
        CMRotationMatrix mat = att.rotationMatrix;
        float data[9] = {mat.m11, mat.m12, mat.m13, mat.m21, mat.m22, mat.m23, mat.m31, mat.m32, mat.m33};
        onUpdate(data);
    }else{
        onUpdate(nil);
    }
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    //monkc draw
    int fps = onDraw();
	if (self.fpsLabel && fps > 0) {
		self.fpsLabel.text = [NSString stringWithFormat:@"fps:%d", fps];
	}
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

//-(BOOL)splitViewController:(UISplitViewController *)splitViewController showViewController:(UIViewController *)vc sender:(id)sender
//{
//
//	if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad) {
//		return YES;
//	}else{
//		return NO;
//	}
//}

//-(BOOL)splitViewController:(UISplitViewController *)splitViewController showDetailViewController:(UIViewController *)vc sender:(id)sender
//{
//    return YES;
//}

-(void)viewDidLayoutSubviews
{
    CGSize size = self.view.bounds.size;
    onResizeScreen(size.width, size.height);

}

- (IBAction)startStopBtnClicked:(id)sender {
	UISwitch* s = (UISwitch*)sender;
	if (s && s.isOn) {
		onStartStopBtn(1);
	}else{
		onStartStopBtn(0);
	}
}

- (IBAction)steperValueChange:(id)sender {
	UIStepper* s = (UIStepper*)sender;
	if ([self isRotateOrPan] == YES) {
		double rpercent = s.value / 100;
		//Currently Rotate
		MC3DiOS_CameraCmd cmd;
		cmd.type = MC3DiOS_CameraRadiusPercent;
		cmd.rpercent = rpercent;
		cameraCommand(&cmd);
		
		self.camDistance.text = [NSString stringWithFormat:@"distance:%dm", (int)s.value];
	}
	else if ([self isRotateOrPan] == NO) {
		//Currently Pan
		MC3DiOS_CameraCmd cmd;
		cmd.type = MC3DiOS_CameraLookAt;
		cmd.lookatX = 0;
		cmd.lookatY = s.value * _cameraHeightRatio;
		cmd.lookatZ = 0;
		cameraCommand(&cmd);
		
		self.camDistance.text = [NSString stringWithFormat:@"height:%dm", (int)s.value];
	}
}

- (IBAction)rotatePanSwitch:(id)sender {
	if ([self isRotateOrPan] == YES) {
		//Rotate -> Pan
		self.camDistance.text = @"camera height";
		//get current info
		MC3DiOS_CameraCmd cmd;
		cmd.type = MC3DiOS_GetCurrent;
		cameraCommand(&cmd);
		//save
		_savedCameraDistance = self.stepper.value;
		_cameraHeightRatio = cmd.lookatY / 100;
		//lock rotation
		cmd.type = MC3DiOS_LockRotation;
		cmd.lockRotation = 1;
		cameraCommand(&cmd);
		//set state
		[self setIsRotateOrPan:NO];
	}
	else if ([self isRotateOrPan] == NO) {
		//Pan -> Rotate
		self.camDistance.text = @"camera distance";
		self.stepper.value = _savedCameraDistance;
		//unlock
		MC3DiOS_CameraCmd cmd;
		cmd.type = MC3DiOS_LockRotation;
		cmd.lockRotation = 0;
		cameraCommand(&cmd);
		//set state
		[self setIsRotateOrPan:YES];
	}
}

- (IBAction)triangleWireSwitch:(id)sender {
    if ([self isTriangleOrWire] == YES) {
        [self setIsTriangleOrWire:NO];
    }
    else if ([self isTriangleOrWire] == NO) {
        [self setIsTriangleOrWire:YES];
    }
}

@end

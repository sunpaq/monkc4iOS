//
//  GameViewController.h
//  monkcGame
//
//  Created by SunYuLi on 5/15/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

@interface GameViewController : GLKViewController <UISplitViewControllerDelegate>

//Outlets
@property (strong, nonatomic) IBOutlet GLKView *uiview;
@property (weak, nonatomic) IBOutlet UILabel *fpsLabel;
@property (weak, nonatomic) IBOutlet UIStepper *stepper;
@property (weak, nonatomic) IBOutlet UINavigationItem *naviItem;
@property (weak, nonatomic) IBOutlet UILabel *camDistance;
@property (weak, nonatomic) IBOutlet UIButton *rotatePan;
@property (weak, nonatomic) IBOutlet UIToolbar *toolbar;

@property NSString* filename;

- (void) onOpenModelStart:(NSNotification*)noti;
- (void) onOpenModelStop:(NSNotification*)noti;

@end


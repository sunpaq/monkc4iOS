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

@property (strong, nonatomic) IBOutlet GLKView *uiview;
@property (weak, nonatomic) IBOutlet UILabel *fpsLabel;
@property (weak, nonatomic) IBOutlet UIStepper *stepper;

@property NSString* filename;
@end


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
@property IBOutlet UIView* uiview;

-(IBAction)onFullScreenClicked:(id)sender;
-(IBAction)hideUnhideMaster:(id)sender;

@property (weak, nonatomic) IBOutlet UIView *hudView;
@property (weak, nonatomic) IBOutlet UILabel *fpsLabel;

@end


//
//  SplitViewController.m
//  monkcGame
//
//  Created by Sun YuLi on 16/4/10.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#import "SplitViewController.h"

@implementation SplitViewController

-(void)viewDidLoad
{
    [super viewDidLoad];
//    UISplitViewControllerDisplayModeAutomatic,
//    UISplitViewControllerDisplayModePrimaryHidden,
//    UISplitViewControllerDisplayModeAllVisible,
//    UISplitViewControllerDisplayModePrimaryOverlay,
    self.preferredDisplayMode = UISplitViewControllerDisplayModePrimaryHidden;
}

@end

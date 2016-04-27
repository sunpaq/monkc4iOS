//
//  SplitViewController.m
//  monkcGame
//
//  Created by Sun YuLi on 16/4/10.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#import "SplitViewController.h"

@implementation SplitViewController

//    UISplitViewControllerDisplayModeAutomatic,
//    UISplitViewControllerDisplayModePrimaryHidden,
//    UISplitViewControllerDisplayModeAllVisible,
//    UISplitViewControllerDisplayModePrimaryOverlay,

-(void)viewDidLoad
{
    [super viewDidLoad];

    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
        self.preferredDisplayMode = UISplitViewControllerDisplayModeAutomatic;

    }else{
        self.preferredDisplayMode = UISplitViewControllerDisplayModePrimaryOverlay;

    }
}

@end
